#pragma once

#include <windows.h>
#include "simulation.h"


class Window
{
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	Window(HINSTANCE hInstance, float width, float height, LPCWSTR windowName);
	~Window();
	HWND m_hwnd;
	float m_width;
	float m_height;
	float m_speed;
	unsigned int m_selectedAgent;
	int m_numAgents;
};