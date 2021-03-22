#include <Windows.h>
#include "graphics.h"
#include <d2d1.h>
#include <d2d1helper.h>

#pragma comment(lib, "d2d1.lib")

template<class Interface> inline void SafeRelease(Interface** ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}

// Initialize device-dependent resources.
HRESULT Graphics::CreateDeviceResources(HWND hwnd)
{
    HRESULT hr = S_OK;

    if (!m_pRenderTarget)
    {
        RECT rc;
        GetClientRect(hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
        );

        // Create a Direct2D render target.
        hr = m_pDirect2dFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, size),
            &m_pRenderTarget
        );


        if (SUCCEEDED(hr))
        {
            // Create a gray brush.
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::LightSlateGray),
                &m_pLightSlateGrayBrush
            );
        }
        if (SUCCEEDED(hr))
        {
            // Create a blue brush.
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
                &m_pCornflowerBlueBrush
            );
        }
        if (SUCCEEDED(hr))
        {
            // Create a blue brush.
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Gray),
                &m_pGrayBrush
            );
        }
    }

    return hr;
}

// Release device-dependent resource.
void Graphics::DiscardDeviceResources() 
{
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pLightSlateGrayBrush);
    SafeRelease(&m_pCornflowerBlueBrush);
}

void Graphics::DrawAgent(int posX, int posY, float size, ID2D1SolidColorBrush* color)
{
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(posX, posY), size / 2.0f, size / 2.0f);
    m_pRenderTarget->FillEllipse(&ellipse, color);
}

void Graphics::DrawAgents(std::vector<Agent> agents, D2D1::ColorF baseColor)
{
    HRESULT hr = S_OK;
    ID2D1SolidColorBrush* pBaseColor;
    ID2D1SolidColorBrush* pShadowColor;

    hr = m_pRenderTarget->CreateSolidColorBrush(
        baseColor,
        &pBaseColor
    );

    hr = m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(baseColor.r - 20.0f / 255.0f, baseColor.g - 20.0f / 255.0f, baseColor.b - 20.0f / 255.0f),
        &pShadowColor
    );

    for (Agent agent : agents)
    {
        //Draw Agent Shadow
        DrawAgent(agent.location.x + 5.0f, agent.location.y + 5.0f, 50.0f, pShadowColor);

        //Draw Agent
        DrawAgent(agent.location.x, agent.location.y, 50.0f, pBaseColor);
    }
}

// Draw content.
HRESULT Graphics::render(Simulation simulation, HWND hwnd)
{
    HRESULT hr = S_OK;

    hr = CreateDeviceResources(hwnd);

    if (SUCCEEDED(hr))
    {
        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
        D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

        //Draw Background

        DrawAgents(simulation.agents, D2D1::ColorF(200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f));

        hr = m_pRenderTarget->EndDraw();
    }

    if (hr == D2DERR_RECREATE_TARGET)
    {
        hr = S_OK;
        DiscardDeviceResources();
    }

    return hr;
}

// Initialize device-independent resources.
HRESULT Graphics::CreateDeviceIndependentResources() {
    HRESULT hr = S_OK;

    // Create a Direct2D factory.
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_pDirect2dFactory);

    return hr;
}

Graphics::Graphics()
{
    HRESULT hr;
    hr = CreateDeviceIndependentResources();
}

Graphics::~Graphics()
{
    SafeRelease(&m_pDirect2dFactory);
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pLightSlateGrayBrush);
    SafeRelease(&m_pCornflowerBlueBrush);
}