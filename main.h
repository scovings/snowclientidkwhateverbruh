#include <Windows.h>
#include <iostream>
#include "jni/jni.h"
#include "Event Bus/EventBus.h"
#include <GL/gl.h> // OpenGL header
#include "guiLibs/Hook.h"
#include "jni/meta_jni.hpp"


extern JavaVM* vm;
extern JNIEnv* env;





extern void client_main(HMODULE mod);