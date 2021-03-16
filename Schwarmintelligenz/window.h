#pragma once

#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include "simulation.h"


#pragma comment(lib, "d2d1.lib")

class Window
{
public:
	Window(HINSTANCE hInstance);
	HWND m_hwnd;
};