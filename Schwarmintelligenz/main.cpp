#include "window.h"
#include <vector>
#include "simulation.h"
#include <random>
#include "graphics.h"

#define NUM_AGENTS 50

int WINAPI WinMain(HINSTANCE  hInstance, HINSTANCE  hPrevInstance, LPSTR  lpCmdLine, int  nCmdShow)
{
    if (SUCCEEDED(CoInitialize(NULL)))
    {
        Window window(hInstance);
        window.numAgents = NUM_AGENTS;
        Simulation simulation(NUM_AGENTS);
        Graphics graphics;

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
                simulation.selectedAgent = window.selectedAgent;
                graphics.render(simulation, window.m_hwnd);
            }
        }

        CoUninitialize();
    }
    return 0;
}