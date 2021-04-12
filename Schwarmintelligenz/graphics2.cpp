#include <Windows.h>
#include "graphics2.h"
#include <sstream>
#include <math.h>

double Graphics2::Helper::angleDegreeToDecimal(double angleDeg)
{
    return angleDeg * 3.14159265358979323846 / 180;
}

int Graphics2::Helper::round(int n)
{
    // Smaller multiple
    int a = (n / 10) * 10;

    // Larger multiple
    int b = a + 10;

    // Return of closest of two
    return (n - a > b - n) ? b : a;
}

Graphics2::Graphics2(HWND hwnd, Simulation* pSimulation)
{
    m_hwnd = hwnd;
    m_pSimulation = pSimulation;
    CreateDXGIRenderTarget();
    CreateBrushes();
}

HRESULT Graphics2::CreateDXGIRenderTarget() 
{
    HRESULT hr = S_OK;

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, m_pDirect2dFactory.GetAddressOf());

    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, _uuidof(IDWriteFactory2), reinterpret_cast<IUnknown**>(m_pWriteFactory.GetAddressOf()));

    hr = m_pWriteFactory->CreateTextFormat(
        L"Sergio UI",
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        20.0f,
        L"de-DE",
        &m_pSergeoTextFormat
    );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    D3D_FEATURE_LEVEL featureLevel;

    ID3D11Device* pDevice;
    ID3D11DeviceContext* pContext;

    hr = D3D11CreateDevice(
        m_pDxgiAdapter.Get(),
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &pDevice,
        &featureLevel,
        &pContext
    );

    hr = pDevice->QueryInterface(__uuidof(ID3D11Device1), (void**)&m_pDevice);
    pDevice->Release();

    hr = pContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&m_pContext);
    pContext->Release();

    hr = m_pDevice.As(&m_pDxgiDevice);

    hr = m_pDirect2dFactory->CreateDevice(m_pDxgiDevice.Get(), &m_pDirect2dDevice);

    hr = m_pDirect2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pDirect2dContext);

    // Allocate a descriptor.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
    swapChainDesc.Width = 0;                           // use automatic sizing
    swapChainDesc.Height = 0;
    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // this is the most common swapchain format
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1;                // don't use multi-sampling
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;                     // use double buffering to enable flip
    swapChainDesc.Scaling = DXGI_SCALING_NONE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // all apps must use this SwapEffect
    swapChainDesc.Flags = 0;

    hr = m_pDxgiDevice->GetAdapter(&m_pDxgiAdapter);

    hr = m_pDxgiAdapter->GetParent(IID_PPV_ARGS(&m_pDxgiFactory));

    hr = m_pDxgiFactory->CreateSwapChainForHwnd(
        m_pDevice.Get(),
        m_hwnd,
        &swapChainDesc,
        NULL,
        NULL,
        &m_pDxgiSwapChain
    );

    hr = m_pDxgiDevice->SetMaximumFrameLatency(1);

    hr = m_pDxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&m_pBackBuffer));

    D2D1_BITMAP_PROPERTIES1 bitmapProperties =
        D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
            0,
            0
        );

    hr = m_pDxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&m_pDxgiBackBuffer));

    hr = m_pDirect2dContext->CreateBitmapFromDxgiSurface(
        m_pDxgiBackBuffer.Get(),
        &bitmapProperties,
        &m_pDirect2dTargetBitmap
    );

    m_pDirect2dContext->SetTarget(m_pDirect2dTargetBitmap.Get());

    return hr;
}

HRESULT Graphics2::CreateBrushes()
{
    HRESULT hr = S_OK;

    hr = m_pDirect2dContext->CreateSolidColorBrush(
        D2D1::ColorF(1.0f, 0.5f, 0.5f),
        &m_pBaseColor
    );

    hr = m_pDirect2dContext->CreateSolidColorBrush(
        D2D1::ColorF(1.0f - 20.0f / 255.0f, 0.5f - 20.0f / 255.0f, 0.5f - 20.0f / 255.0f),
        &m_pShadowColor
    );

    hr = m_pDirect2dContext->CreateSolidColorBrush(
        D2D1::ColorF(0.5f, 0.5f, 1.0f),
        &m_pBaseColorB
    );

    hr = m_pDirect2dContext->CreateSolidColorBrush(
        D2D1::ColorF(0.5f - 20.0f / 255.0f, 0.5f - 20.0f / 255.0f, 1.0f - 20.0f / 255.0f),
        &m_pShadowColorB
    );

    hr = m_pDirect2dContext->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Black),
        &m_pBlackBrush
    );

    return hr;
}

// Draw content.
void Graphics2::render()
{
    //Preparation
    m_pDirect2dContext->BeginDraw();
    m_pDirect2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
    m_pDirect2dContext->Clear(D2D1::ColorF(D2D1::ColorF::White));
    D2D1_SIZE_F rtSize = m_pDirect2dContext->GetSize();

    //Draw Agents
    DrawAgents(m_pSimulation->agents);

    //Draw Statistics
    std::wostringstream outFPSString;
    outFPSString.precision(6);
    outFPSString << "FPS: " << m_pSimulation->fps << std::endl;
    outFPSString << "Simulation Time: " << int(m_pSimulation->timer.getTotalTime()) << " Sekunden" << std::endl;
    outFPSString << "Agent_" << m_pSimulation->selectedAgent << " posX: " << helper.round(int(m_pSimulation->agents[m_pSimulation->selectedAgent].location.x)) << " posY: " << helper.round(int(m_pSimulation->agents[m_pSimulation->selectedAgent].location.y)) << std::endl;
    std::wstring outFPSText = outFPSString.str();

    m_pDirect2dContext->DrawTextW(
        outFPSText.c_str(),
        wcslen(outFPSText.c_str()),
        m_pSergeoTextFormat.Get(),
        D2D1::RectF(0.0f, 0.0f, 1200.0f, 720.0f),
        m_pBlackBrush.Get()
    );

    //End Draw
    m_pDirect2dContext->EndDraw();
    m_pDxgiSwapChain->Present(1, 0);
}

void Graphics2::DrawAgents(std::vector<Agent> agents)
{
    int i = 0;

    for (Agent agent : agents)
    {
        // Draws agent with shadow and agentnumber
        if (!agent.swarmB) {
            //Draw Agent Shadow
            DrawAgent(agent.location.x + 1.0f, agent.location.y + 1.0f, agent.angle(agent.velocity), 10.0f, m_pShadowColor.Get());

            //Draw Agent
            DrawAgent(agent.location.x, agent.location.y, agent.angle(agent.velocity), 10.0f, m_pBaseColor.Get());
        }
        else {
            //Draw Agent Shadow
            DrawAgent(agent.location.x + 1.0f, agent.location.y + 1.0f, agent.angle(agent.velocity), 10.0f, m_pShadowColorB.Get());

            //Draw Agent
            DrawAgent(agent.location.x, agent.location.y, agent.angle(agent.velocity), 10.0f, m_pBaseColorB.Get());
        }

        std::wostringstream agentNumberString;
        agentNumberString << i << std::endl;
        std::wstring agentNumberText = agentNumberString.str();

        m_pDirect2dContext->DrawTextW(
            agentNumberText.c_str(),
            wcslen(agentNumberText.c_str()),
            m_pSergeoTextFormat.Get(),
            D2D1::RectF(agent.location.x, agent.location.y, 1200.0f, 720.0f),
            m_pBlackBrush.Get());

        i++;
    }
}

void Graphics2::DrawAgent(int posX, int posY, float angleDeg, float size, ID2D1SolidColorBrush* color)
{
    // Draw triangle agent with direction
    D2D1_POINT_2F pt1 = { 1.7 * size * cos(helper.angleDegreeToDecimal(angleDeg)) + posX, 1.7 * size * sin(helper.angleDegreeToDecimal(angleDeg)) + posY };
    D2D1_POINT_2F pt2 = { size * cos(helper.angleDegreeToDecimal(angleDeg + 120)) + posX, size * sin(helper.angleDegreeToDecimal(angleDeg + 120)) + posY };
    D2D1_POINT_2F pt3 = { size * cos(helper.angleDegreeToDecimal(angleDeg + 240)) + posX, size * sin(helper.angleDegreeToDecimal(angleDeg + 240)) + posY };

    ComPtr<ID2D1GeometrySink> pSink = NULL;
    ComPtr<ID2D1PathGeometry> pPathGeometry = NULL;
    m_pDirect2dFactory->CreatePathGeometry(&pPathGeometry);
    pPathGeometry->Open(&pSink);

    pSink->BeginFigure(pt1, D2D1_FIGURE_BEGIN_FILLED);
    pSink->AddLine(pt2);
    pSink->AddLine(pt3);
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
    pSink->Close();

    m_pDirect2dContext->FillGeometry(pPathGeometry.Get(), color);
}