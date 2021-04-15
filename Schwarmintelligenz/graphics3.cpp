#include "graphics3.h"

double Graphics3::Helper::angleDegreeToDecimal(double angleDeg)
{
    return angleDeg * 3.14159265358979323846 / 180;
}

int Graphics3::Helper::round(int n)
{
    // Smaller multiple
    int a = (n / 10) * 10;

    // Larger multiple
    int b = a + 10;

    // Return of closest of two
    return (n - a > b - n) ? b : a;
}

HRESULT Graphics3::CreateDirect2dRenderTarget()
{
    HRESULT hr = S_OK;

    // Create Device and Swapchain
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
    DXGI_SWAP_CHAIN_DESC scd;

    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.Width = m_Width;
    scd.BufferDesc.Height = m_Height;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = m_hwnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Windowed = true;
    scd.Flags = 0;

    hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, 1, D3D11_SDK_VERSION, &scd, &m_pDxgiSwapChain, &m_pDevice, &featureLevel, &m_pContext);


    // Create RenderTargetView
    hr = m_pDxgiSwapChain->GetBuffer(
        0, 
        __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(m_pBackBuffer.Get())
    );
    hr = m_pDevice->CreateRenderTargetView(m_pBackBuffer.Get(), 0, &m_pRenderTargetView);

    // Create DepthStencilView
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = m_Width;
    depthStencilDesc.Height = m_Height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    hr = m_pDevice->CreateTexture2D(&depthStencilDesc, 0, m_pDepthStencilBuffer.GetAddressOf());
    hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), 0, m_pDepthStencilView.GetAddressOf());
    m_pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

    // Create Factory
    D2D1_FACTORY_OPTIONS options;
    options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, options, m_pFactory.GetAddressOf());

    // set up the D2D render target using the back buffer
    ComPtr<IDXGISurface> dxgiBackbuffer;
    hr = m_pDxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(dxgiBackbuffer.GetAddressOf()));
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
    );
    hr = m_pFactory->CreateDxgiSurfaceRenderTarget(dxgiBackbuffer.Get(), props, m_pDirect2dRenderTarget.GetAddressOf());
    dxgiBackbuffer->Release();

    // Create DWrite factory
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(m_pWriteFactory.Get()), (IUnknown**)(m_pWriteFactory.GetAddressOf()));

    // Create DWrite text format
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

    // Create ViewPort
    D3D11_VIEWPORT vp;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Width = m_Width;
    vp.Height = m_Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_pContext->RSSetViewports(1, &vp);

    return hr;
}

HRESULT Graphics3::CreateBrushes()
{
    HRESULT hr = S_OK;

    hr = m_pDirect2dRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(1.0f, 0.5f, 0.5f),
        &m_pBaseColor
    );

    hr = m_pDirect2dRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(1.0f - 20.0f / 255.0f, 0.5f - 20.0f / 255.0f, 0.5f - 20.0f / 255.0f),
        &m_pShadowColor
    );

    hr = m_pDirect2dRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(0.5f, 0.5f, 1.0f),
        &m_pBaseColorB
    );

    hr = m_pDirect2dRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(0.5f - 20.0f / 255.0f, 0.5f - 20.0f / 255.0f, 1.0f - 20.0f / 255.0f),
        &m_pShadowColorB
    );

    hr = m_pDirect2dRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Black),
        &m_pBlackBrush
    );

    return hr;
}

void Graphics3::DrawAgents(std::vector<Agent> agents)
{
    int i = 0;

    for (Agent agent : agents)
    {
        // Draw Agent
        //Draw Agent Shadow
        DrawAgent(agent.location.x + 2.0f, agent.location.y + 2.0f, agent.angle(agent.velocity), 15.0f, agent.swarmB, agent.overtakenPercentage, true);

        //Draw Agent
        DrawAgent(agent.location.x, agent.location.y, agent.angle(agent.velocity), 15.0f, agent.swarmB, agent.overtakenPercentage, false);


        // Draw number
        /*agentNumberString.seekp(0);
        if (agent.swarmB) {
            agentNumberString << "B";
        }
        else
        {
            agentNumberString << "A";
        }
        agentNumberString << i << std::endl;
        agentNumberText.empty();
        agentNumberText = agentNumberString.str();

        m_pDirect2dRenderTarget->DrawTextW(
            agentNumberText.c_str(),
            wcslen(agentNumberText.c_str()),
            m_pSergeoTextFormat.Get(),
            D2D1::RectF(agent.location.x, agent.location.y, 1200.0f, 720.0f),
            m_pBlackBrush.Get()
        );

        i++;*/
    }
}

void Graphics3::DrawAgent(int posX, int posY, float angleDeg, float size, bool swarmB, float overtakenPercentage, bool isShadow)
{
    // Draw triangle agent with direction
    D2D1_POINT_2F pt1 = { 1.7 * size * cos(helper.angleDegreeToDecimal(angleDeg)) + posX, 1.7 * size * sin(helper.angleDegreeToDecimal(angleDeg)) + posY };
    D2D1_POINT_2F pt2 = { size * cos(helper.angleDegreeToDecimal(angleDeg + 120)) + posX, size * sin(helper.angleDegreeToDecimal(angleDeg + 120)) + posY };
    D2D1_POINT_2F pt3 = { size * cos(helper.angleDegreeToDecimal(angleDeg + 240)) + posX, size * sin(helper.angleDegreeToDecimal(angleDeg + 240)) + posY };

    ComPtr<ID2D1GeometrySink> pSink = NULL;
    ComPtr<ID2D1PathGeometry> pPathGeometry = NULL;
    m_pFactory->CreatePathGeometry(&pPathGeometry);
    pPathGeometry->Open(&pSink);

    pSink->BeginFigure(pt1, D2D1_FIGURE_BEGIN_FILLED);
    pSink->AddLine(pt2);
    pSink->AddLine(pt3);
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
    pSink->Close();

    ComPtr<ID2D1SolidColorBrush> colorBrush;
    float color[4];
    color[3] = 1;

    if (swarmB)
    {
        color[0] = m_swarmB[0] + 0.25 * overtakenPercentage;
        color[1] = m_swarmB[1];
        color[2] = m_swarmB[2];
    }
    else 
    {
        color[0] = m_swarmA[0];
        color[1] = m_swarmA[1];
        color[2] = m_swarmA[2] + 0.25 * overtakenPercentage;
    }
    if (isShadow)
    {
        color[0] = color[0] - 0.2;
        color[1] = color[1] - 0.2;
        color[2] = color[2] - 0.2;
    }

    m_pDirect2dRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(color[0], color[1], color[2], color[3]),
        &colorBrush
    );
    
    m_pDirect2dRenderTarget->FillGeometry(pPathGeometry.Get(), colorBrush.Get());
}

void Graphics3::render()
{
    //Preparation
    m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);
    m_pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_pDirect2dRenderTarget->BeginDraw();
    m_pDirect2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    m_pDirect2dRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
    D2D1_SIZE_F rtSize = m_pDirect2dRenderTarget->GetSize();

    //Draw Agents
    DrawAgents(m_pSimulation->agents);

    //Draw Statistics
    std::wostringstream outFPSString;
    outFPSString.precision(6);
    outFPSString << "FPS: " << m_pSimulation->fps << std::endl;
    outFPSString << "Simulation Time: " << int(m_pSimulation->timer.getTotalTime()) << " Sekunden" << std::endl;
    outFPSString << "Simulation Speed: " << m_pSimulation->speed << "x" << std::endl;
    outFPSString << "Agent_";
    if(m_pSimulation->agents[m_pSimulation->selectedAgent].swarmB) 
    {
        outFPSString << "B" << m_pSimulation->selectedAgent;
    }
    else
    {
        outFPSString << "A" << m_pSimulation->selectedAgent;
    }

    outFPSString << " posX: " << helper.round(int(m_pSimulation->agents[m_pSimulation->selectedAgent].location.x)) << " posY: " << helper.round(int(m_pSimulation->agents[m_pSimulation->selectedAgent].location.y)) << std::endl;
    std::wstring outFPSText = outFPSString.str();

    m_pDirect2dRenderTarget->DrawTextW(
        outFPSText.c_str(),
        wcslen(outFPSText.c_str()),
        m_pSergeoTextFormat.Get(),
        D2D1::RectF(0.0f, 0.0f, m_Width, m_Height),
        m_pBlackBrush.Get()
    );

    //End Draw
    m_pDirect2dRenderTarget->EndDraw();
    m_pDxgiSwapChain->Present(0, 0);
}

Graphics3::Graphics3(HWND hwnd, float width, float height, Simulation* pSimulation)
{
	m_hwnd = hwnd;
    m_Width = width;
    m_Height = height;
	m_pSimulation = pSimulation;
	CreateDirect2dRenderTarget();
	CreateBrushes();
}