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
    }

    return hr;
}

// Release device-dependent resource.
void Graphics::DiscardDeviceResources() {
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pLightSlateGrayBrush);
    SafeRelease(&m_pCornflowerBlueBrush);
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
        //Draw Agents (Differentiate )

        for (Agent agent : simulation.agents)
        {
            D2D1_RECT_F rectangle2 = D2D1::RectF(
                agent.getPosX() - 50.0f,
                agent.getPosY() - 50.0f,
                agent.getPosX() + 50.0f,
                agent.getPosY() + 50.0f
            );
            m_pRenderTarget->FillRectangle(&rectangle2, m_pLightSlateGrayBrush);
        }

        //// Draw a grid background.
        //int width = static_cast<int>(rtSize.width);
        //int height = static_cast<int>(rtSize.height);

        ////Draw UI

        //for (int x = 0; x < width; x += 10)
        //{
        //    m_pRenderTarget->DrawLine(
        //        D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
        //        D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
        //        m_pLightSlateGrayBrush,
        //        0.5f
        //    );
        //}

        //for (int y = 0; y < height; y += 10)
        //{
        //    m_pRenderTarget->DrawLine(
        //        D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
        //        D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
        //        m_pLightSlateGrayBrush,
        //        0.5f
        //    );
        //}

        //// Draw two rectangles.
        //D2D1_RECT_F rectangle1 = D2D1::RectF(
        //    rtSize.width / 2 - 50.0f,
        //    rtSize.height / 2 - 50.0f,
        //    rtSize.width / 2 + 50.0f,
        //    rtSize.height / 2 + 50.0f
        //);

        //D2D1_RECT_F rectangle2 = D2D1::RectF(
        //    rtSize.width / 2 - 100.0f,
        //    rtSize.height / 2 - 100.0f,
        //    rtSize.width / 2 + 100.0f,
        //    rtSize.height / 2 + 100.0f
        //);

        //// Draw a filled rectangle.
        //m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);

        //// Draw the outline of a rectangle.
        //m_pRenderTarget->DrawRectangle(&rectangle2, m_pCornflowerBlueBrush);

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
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

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