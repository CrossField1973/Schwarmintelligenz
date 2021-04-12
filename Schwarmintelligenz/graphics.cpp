#include <Windows.h>
#include "graphics.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <sstream>
#include <math.h>

#define _USE_MATH_DEFINES

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
                D2D1::ColorF(D2D1::ColorF::Black),
                &m_pBlackBrush
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

double angleDegreeToDecimal(double angleDeg)
{
    return angleDeg * 3.14159265358979323846 / 180;
}

void Graphics::DrawAgent(int posX, int posY, float angleDeg, float size, ID2D1SolidColorBrush* color)
{
    /*D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(posX, posY), size / 2.0f, size / 2.0f);
    m_pRenderTarget->FillEllipse(&ellipse, color);*/


    //TEST
    D2D1_POINT_2F pt1 = { 1.7 * size * cos(angleDegreeToDecimal(angleDeg)) + posX, 1.7 * size * sin(angleDegreeToDecimal(angleDeg)) + posY };
    D2D1_POINT_2F pt2 = { size * cos(angleDegreeToDecimal(angleDeg + 120)) + posX, size * sin(angleDegreeToDecimal(angleDeg + 120)) + posY };
    D2D1_POINT_2F pt3 = { size * cos(angleDegreeToDecimal(angleDeg + 240)) + posX, size * sin(angleDegreeToDecimal(angleDeg + 240)) + posY };

    ID2D1GeometrySink* pSink = NULL;
    ID2D1PathGeometry* pPathGeometry = NULL;
    m_pDirect2dFactory->CreatePathGeometry(&pPathGeometry);
    pPathGeometry->Open(&pSink);

    pSink->BeginFigure(pt1, D2D1_FIGURE_BEGIN_FILLED);
    pSink->AddLine(pt2);
    pSink->AddLine(pt3);
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
    pSink->Close();

    m_pRenderTarget->FillGeometry(pPathGeometry, color);
    pSink->Release();
    pPathGeometry->Release();
    //TEST
}

void Graphics::DrawAgents(std::vector<Agent> agents, D2D1::ColorF baseColor)
{
    HRESULT hr = S_OK;
    ID2D1SolidColorBrush* pBaseColor;
    ID2D1SolidColorBrush* pShadowColor;

    ID2D1SolidColorBrush* pBaseColorB;
    ID2D1SolidColorBrush* pShadowColorB;

    // Swarm a color
    hr = m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(1.0f, 0.5f, 0.5f),
        &pBaseColor
    );

    hr = m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(1.0f - 20.0f / 255.0f, 0.5f - 20.0f / 255.0f, 0.5f - 20.0f / 255.0f),
        &pShadowColor
    );

    // Swarm b color
    hr = m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(0.5f, 0.5f, 1.0f),
        &pBaseColorB
    );

    hr = m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(0.5f - 20.0f / 255.0f, 0.5f - 20.0f / 255.0f, 1.0f - 20.0f / 255.0f),
        &pShadowColorB
    );

    int i = 0;
    for (Agent agent : agents)
    {
        if (!agent.swarmB) {
            //Draw Agent Shadow
            DrawAgent(agent.location.x + 1.0f, agent.location.y + 1.0f, agent.angle(agent.velocity), 10.0f, pShadowColor);

            //Draw Agent
            DrawAgent(agent.location.x, agent.location.y, agent.angle(agent.velocity), 10.0f, pBaseColor);
        }
        else {
            //Draw Agent Shadow
            DrawAgent(agent.location.x + 1.0f, agent.location.y + 1.0f, agent.angle(agent.velocity), 10.0f, pShadowColorB);

            //Draw Agent
            DrawAgent(agent.location.x, agent.location.y, agent.angle(agent.velocity), 10.0f, pBaseColorB);
        }      

        std::wostringstream agentNumberString;
        agentNumberString << i << std::endl;
        std::wstring agentNumberText = agentNumberString.str();

        m_pRenderTarget->DrawTextW(
            agentNumberText.c_str(),
            wcslen(agentNumberText.c_str()),
            m_pSergeoTextFormat,
            D2D1::RectF(agent.location.x, agent.location.y, 1200.0f, 720.0f),
            m_pBlackBrush);

        i++;
    }
}

int round(int n)
{
    // Smaller multiple
    int a = (n / 10) * 10;

    // Larger multiple
    int b = a + 10;

    // Return of closest of two
    return (n - a > b - n) ? b : a;
}

// Draw content.
HRESULT Graphics::render(Simulation simulation, HWND hwnd)
{
    HRESULT hr = S_OK;

    hr = CreateDeviceResources(hwnd);

    if (SUCCEEDED(hr))
    {
        //Preparation
        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
        D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

        //Draw Background

        DrawAgents(simulation.agents, D2D1::ColorF(200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f));
        

        //Draw Text
        std::wostringstream outFPSString;
        outFPSString.precision(6);
        outFPSString << "FPS: " << simulation.fps << std::endl;
        outFPSString << "Simulation Time: " << int(simulation.timer.getTotalTime()) << " Sekunden" << std::endl;
        outFPSString << "Agent_" << simulation.selectedAgent << " posX: " << round(int(simulation.agents[simulation.selectedAgent].location.x)) << " posY: " << round(int(simulation.agents[simulation.selectedAgent].location.y)) << std::endl;
        outFPSString << "Overtaken " << simulation.agents[simulation.selectedAgent].overtakenPercentage << " Percentage" << std::endl;
        std::wstring outFPSText = outFPSString.str();

        m_pRenderTarget->DrawTextW(
            outFPSText.c_str(),
            wcslen(outFPSText.c_str()),
            m_pSergeoTextFormat, 
            D2D1::RectF(0.0f, 0.0f, 1200.0f, 720.0f), 
            m_pBlackBrush);

        //End Draw
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
    
    // Create a DWrite factory.
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, _uuidof(IDWriteFactory2), reinterpret_cast<IUnknown**>(&m_pWriteFactory));

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
    SafeRelease(&m_pWriteFactory);
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pLightSlateGrayBrush);
    SafeRelease(&m_pCornflowerBlueBrush);
    SafeRelease(&m_pSergeoTextFormat);
}