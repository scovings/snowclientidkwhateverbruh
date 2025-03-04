#include <Windows.h>
#include <cstdio>
FILE* pfile = nullptr;

#include "main.h"

JavaVM* vm;
JNIEnv* env = NULL;

void init(HMODULE mod) {
    // OPEN CONSOLE AND WRITE
    AllocConsole();
    freopen_s(&pfile, "CONOUT$", "w", stdout);
    printf("[+] DOWNPOUR INJECTED \n");
/*    // INIT JNI
    jsize vmCount;
    if (JNI_GetCreatedJavaVMs(&vm, 1, &vmCount) != JNI_OK || vmCount == 0) {
        printf("[!] ERROR: Java VM not found!\n");
        return;
    }
    jint res = vm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (res == JNI_EDETACHED) {
        res = vm->AttachCurrentThread((void**)&env, nullptr);
    }
    if (res != JNI_OK) {
        printf("[!] ERROR: Failed to set up JNI!\n");
        return;
    }
*/
    
    JavaVM* jvm = nullptr;
    JNI_GetCreatedJavaVMs(&jvm, 1, nullptr);
    JNIEnv* env = nullptr;
    jvm->AttachCurrentThread((void**)&env, nullptr);
    jni::init();

    jni::set_thread_env(env); //this is needed for every new thread that uses the lib


    if (env != nullptr) {
        printf("[+] JNI Hooked Successfully!\n");
        client_main(mod);
        std::cout <<  "quick test";
        
    }

  
}

BOOL APIENTRY DllMain(HMODULE mod, DWORD reason, LPVOID lpreserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init, mod, 0, 0);
            break;
        case DLL_PROCESS_DETACH:
          
            break;
    }
    return TRUE;
}