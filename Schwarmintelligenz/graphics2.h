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

class Graphics2
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

    ComPtr<IDXGIAdapter> m_pDxgiAdapter;
    ComPtr<IDXGIDevice1> m_pDxgiDevice;
    ComPtr<IDXGIFactory2> m_pDxgiFactory;
    ComPtr<IDXGISwapChain1> m_pDxgiSwapChain;
    ComPtr<IDXGISurface> m_pDxgiBackBuffer;

    ComPtr<ID3D11Device1> m_pDevice;
    ComPtr<ID3D11DeviceContext1> m_pContext;
    ComPtr<ID3D11Texture2D> m_pBackBuffer;
    
    ComPtr<ID2D1Factory1> m_pDirect2dFactory;
    ComPtr<ID2D1Device> m_pDirect2dDevice;
    ComPtr<ID2D1DeviceContext> m_pDirect2dContext;
    ComPtr<ID2D1Bitmap1> m_pDirect2dTargetBitmap;

    ComPtr<IDWriteFactory2> m_pWriteFactory;
    ComPtr<IDWriteTextFormat> m_pSergeoTextFormat;
    
    ComPtr<ID2D1SolidColorBrush> m_pBaseColor;
    ComPtr<ID2D1SolidColorBrush> m_pShadowColor;
    ComPtr<ID2D1SolidColorBrush> m_pBaseColorB;
    ComPtr<ID2D1SolidColorBrush> m_pShadowColorB;
    ComPtr<ID2D1SolidColorBrush> m_pBlackBrush;

    HRESULT CreateDXGIRenderTarget();
    HRESULT CreateBrushes();
    void DrawAgents(std::vector<Agent> agents);
    void DrawAgent(int posX, int posY, float angleDeg, float size, ID2D1SolidColorBrush* color);

public:
    Graphics2(HWND hwnd, Simulation* pSimulation);
    void render();
};