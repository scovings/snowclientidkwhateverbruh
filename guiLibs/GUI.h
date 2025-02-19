#ifndef GUI_H_
#define GUI_H_

#include <cstdio>
#include <Windows.h>
#include "include/ImGui/imgui.h"
#include "include/ImGui/imgui_impl_win32.h"
#include "include/ImGui/imgui_impl_opengl3.h"

namespace GUI
{
    // Color theme variables
    static ImVec4 accent_color = ImVec4(0.38f, 0.61f, 0.89f, 1.00f);
    static ImVec4 background_color = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
    static ImVec4 element_bg = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);

    // Management variables
    static bool is_init{};
    static bool do_draw{true};
    static bool waiting_for_key = false;
    static int keybind_to_set = 0;
    
    // Module states
    static int fastplace_key = 'G';
    static int flight_key = 'F';
    static bool fastplace_enabled = false;
    static bool flight_enabled = false;
    static bool eagle_enabled = false;
    static bool sprint_enabled = false;
    static bool velo_enabled = false;

    // Configuration variables
    static int tab = 0;
    static float x1 = 0.4f;
    static float y1 = 0.4f;
    static float z1 = 0.4f;
    static float hurtTime = 0;
    static int newdelay = 1;

    // Core functions
    bool init(HWND wnd_handle);
    void shutdown();
    void draw();
    void UpdateThemeColors();

    // Helper functions
    void KeybindButton(const char* label, int bind_id);
    void BlockGameInput(bool block);

    // Accessors
    bool getIsInit();
    bool getDoDraw();
    void setDoDraw(bool new_value);
}

#endif