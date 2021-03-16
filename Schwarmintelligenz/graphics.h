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
	ID2D1SolidColorBrush* m_pGrayBrush;

	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources(HWND hwnd);
	void DiscardDeviceResources();
	void DrawAgents(std::vector<Agent> agents, D2D1::ColorF baseColor);
	void DrawAgent(int x, int y, float size, ID2D1SolidColorBrush* color);

public:
	Graphics();
	~Graphics();
	HRESULT render(Simulation simulation, HWND hwnd);
};