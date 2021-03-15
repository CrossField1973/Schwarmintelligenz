#include "window.h"
#include <vector>
#include "simulation.h"
#include <random>
#include "graphics.h"



int WINAPI wWinMain(HINSTANCE  hInstance, HINSTANCE  hPrevInstance, LPWSTR  lpCmdLine, int  nCmdShow)
{
    //Time tick;
    if (SUCCEEDED(CoInitialize(NULL)))
    {
        Window window(hInstance);
        Simulation simulation(50);
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
                //update tick
                simulation.update();
                graphics.render(simulation, window.m_hwnd);
            }
        }

        CoUninitialize();
    }



    return 0;
}