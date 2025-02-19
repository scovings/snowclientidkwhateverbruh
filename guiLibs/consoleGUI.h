#include <iostream>
#include <vector>
#include <cstdio>
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <thread>
#include <atomic>
#include <mutex>

#include "GUI.h"

// Custom console buffer
class ImGuiConsoleBuffer {
public:
    std::vector<std::string> logs;
    std::string current_line;
    bool autoScroll = true;
    std::mutex log_mutex;

    void append(const char* text, size_t len) {
        std::lock_guard<std::mutex> lock(log_mutex);
        for (size_t i = 0; i < len; ++i) {
            if (text[i] == '\n') {
                logs.push_back(current_line);
                current_line.clear();
            } else {
                current_line += text[i];
            }
        }
    }

    void clear() {
        std::lock_guard<std::mutex> lock(log_mutex);
        logs.clear();
        current_line.clear();
    }
};

ImGuiConsoleBuffer consoleBuffer;

// Pipe and thread for stdout redirection
HANDLE g_hReadPipe;
HANDLE g_hWritePipe;
std::atomic<bool> g_keepRunning(true);

// Thread function to read from pipe
void ReadFromPipe() {
    char buffer[1024];
    DWORD bytesRead;

    while (g_keepRunning) {
        if (ReadFile(g_hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            consoleBuffer.append(buffer, bytesRead);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// Redirect stdout to pipe
void RedirectStdoutToPipe() {
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };
    CreatePipe(&g_hReadPipe, &g_hWritePipe, &sa, 0);

    // Redirect stdout to the pipe
    int fd = _open_osfhandle(reinterpret_cast<intptr_t>(g_hWritePipe), _O_TEXT);
    FILE* fp = _fdopen(fd, "w");
    *stdout = *fp;
    setvbuf(stdout, nullptr, _IONBF, 0);

    // Start reader thread
    std::thread(ReadFromPipe).detach();
}

// Display ImGui console
void ShowConsoleWindow() {
    ImGui::Begin("Console Output");

    if (ImGui::Button("Clear")) consoleBuffer.clear();
    ImGui::SameLine();
    ImGui::Checkbox("Auto Scroll", &consoleBuffer.autoScroll);

    ImGui::Separator();
    ImGui::BeginChild("ConsoleScroll", ImVec2(0, 0), true);

    std::lock_guard<std::mutex> lock(consoleBuffer.log_mutex);
    for (const auto& line : consoleBuffer.logs) {
        ImGui::TextUnformatted(line.c_str());
    }

    if (consoleBuffer.autoScroll) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
    ImGui::End();
}

// Cleanup when exiting
void CleanupConsoleRedirection() {
    g_keepRunning = false;
    CloseHandle(g_hReadPipe);
    CloseHandle(g_hWritePipe);
}

// Usage in your existing code
void RedirectConsole() {
    RedirectStdoutToPipe();
    std::cout << "[+] Redirected stdout and printf to ImGui!\n";
    printf("[+] Test message from printf\n");
}