#include <windows.h>    // Windows API for process manipulation, memory management, etc.
#include <tlhelp32.h>  // For creating process snapshots and retrieving process information
#include <iostream>    // For console input/output
#include <string>      // For using std::string to handle DLL paths and process names

// Function to inject a DLL into a target process
bool InjectDLL(DWORD processID, const std::string& dllPath) {
    // Step 1: Open the target process with all access rights
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (hProcess == NULL) {
        std::cerr << "Failed to open target process. Error: " << GetLastError() << std::endl;
        return false; // Exit if we can't open the process
    }

    // Step 2: Allocate memory in the target process to store the DLL path
    // MEM_COMMIT reserves memory, PAGE_READWRITE makes it readable and writable
    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, dllPath.size() + 1, MEM_COMMIT, PAGE_READWRITE);
    if (pRemoteMemory == NULL) {
        std::cerr << "Failed to allocate memory in target process. Error: " << GetLastError() << std::endl;
        CloseHandle(hProcess); // Clean up the process handle
        return false; // Exit if memory allocation fails
    }

    // Step 3: Write the DLL path into the allocated memory in the target process
    if (!WriteProcessMemory(hProcess, pRemoteMemory, dllPath.c_str(), dllPath.size() + 1, NULL)) {
        std::cerr << "Failed to write to target process memory. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE); // Free the allocated memory
        CloseHandle(hProcess); // Clean up the process handle
        return false; // Exit if writing to memory fails
    }

    // Step 4: Get the address of LoadLibraryA function in kernel32.dll
    // LoadLibraryA is used to load the DLL into the target process
    LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    if (pLoadLibrary == NULL) {
        std::cerr << "Failed to get address of LoadLibraryA. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE); // Free the allocated memory
        CloseHandle(hProcess); // Clean up the process handle
        return false; // Exit if we can't find LoadLibraryA
    }

    // Step 5: Create a remote thread in the target process to execute LoadLibraryA
    // The thread will load the DLL using the path we wrote into memory
    HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pRemoteMemory, 0, NULL);
    if (hRemoteThread == NULL) {
        std::cerr << "Failed to create remote thread. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE); // Free the allocated memory
        CloseHandle(hProcess); // Clean up the process handle
        return false; // Exit if thread creation fails
    }

    // Step 6: Wait for the remote thread to finish executing
    WaitForSingleObject(hRemoteThread, INFINITE);

    // Step 7: Clean up resources
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE); // Free the allocated memory
    CloseHandle(hRemoteThread); // Close the remote thread handle
    CloseHandle(hProcess); // Close the process handle

    return true; // Return success if everything worked
}

// Function to retrieve the process ID of a running process by its name
DWORD GetProcessID(const std::string& processName) {
    // Step 1: Create a snapshot of all currently running processes
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create process snapshot. Error: " << GetLastError() << std::endl;
        return 0; // Exit if snapshot creation fails
    }

    // Step 2: Initialize a PROCESSENTRY32 structure to store process information
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    // Step 3: Retrieve information about the first process in the snapshot
    if (!Process32First(hSnapshot, &pe)) {
        std::cerr << "Failed to retrieve first process. Error: " << GetLastError() << std::endl;
        CloseHandle(hSnapshot); // Clean up the snapshot handle
        return 0; // Exit if we can't retrieve the first process
    }

    // Step 4: Loop through all processes in the snapshot
    do {
        // Check if the current process's name matches the target process name
        if (processName == pe.szExeFile) {
            CloseHandle(hSnapshot); // Clean up the snapshot handle
            return pe.th32ProcessID; // Return the process ID if a match is found
        }
    } while (Process32Next(hSnapshot, &pe)); // Move to the next process in the snapshot

    // Step 5: Clean up and return 0 if no matching process is found
    CloseHandle(hSnapshot);
    return 0;
}

int main() {
    // Step 1: Define the name of the target process and the full path to the DLL
    std::string processName = "target.exe"; // Replace with the name of the target process
    std::string dllPath = "C:\\path\\to\\your\\dll.dll"; // Replace with the full path to our DLL

    // Step 2: Retrieve the process ID of the target process
    DWORD processID = GetProcessID(processName);
    if (processID == 0) { // PID Num
        std::cerr << "Failed to find target process." << std::endl;
        return 1; // Exit if the target process is not found
    }

    // Step 3: Attempt to inject the DLL into the target process
    if (InjectDLL(processID, dllPath)) {
        std::cout << "DLL injected successfully!" << std::endl;
    } else {
        std::cerr << "DLL injection failed." << std::endl;
    }

    return 0;
}