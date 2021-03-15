#include "window.h"
#include <vector>
#include "simulation.h"
#include <random>



int WINAPI wWinMain(HINSTANCE  hInstance, HINSTANCE  hPrevInstance, LPWSTR  lpCmdLine, int  nCmdShow)
{
    //Time tick;

    Window window;
    Simulation simulation;
    

    if (SUCCEEDED(CoInitialize(NULL)))
    {
        window.Initialize(hInstance);
        
        

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
                //do logic
                window.OnRender(simulation);
            }
        }

        CoUninitialize();
    }



    return 0;
}