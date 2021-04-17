#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")

#include <Windows.h>
#include <d3d11_1.h>
#include <DXGI.h>
#include <d2d1_1.h>
#include <sstream>
#include <dwrite_2.h>
#include <wrl.h>

#include "simulation.h"

using namespace Microsoft::WRL;

class Graphics3
{
private:
    class Helper
    {
    public:
        double angleDegreeToDecimal(double angleDeg);
        int round(int n);
    };

    Helper helper;

    HWND m_hwnd;
    Simulation* m_pSimulation;

    float m_Width;
    float m_Height;

    // rate at which the simulation will be drawn compared to the simulation size
    float m_simulationDisplayPercentageX;
    float m_simulationDisplayPercentageY;


    //Direct3D
    ComPtr<IDXGISwapChain> m_pDxgiSwapChain = nullptr;
    ComPtr<ID3D11Device> m_pDevice = nullptr;
    ComPtr<ID3D11DeviceContext> m_pContext = nullptr;

    ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
    ComPtr<ID3D11Texture2D> m_pBackBuffer = nullptr;

    ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer = nullptr;
    ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;


    //Direct2D
    ComPtr<ID2D1Factory> m_pFactory = nullptr;

    ComPtr<ID2D1RenderTarget> m_pDirect2dRenderTarget = nullptr;

    ComPtr<IDWriteFactory2> m_pWriteFactory;
    ComPtr<IDWriteTextFormat> m_pSergeoTextFormat;

    ComPtr<ID2D1SolidColorBrush> m_pBaseColor;
    ComPtr<ID2D1SolidColorBrush> m_pShadowColor;
    ComPtr<ID2D1SolidColorBrush> m_pBaseColorB;
    ComPtr<ID2D1SolidColorBrush> m_pShadowColorB;
    ComPtr<ID2D1SolidColorBrush> m_pBlackBrush;

    //Clear Color
    float black[4] = { 0, 0, 0, 1 };
    float m_swarmA[4] = { 1, 0.5, 0.5, 1 }; // Helles Rot
    float m_swarmB[4] = { 0.5, 0.5, 1, 1 }; // Helles Blau
    std::wostringstream agentNumberString;
    std::wstring agentNumberText;

    HRESULT CreateDirect2dRenderTarget();
    HRESULT CreateBrushes();
    void DrawAgents(std::vector<Agent> agents);
    void DrawAgent(int posX, int posY, float angleDeg, float size, bool swarmB, float overtakenPercentage, bool isShadow);

public:
    Graphics3(HWND hwnd, float width, float height, Simulation* pSimulation, float simulationDisplayPercentageX = 1, float simulationDisplayPercentageY = 1);
    void render();
};