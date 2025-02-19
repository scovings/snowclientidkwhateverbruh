#include "../modules/modules.h"
#include <Windows.h>
#include <algorithm>
#include "consoleGUI.h"

namespace {
    // Color operation helpers
    ImVec4 operator*(const ImVec4& color, float factor) {
        return ImVec4(
            std::clamp(color.x * factor, 0.0f, 1.0f),
            std::clamp(color.y * factor, 0.0f, 1.0f),
            std::clamp(color.z * factor, 0.0f, 1.0f),
            color.w
        );
    }

    ImVec4 ColorMultiply(const ImVec4& a, const ImVec4& b) {
        return ImVec4(
            a.x * b.x,
            a.y * b.y,
            a.z * b.z,
            a.w * b.w
        );
    }
}

#define imgui ImGui



void GUI::BlockGameInput(bool block) {
    ShowCursor(block);
    if (block) {
        ClipCursor(nullptr);
        SetCapture(nullptr);
    }
}

bool GUI::init(HWND wnd_handle) {
    if (is_init) return false;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // Enhanced style settings
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    // Modern styling parameters
    style.WindowPadding = ImVec2(15, 15);
    style.FramePadding = ImVec2(12, 8);
    style.ItemSpacing = ImVec2(12, 10);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.IndentSpacing = 22.0f;

    style.WindowRounding = 12.0f;
    style.ChildRounding = 10.0f;
    style.FrameRounding = 8.0f;
    style.PopupRounding = 10.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding = 6.0f;
    style.TabRounding = 8.0f;

    style.WindowBorderSize = 1.5f;
    style.ChildBorderSize = 1.2f;
    style.PopupBorderSize = 1.2f;
    style.FrameBorderSize = 1.0f;

    // Base color scheme
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.11f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.99f, 1.00f);
    
    // Accent color system
    GUI::accent_color = ImVec4(0.38f, 0.61f, 0.89f, 1.00f);
    UpdateThemeColors();

    ImGui_ImplWin32_Init(wnd_handle);
    ImGui_ImplOpenGL3_Init();

    RedirectConsole();
    std::cout << "[+] Console Redirected to ImGui!\n";

    is_init = true;
    return true;
}

void GUI::shutdown() {
    if (!is_init) return;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    is_init = false;
}

Fly* fly_mod = new Fly("Flyhack", 'F');
Fastplace* fastplace_mod = new Fastplace(std::string("Fastplace"), 'G');
Eagle* eagle_mod = new Eagle("Eagle", 'M');
Velocity* velocity_mod = new Velocity("Velocity", 'I');

void GUI::draw() {

// Switched assignments
    fly_mod->enabled = flight_enabled;         // Now sets fly_mod->enabled to flight_enabled
    eagle_mod->enabled = eagle_enabled;        // Now sets eagle_mod->enabled to eagle_enabled
    fastplace_mod->enabled = fastplace_enabled; // Now sets fastplace_mod->enabled to fastplace_enabled
    velocity_mod->enabled = velo_enabled;      

    if (!do_draw) return;
    BlockGameInput(true);

    // Update module parameters
    fastplace_mod->max_delay = GUI::newdelay;
    velocity_mod->x = GUI::x1;
    velocity_mod->y = GUI::y1;
    velocity_mod->z = GUI::z1;
    velocity_mod->hurtTimeDelay = GUI::hurtTime;

    // Keybind handling
    if (waiting_for_key) {
        for (int key = 1; key < 256; key++) {
            if (GetAsyncKeyState(key) & 0x8000) {
                if (keybind_to_set == 1) fastplace_key = key;
                else if (keybind_to_set == 2) flight_key = key;
                waiting_for_key = false;
                keybind_to_set = 0;
                break;
            }
        }
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Main Window
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("DOWNPOUR v1 - _snow_x", nullptr, 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoResize
    );
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 1) {
            do_draw = false;
            BlockGameInput(false);
        }

        // Header Section
        ImGui::TextColored(accent_color, "DOWNPOUR v1");
        ImGui::Separator();
        
        // Tabs
        ImGui::BeginChild("Tabs", ImVec2(0, 40), true);
        {
            const ImVec2 button_size = ImVec2(ImGui::GetWindowWidth() * 0.2f, 30);
            
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));
            if (ImGui::Button("Movement", button_size)) GUI::tab = 0;
            ImGui::SameLine();
            if (ImGui::Button("Combat", button_size)) GUI::tab = 1;
            ImGui::SameLine();
            if (ImGui::Button("Visuals", button_size)) GUI::tab = 2;
            ImGui::SameLine();
            if (ImGui::Button("Settings", button_size)) GUI::tab = 3;
            ImGui::PopStyleVar();
        }
        ImGui::EndChild();

        // Content Area
        ImGui::BeginChild("Content", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
        {
            // Movement Tab
            if (GUI::tab == 0) {
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
                
                ImGui::BeginChild("Flight", ImVec2(0, 120), true);
                {
                    ImGui::Checkbox("Enable Flight", &flight_enabled);
                    ImGui::SameLine();
                    KeybindButton("Key##Flight", 2);
                    ImGui::SliderFloat("Speed", &GUI::x1, 0.1f, 5.0f, "%.1f");
                }
                ImGui::EndChild();

                ImGui::BeginChild("Velocity", ImVec2(0, 180), true);
                {
                    ImGui::Checkbox("Enable Velocity", &velo_enabled);
                    ImGui::SliderFloat("Hurt Time", &GUI::hurtTime, 0.0f, 1.0f, "%.1f");
                    ImGui::SliderFloat("X Axis", &GUI::x1, 0.0f, 2.0f, "%.1f");
                    ImGui::SliderFloat("Y Axis", &GUI::y1, 0.0f, 2.0f, "%.1f");
                    ImGui::SliderFloat("Z Axis", &GUI::z1, 0.0f, 2.0f, "%.1f");
                }
                ImGui::EndChild();

                ImGui::PopStyleVar();
            }

            // Combat Tab
            else if (GUI::tab == 1) {
                ImGui::BeginChild("Fastplace", ImVec2(0, 100), true);
                {
                    ImGui::Checkbox("Enable Fastplace", &fastplace_enabled);
                    ImGui::SameLine();
                    KeybindButton("Key##Fastplace", 1);
                    ImGui::SliderInt("Delay", &GUI::newdelay, 1, 10, "%dms");
                }
                ImGui::EndChild();

                ImGui::BeginChild("Eagle", ImVec2(0, 80), true);
                {
                    ImGui::Checkbox("Enable Eagle", &eagle_enabled);
                }
                ImGui::EndChild();
            }

            // Settings Tab
            else if (GUI::tab == 3) {
                ImGui::BeginChild("Theme", ImVec2(0, 100), true);
                {
                    ImGui::Text("Accent Color");
                    ImGui::SetNextItemWidth(150);
                    if (ImGui::ColorEdit3("##AccentColor", (float*)&accent_color, 
                        ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) {
                        UpdateThemeColors();
                    }
                    
                    ImGui::SameLine();
                    if (ImGui::Button("Reset Defaults", ImVec2(120, 0))) {
                        accent_color = ImVec4(0.38f, 0.61f, 0.89f, 1.00f);
                        UpdateThemeColors();
                    }
                }
                ImGui::EndChild();
            }
        }
        ImGui::EndChild();

        // Footer
        ImGui::Separator();
        ImGui::Text("Status: Active | FPS: %.1f", ImGui::GetIO().Framerate);
    }
    ImGui::End();

    ShowConsoleWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::KeybindButton(const char* label, int bind_id) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, accent_color * 0.8f);
    
    if (ImGui::Button(waiting_for_key && keybind_to_set == bind_id ? 
        "Press key..." : label, ImVec2(80, 0))){
        waiting_for_key = true;
        keybind_to_set = bind_id;
    }
    
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

void GUI::UpdateThemeColors() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Button colors
    style.Colors[ImGuiCol_Button] = accent_color * 0.8f;
    style.Colors[ImGuiCol_ButtonHovered] = accent_color * 1.1f;
    style.Colors[ImGuiCol_ButtonActive] = accent_color * 0.9f;
    
    // Slider colors
    style.Colors[ImGuiCol_SliderGrab] = accent_color;
    style.Colors[ImGuiCol_SliderGrabActive] = accent_color * 0.9f;
    
    // Checkmark colors
    style.Colors[ImGuiCol_CheckMark] = accent_color;
    
    // Header colors
    style.Colors[ImGuiCol_Header] = accent_color * 0.4f;
    style.Colors[ImGuiCol_HeaderHovered] = accent_color * 0.5f;
    style.Colors[ImGuiCol_HeaderActive] = accent_color * 0.6f;
    
    // Frame background
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
}

bool GUI::getIsInit() { return is_init; }
bool GUI::getDoDraw() { return do_draw; }
void GUI::setDoDraw(bool new_value) { do_draw = new_value; }