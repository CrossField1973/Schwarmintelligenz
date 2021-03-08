#include "window.h"

int WINAPI wWinMain(HINSTANCE  hInstance, HINSTANCE  hPrevInstance, LPWSTR  lpCmdLine, int  nCmdShow)
{
    Window window;

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
                window.OnRender();
            }
        }

        CoUninitialize();
    }



    return 0;
}