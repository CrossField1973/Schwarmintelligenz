// Windows Header Files:
#include "window.h"

Window::Window(HINSTANCE hInstance, float width, float height, LPCWSTR windowName)
    : m_hwnd(NULL), m_width(0), m_height(0), m_selectedAgent(0), m_numAgents(0), m_speed(1), m_isPaused(false), m_pSimulation(NULL), m_pGraphics(NULL)
{
    m_width = width;
    m_height = height;

    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = HandleMsgSetup;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"SimulationWindow";

    RegisterClassEx(&wcex);

    // Create the window. 
    RECT rc = { 0, 0, width, height};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    m_hwnd = CreateWindow(
        L"SimulationWindow",
        windowName,
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rc.right - rc.left,
        rc.bottom - rc.top,
        NULL,
        NULL,
        hInstance,
        this
    );
    ShowWindow(m_hwnd, SW_SHOW);
}

Window::~Window()
{
    DestroyWindow(m_hwnd);
}

void Window::setSimulation(Simulation* simulation)
{
    m_pSimulation = simulation;
}

void Window::setGraphics(Graphics3* graphics)
{
    m_pGraphics = graphics;
}

//Win32 schenanigans to access member variables/functions from wndProc
//Creadits to ChiliTomatoNoodle
LRESULT Window::HandleMsgSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd) );

        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

        return pWnd->HandleMsg(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->HandleMsg(hWnd, message, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

    case WM_SIZE:
        // If it is minimized or maximized and graphics is set
        if ((wParam == 0 || wParam == 2) && m_pGraphics != NULL)
        {
            // Change drawing size          
            m_width = LOWORD(lParam);
            m_height = HIWORD(lParam);
            
            *m_pGraphics = Graphics3(m_hwnd, m_width, m_height, m_pSimulation, m_width / m_pSimulation->worldWidth, m_height / m_pSimulation->worldHeight);
        }
        break;

    case WM_KEYDOWN:
        if (wParam == VK_LEFT && m_selectedAgent > 0)
        {
            m_selectedAgent--;
        }
        else if (wParam == VK_RIGHT && m_selectedAgent < m_numAgents - 1)
        {
            m_selectedAgent++;
        }
        else if (wParam == 'P')
        {
            if (m_isPaused)
            {
                m_pSimulation->timer.start();
            }
            else
            {
                m_pSimulation->timer.stop();
            }
            m_isPaused = !m_isPaused;
        }
        else if (wParam == VK_UP && m_speed < 10)
        {
            m_speed += 0.5;
        }
        else if (wParam == VK_DOWN && m_speed > 0.5)
        {
            m_speed -= 0.5;
        }    
        else if (wParam == 'R')
        {
            if (m_pSimulation != NULL) 
            {
                Simulation simulation(m_numAgents, m_width, m_height, m_pSimulation->m_seed);
                *m_pSimulation = simulation;
                m_speed = 1;
                m_selectedAgent = 1;
            }           
        }
        else if (wParam == 'N')
        {
            if (m_pSimulation != NULL)
            {
                Simulation simulation(m_numAgents, m_width, m_height);
                *m_pSimulation = simulation;
                m_speed = 1;
                m_selectedAgent = 1;
            }
        }
        break;


    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
