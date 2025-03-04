import tkinter as tk
from tkinter import filedialog, messagebox
import os  # For file validation
os.system("pip install psutil customtkinter")

import customtkinter as ctk
import psutil  # For process list
import ctypes
from ctypes import wintypes

# Define necessary constants
PROCESS_CREATE_THREAD = 0x0002
PROCESS_QUERY_INFORMATION = 0x0400
PROCESS_VM_OPERATION = 0x0008
PROCESS_VM_WRITE = 0x0020
PROCESS_VM_READ = 0x0010
MEM_COMMIT = 0x1000
MEM_RESERVE = 0x2000
PAGE_READWRITE = 0x04

# Load kernel32.dll
kernel32 = ctypes.WinDLL("kernel32.dll", use_last_error=True)

# Define Windows API functions
OpenProcess = kernel32.OpenProcess
OpenProcess.argtypes = (wintypes.DWORD, wintypes.BOOL, wintypes.DWORD)
OpenProcess.restype = wintypes.HANDLE

VirtualAllocEx = kernel32.VirtualAllocEx
VirtualAllocEx.argtypes = (wintypes.HANDLE, wintypes.LPVOID, ctypes.c_size_t, wintypes.DWORD, wintypes.DWORD)
VirtualAllocEx.restype = wintypes.LPVOID

WriteProcessMemory = kernel32.WriteProcessMemory
WriteProcessMemory.argtypes = (wintypes.HANDLE, wintypes.LPVOID, wintypes.LPCVOID, ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t))
WriteProcessMemory.restype = wintypes.BOOL

GetProcAddress = kernel32.GetProcAddress
GetProcAddress.argtypes = (wintypes.HMODULE, wintypes.LPCSTR)
GetProcAddress.restype = wintypes.LPVOID

GetModuleHandle = kernel32.GetModuleHandleA
GetModuleHandle.argtypes = (wintypes.LPCSTR,)
GetModuleHandle.restype = wintypes.HMODULE

CreateRemoteThread = kernel32.CreateRemoteThread
CreateRemoteThread.argtypes = (wintypes.HANDLE, ctypes.POINTER(ctypes.c_void_p), ctypes.c_size_t, wintypes.LPVOID, wintypes.LPVOID, wintypes.DWORD, ctypes.POINTER(wintypes.DWORD))
CreateRemoteThread.restype = wintypes.HANDLE

CloseHandle = kernel32.CloseHandle
CloseHandle.argtypes = (wintypes.HANDLE,)
CloseHandle.restype = wintypes.BOOL

# Global variables
recent_dlls = []

def get_pid_by_name(process_name):
    """Returns the first process ID matching the given process name."""
    for process in psutil.process_iter(attrs=['pid', 'name']):
        if process.info['name'].lower() == process_name.lower():
            return process.info['pid']
    return None  # No matching process found

def inject_dll(process_id, dll_path):
    process_handle = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        False,
        process_id
    )
    if not process_handle:
        return f"Failed to open target process! Error: {ctypes.get_last_error()}"

    allocated_memory = VirtualAllocEx(
        process_handle,
        None,
        len(dll_path) + 1,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    )
    if not allocated_memory:
        CloseHandle(process_handle)
        return f"Failed to allocate memory in target process! Error: {ctypes.get_last_error()}"

    written = ctypes.c_size_t(0)
    write_result = WriteProcessMemory(
        process_handle,
        allocated_memory,
        dll_path.encode("utf-8"),
        len(dll_path) + 1,
        ctypes.byref(written)
    )
    if not write_result:
        CloseHandle(process_handle)
        return f"Failed to write DLL path to target process! Error: {ctypes.get_last_error()}"

    load_library_address = GetProcAddress(GetModuleHandle(b"kernel32.dll"), b"LoadLibraryA")
    if not load_library_address:
        CloseHandle(process_handle)
        return f"Failed to get address of LoadLibraryA! Error: {ctypes.get_last_error()}"

    thread_handle = CreateRemoteThread(
        process_handle,
        None,
        0,
        load_library_address,
        allocated_memory,
        0,
        None
    )
    if not thread_handle:
        CloseHandle(process_handle)
        return f"Failed to create remote thread in target process! Error: {ctypes.get_last_error()}"

    CloseHandle(thread_handle)
    CloseHandle(process_handle)
    return "DLL injected successfully!"

def browse_dll():
    dll_path = filedialog.askopenfilename(filetypes=[("DLL Files", "*.dll")])
    if dll_path:
        dll_path_entry.delete(0, tk.END)
        dll_path_entry.insert(0, dll_path)
        add_to_recent_dlls(dll_path)

def add_to_recent_dlls(dll_path):
    if dll_path not in recent_dlls:
        recent_dlls.append(dll_path)
        recent_dlls.add_command(label=dll_path, command=lambda p=dll_path: dll_path_entry.insert(0, p))

def start_injection():
    process_name = process_id_entry.get()  # Now used for process name input
    dll_path = dll_path_entry.get()

    if not process_name:
        status_var.set("Error: Process name cannot be empty!")
        return

    process_id = get_pid_by_name(process_name)
    if not process_id:
        status_var.set(f"Error: No process named '{process_name}' found!")
        return

    if not dll_path or not os.path.exists(dll_path):
        status_var.set("Error: Please select a valid DLL file!")
        return

    result = inject_dll(process_id, dll_path)
    status_var.set(result)

# Set up the CustomTkinter theme
ctk.set_appearance_mode("System")
ctk.set_default_color_theme("blue")

# Create the main window
root = ctk.CTk()
root.title("DLL Injector")
root.geometry("500x400")
root.resizable(False, False)

# Create a main frame
main_frame = ctk.CTkFrame(root)
main_frame.pack(fill=tk.BOTH, expand=True, padx=20, pady=20)

# Process Name (Instead of Process ID)
ctk.CTkLabel(main_frame, text="Process Name:").grid(row=0, column=0, sticky="ew", pady=(10, 5))
process_id_entry = ctk.CTkEntry(main_frame, width=300, placeholder_text="Enter Process Name")  # Updated placeholder
process_id_entry.grid(row=0, column=1, sticky="ew", pady=(10, 5))

# DLL Path
ctk.CTkLabel(main_frame, text="DLL Path:").grid(row=1, column=0, sticky="ew", pady=(10, 5))
dll_path_entry = ctk.CTkEntry(main_frame, width=300, placeholder_text="Select DLL File")
dll_path_entry.grid(row=1, column=1, sticky="ew", pady=(10, 5))
browse_button = ctk.CTkButton(main_frame, text="Browse", command=browse_dll, width=80)
browse_button.grid(row=1, column=2, sticky="ew", pady=(10, 5))

# Inject Button
inject_button = ctk.CTkButton(main_frame, text="Inject DLL", command=start_injection, width=120)
inject_button.grid(row=2, column=0, columnspan=3, sticky="ew", pady=20)

# Status Bar Label
ctk.CTkLabel(main_frame, text="Status Bar:").grid(row=3, column=0, columnspan=3, sticky="ew", pady=(10, 5))

# Status Bar
status_var = tk.StringVar()
status_var.set("Ready")
status_bar = ctk.CTkLabel(main_frame, textvariable=status_var, fg_color=("gray75", "gray25"), corner_radius=5, height=30)
status_bar.grid(row=4, column=0, columnspan=3, sticky="ew", pady=(10, 5))

# Run the application
root.mainloop()
