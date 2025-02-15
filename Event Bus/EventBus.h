
#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>
#include <thread>
#include "Vars.h"
class EventBus {
public:
    using EventCallback = std::function<void()>;

    static EventBus& getInstance() {
        static EventBus instance;
        return instance;
    }


void checkForKeyPress()
{
    std::map<int, std::string> keyMap;
    for (int i = 'A'; i <= 'Z'; ++i)
    {
        keyMap[i] = std::string(1, (char)i);
    }
    keyMap[VK_NUMPAD0] = "NUMPAD 0";
    keyMap[VK_NUMPAD1] = "NUMPAD 1";
    keyMap[VK_NUMPAD2] = "NUMPAD 2";
    keyMap[VK_NUMPAD3] = "NUMPAD 3";
    keyMap[VK_NUMPAD4] = "NUMPAD 4";
    keyMap[VK_NUMPAD5] = "NUMPAD 5";
    keyMap[VK_NUMPAD6] = "NUMPAD 6";
    keyMap[VK_NUMPAD7] = "NUMPAD 7";
    keyMap[VK_NUMPAD8] = "NUMPAD 8";
    keyMap[VK_NUMPAD9] = "NUMPAD 9";
    
        for (const auto& key : keyMap)
        {

            if (GetAsyncKeyState(key.first) & 1)
            {
                
                if(DEBUG){
                    std::cout << "[D] Key " + std::to_string(key.first) + "pressed!" << std::endl;
                }

                emit(std::to_string(key.first));
            }
        }

    
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
// helper for checking with the keys
inline void CheckForKeys(){
    EventBus::getInstance().checkForKeyPress();
}