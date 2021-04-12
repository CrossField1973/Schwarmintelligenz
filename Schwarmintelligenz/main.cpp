#include "window.h"
#include <vector>
#include "simulation.h"
#include <random>
#include "graphics3.h"

#define NUM_AGENTS 100
#define WIDTH 1200
#define HEIGHT 700

int WINAPI WinMain(HINSTANCE  hInstance, HINSTANCE  hPrevInstance, LPSTR  lpCmdLine, int  nCmdShow)
{
    if (SUCCEEDED(CoInitialize(NULL)))
    {
        Window window(hInstance, WIDTH, HEIGHT, L"Schwarm Simulation");
        window.m_numAgents = NUM_AGENTS;
        Simulation simulation(NUM_AGENTS, WIDTH, HEIGHT);
        Graphics3 graphics(window.m_hwnd, WIDTH, HEIGHT, &simulation);

        MSG msg = { 0 };
        while (WM_QUIT != msg.message)
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                simulation.update();
                simulation.selectedAgent = window.m_selectedAgent;
                graphics.render();
            }
        }

        CoUninitialize();
    }
    return 0;
}