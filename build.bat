:: g++ -shared -o build/DOWNPOUR.dll  src/dllmain.cpp src/main.cpp src/functions/functions.cpp src/modules/modules.cpp -L"C:\Program Files\OpenLogic\jdk-8.0.432.06-hotspot\lib" -ljvm -lopengl32 -static


cls

g++ -shared -o build/DOWNPOUR.dll dllmain.cpp main.cpp functions/functions.cpp modules/modules.cpp guiLibs/GUI.cpp guiLibs/Hook.cpp guiLibs/include/ImGui/imgui_impl_opengl3.cpp guiLibs/include/ImGui/imgui_impl_win32.cpp guiLibs/include/ImGui/imgui.cpp guiLibs/include/ImGui/imgui_draw.cpp guiLibs/include/ImGui/imgui_widgets.cpp guiLibs/include/ImGui/imgui_tables.cpp -L"C:/Program Files/OpenLogic/jdk-8.0.432.06-hotspot/lib" -L"guiLibs/lib" -lMinHook -ljvm -ldwmapi -lopengl32 -lgdi32 -static

@echo off
echo BUILD SUCCESSFUL

cd jni
cd Injector
python3 app.py
timeout /t 5 /nobreak >nul

