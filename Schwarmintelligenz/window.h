#pragma once

#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include "simulation.h"


#pragma comment(lib, "d2d1.lib")

class Window
{
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	Window(HINSTANCE hInstance);
	~Window();
	HWND m_hwnd;
	unsigned int selectedAgent = 0;
	int numAgents = 0;
};