#pragma once

#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>

#pragma comment(lib, "d2d1.lib")

class Window
{
public:
	Window();
	~Window();
	void Initialize(HINSTANCE hInstance);
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	HRESULT OnRender();
	HRESULT CreateDeviceIndependentResources();
};