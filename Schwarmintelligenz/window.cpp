// Windows Header Files:
#include "window.h"

Window::Window(HINSTANCE hInstance, float width, float height, LPCWSTR windowName) 
    : m_hwnd(NULL), m_width(0), m_height(0), m_selectedAgent(0), m_numAgents(0), m_speed(1), m_isPaused(false)
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
        WS_OVERLAPPEDWINDOW,
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
        break;

        // Note that this tutorial does not handle resizing (WM_SIZE) requests,
        // so we created the window without the resize border.  

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
