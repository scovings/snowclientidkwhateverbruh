#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>
#include <thread>
#include <windows.h>
#include "Vars.h"
#include <tchar.h>
class EventBus {
public:
    using EventCallback = std::function<void()>;

    static EventBus& getInstance() {
        static EventBus instance;
        return instance;
    }

    // Subscribe a free function or static member function
    void subscribe(const std::string& eventName, EventCallback callback) {
        subscribers[eventName].push_back(callback);
    }

    // Subscribe a non-static member function with an instance
    template <typename ObjectType>
    void subscribe(const std::string& eventName, void (ObjectType::*memberFunction)(), ObjectType* instance) {
        subscribers[eventName].push_back([=]() { (instance->*memberFunction)(); });
    }

    void emit(const std::string& eventName) {
        if (subscribers.find(eventName) != subscribers.end()) {
            for (auto& callback : subscribers[eventName]) {
                callback();
            }
        }
    }

private:
    std::map<std::string, std::vector<EventCallback>> subscribers;

    EventBus() = default;
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
};

// Helper function for free/static functions
inline void subscribe(const std::string& eventName, EventBus::EventCallback callback) {
    EventBus::getInstance().subscribe(eventName, callback);
}

// Overload helper function for member functions
template <typename ObjectType>
inline void subscribe(const std::string& eventName, void (ObjectType::*memberFunction)(), ObjectType* instance) {
    EventBus::getInstance().subscribe(eventName, memberFunction, instance);
}

// Helper function for emitting events
inline void emit(const std::string& eventName) {
    EventBus::getInstance().emit(eventName);
}

// Window procedure to handle key events
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_KEYDOWN:
            // Emit an event when a key is pressed
            emit(std::to_string(static_cast<int>(wParam)));
            if (DEBUG) {
                std::cout << "[D] Key " << static_cast<int>(wParam) << " pressed!" << std::endl;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Function to create a window and start the message loop
void CreateMessageLoop() {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("EventBusWindowClass"), NULL };
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, _T("EventBus Window"), WS_OVERLAPPEDWINDOW, 100, 100, 300, 200, NULL, NULL, wc.hInstance, NULL);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnregisterClass(wc.lpszClassName, wc.hInstance);
}

// Start the message loop in a separate thread
inline void StartMessageLoop() {
    std::thread messageLoopThread(CreateMessageLoop);
    messageLoopThread.detach();
}