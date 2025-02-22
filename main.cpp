#include "main.h"
#include "functions/functions.h"
#include "modules/modules.h"






void client_main(HMODULE mod) {
    Fly* fly_mod = new Fly("Flyhack", 'F');
    Fastplace* fastplace_mod = new Fastplace(std::string("Fastplace"), 'G');
    Eagle* eagle_mod = new Eagle("Eagle", 'M');
    Velocity* velocity_mod = new Velocity("Velocity", 'I');
    //fly_mod->toggle();
    mods.push_back(velocity_mod);
    mods.push_back(fly_mod);
    mods.push_back(fastplace_mod);
    mods.push_back(eagle_mod);

    if (Hook::init())
        Hook::shutdown();


    // Game loop
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


        // Cleanup ImGui


    // Destroy the GLFW window and terminate GLFW


    jni::shutdown();
    Hook::imguishut();
    printf("[-] Client Shutdown Executed!\n");
    FreeConsole();
    FreeLibraryAndExitThread(mod, 0);
    return;
 
}

