#pragma once
#include <Windows.h>
#include "simulation.h"
#include <d2d1.h>
#include <d2d1helper.h>

#pragma comment(lib, "d2d1.lib")

class Graphics
{
private:
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;

	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources(HWND hwnd);
	void DiscardDeviceResources();

public:
	Graphics();
	~Graphics();
	HRESULT render(Simulation simulation, HWND hwnd);
};