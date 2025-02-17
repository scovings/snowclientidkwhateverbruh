#ifndef GUI_H_
#define GUI_H_

#include <cstdio>

#include <Windows.h>
#include "include/ImGui/imgui.h"
#include "include/ImGui/imgui_impl_win32.h"
#include "include/Imgui/imgui_impl_opengl3.h"

namespace GUI
{
	//
	// Management functions
	//
	static bool blatentMod = false;
	static float downVec = 1.5;
	static int newdelay = 1;
	static float newHeight = 0.6;


	static float x1 = 0.4;
	static float y1 = 0.4;
	static float z1 = 0.4;
	static float hurtTime = 0;

	bool init(HWND wnd_handle);
	void shutdown();

	//
	// Global functions
	//

	void draw();

	//
	// Get & Set functions
	//

	bool getIsInit();
	bool getDoDraw();

	void setDoDraw(bool new_value);
}

#endif