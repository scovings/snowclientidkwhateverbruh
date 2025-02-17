#include "GUI.h"
#include "../modules/modules.h"
#include <Windows.h>

static bool is_init{};
static bool do_draw{true};
static bool waiting_for_key = false;
static int keybind_to_set = 0;
static int fastplace_key = 'G';
static int flight_key = 'F';
static bool fastplace_enabled = false;
static bool step_enabled = false;
static bool flight_enabled = false;
static bool eagle_enabled = false;
static bool sprint_enabled = false;
//fixed that shit
void BlockGameInput(bool block)
{
    ShowCursor(block);
    if (block)
    {
        ClipCursor(NULL);
        SetCapture(NULL);
    }
}

bool GUI::init(HWND wnd_handle)
{
    if (is_init)
        return false;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 10.0f;
    style.WindowRounding = 12.0f;
    style.GrabRounding = 6.0f;
    style.WindowPadding = ImVec2(16, 16);
    style.FramePadding = ImVec2(10, 6);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.28f, 0.35f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.38f, 0.45f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.48f, 0.55f, 1.0f);

    ImGui_ImplWin32_Init(wnd_handle);
    ImGui_ImplOpenGL3_Init();

    is_init = true;
    return false;
}

void GUI::shutdown()
{
    if (!is_init)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    is_init = false;
}


Fly* fly_mod = new Fly("Flyhack", 'F');
Fastplace* fastplace_mod = new Fastplace(std::string("Fastplace"), 'G');
Step* step_mod = new Step("Step", 'P');
Eagle* eagle_mod = new Eagle("Eagle", 'M');
void GUI::draw()
{

    flight_enabled = fly_mod->enabled;
    eagle_enabled = eagle_mod->enabled;
    fastplace_enabled = fastplace_mod->enabled;



    if (!do_draw)
        return;

    BlockGameInput(true);

    fastplace_mod->max_delay = GUI::newdelay;
    step_mod->height = GUI::newHeight;
    if (waiting_for_key) {
        for (int key = 1; key < 256; key++) {
            if (GetAsyncKeyState(key) & 0x8000) {
                if (keybind_to_set == 1)
                    fastplace_key = key;
                else if (keybind_to_set == 2)
                    flight_key = key;
                waiting_for_key = false;
                keybind_to_set = 0;
                break;
            }
        }
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("DOWNPOAR B0.1", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 1) {
            do_draw = false;
            BlockGameInput(false);
        }

        ImGui::Text("Welcome to DOWNPOAR!");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Mod Settings:");

        if (ImGui::Checkbox("Enable Fastplace", &fastplace_enabled)) {
            emit("71");
        }
        ImGui::SliderInt("Fastplace Delay", &GUI::newdelay, 1, 10, "Delay: %d");
        
        ImGui::SameLine();
        if (ImGui::Button(waiting_for_key && keybind_to_set == 1 ? "Press a key..." : (std::string("Key: ") + (char)fastplace_key).c_str())) {
            waiting_for_key = true;
            keybind_to_set = 1;
        }

        if (ImGui::Checkbox("Enable Flight", &flight_enabled)) {
            emit("96");
        }
        ImGui::SameLine();
        if (ImGui::Button(waiting_for_key && keybind_to_set == 2 ? "Press a key..." : (std::string("Key: ") + (char)flight_key).c_str())) {
            waiting_for_key = true;
            keybind_to_set = 2;
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if(ImGui::Checkbox("Enable Step", &step_enabled)){
            emit("80");
        }
        ImGui::SliderFloat("Step Height", &GUI::newHeight, 0.6f, 10.0f, "%.3f");

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if(ImGui::Checkbox("Enable Eagle", &eagle_enabled)){
            emit("77");
        }
        ImGui::SliderFloat("Down Amount", &GUI::downVec, 1.0, 5.0, "%.1f");


        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if(ImGui::Checkbox("Enable Sprint", &sprint_enabled)){
            emit("79");
           
        }


        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Color picker widget
        static ImVec4 color = ImVec4(0.2f, 0.3f, 0.8f, 1.0f);  // Default accent color
        ImGui::ColorEdit3("Accent Color", (float*)&color);

        // Apply the selected color to accent elements
        ImGuiStyle& style = ImGui::GetStyle();
        
        // Change only the accent colors (buttons, sliders, etc.)
        style.Colors[ImGuiCol_Button] = ImVec4(color.x, color.y, color.z, 1.0f);  // Button background
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(color.x * 1.1f, color.y * 1.1f, color.z * 1.1f, 1.0f); // Hovered button
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(color.x * 1.2f, color.y * 1.2f, color.z * 1.2f, 1.0f); // Active button
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(color.x, color.y, color.z, 1.0f); // Slider grab color
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(color.x * 1.1f, color.y * 1.1f, color.z * 1.1f, 1.0f); // Active slider color
        style.Colors[ImGuiCol_CheckMark] = ImVec4(color.x, color.y, color.z, 1.0f); // Checkbox checkmark color

        if (ImGui::Button("Apply Settings", ImVec2(150, 30))) {
            // Placeholder for additional actions
        }
    }
    ImGui::End();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


bool GUI::getIsInit()
{
    return is_init;
}

bool GUI::getDoDraw()
{
    return do_draw;
}

void GUI::setDoDraw(bool new_value)
{
    do_draw = new_value;
}
