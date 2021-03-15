// Windows Header Files:
#include "window.h"

#include <windows.h>
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



HWND m_hwnd;
HINSTANCE m_hInstance;
ID2D1Factory* m_pDirect2dFactory;
ID2D1HwndRenderTarget* m_pRenderTarget;
ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
ID2D1SolidColorBrush* m_pCornflowerBlueBrush;


// Initialize device-dependent resources.
HRESULT Window::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    if (!m_pRenderTarget)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
        );

        // Create a Direct2D render target.
        hr = m_pDirect2dFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
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
void Window::DiscardDeviceResources() {
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pLightSlateGrayBrush);
    SafeRelease(&m_pCornflowerBlueBrush);
}

// Draw content.
HRESULT Window::OnRender()
{
    HRESULT hr = S_OK;

    hr = CreateDeviceResources();

    if (SUCCEEDED(hr))
    {
        m_pRenderTarget->BeginDraw();

        m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

        D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

        //Draw Background
        //Draw Agents (Differentiate )


        int agentX;
        int agentY;

        D2D1_RECT_F rectangle2 = D2D1::RectF(
            rtSize.width / 2 - 100.0f,
            rtSize.height / 2 - 100.0f,
            rtSize.width / 2 + 100.0f,
            rtSize.height / 2 + 100.0f
        );

        //Draw a filled rectangle.
        m_pRenderTarget->FillRectangle(&rectangle2, m_pLightSlateGrayBrush);

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

// The windows procedure.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

        // Note that this tutorial does not handle resizing (WM_SIZE) requests,
        // so we created the window without the resize border.

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// Initialize device-independent resources.
HRESULT Window::CreateDeviceIndependentResources() {
    HRESULT hr = S_OK;

    // Create a Direct2D factory.
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

    return hr;
}

void Window::Initialize(HINSTANCE hInstance)
{
    HRESULT hr;

    // Initialize device-indpendent resources, such
    // as the Direct2D factory.
    hr = CreateDeviceIndependentResources();

    if (SUCCEEDED(hr))
    {
        // Register the window class.
        WNDCLASSEX wcex;
        ZeroMemory(&wcex, sizeof(WNDCLASSEX));
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hbrBackground = NULL;
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = L"D2DDemoApp";

        RegisterClassEx(&wcex);

        // Create the window. 
        RECT rc = { 0, 0, 800, 600 };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

        m_hwnd = CreateWindow(
            L"D2DDemoApp",
            L"Direct2D Demo App",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            rc.right - rc.left,
            rc.bottom - rc.top,
            NULL,
            NULL,
            hInstance,
            NULL
        );
        ShowWindow(m_hwnd, SW_SHOW);
    }
}

Window::~Window()
{
    SafeRelease(&m_pDirect2dFactory);
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pLightSlateGrayBrush);
    SafeRelease(&m_pCornflowerBlueBrush);
}

Window::Window()
{

}