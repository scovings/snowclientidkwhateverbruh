#include "main.h"
#include "modules/modules.h"


static WNDPROC window_proc;

LRESULT CALLBACK h_WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_KEYDOWN) {
        // pressed down
    }

    if (uMsg == WM_KEYUP) {
        std::cout << "Key Press Ran " << (char)wParam << std::endl;
        emit("105");
    }

    return CallWindowProc(window_proc, hWnd, uMsg, wParam, lParam);
}

void setup_wndproc() {
    HWND window = FindWindowA(NULL, "LWJGL");
    if (!window) {
        printf("[!] ERROR: Window not found, failed to set up wndproc\n");
    }
    window_proc = (WNDPROC)SetWindowLongPtrA(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(h_WndProc));
}


void client_main(HMODULE mod) {


    if (Hook::init())
        Hook::shutdown();


    // Game loop ill just go back to the injector... 
    while (true) {
        Sleep(10);
        emit("on-tick");
        CheckForKeys();
        if (GetAsyncKeyState(VK_END)) {
            break;
        }
    }

    // Cleanup
    for (Module* mod : mods) {
        delete mod;
    }

    HWND window = FindWindowA(NULL, "Minecraft 1.8.9");
    SetWindowLongPtrA(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(window_proc));

        // Cleanup ImGui


    // Destroy the GLFW window and terminate GLFW


    printf("[-] Client Shutdown Executed!\n");
    FreeConsole();
    FreeLibraryAndExitThread(mod, 0);
    return;
 
}

