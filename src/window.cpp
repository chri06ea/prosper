#include <stdexcept>
#include <Windows.h>
#ifdef WIN32
#endif

namespace prosper
{
    /*
     *   WIN32 Implementation of the window
     */

    // Win32 window callback
    LRESULT CALLBACK window_proc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        LRESULT result{};

        switch (msg)
        {
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            result = DefWindowProcA(window, msg, wparam, lparam);
            break;
        }

        return result;
    }

    HWND create_window_instance(std::string_view title)
    {
        // Setup and create game window class
        WNDCLASSEXA window_class{
            .cbSize = sizeof(WNDCLASSEXA),
            .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
            .lpfnWndProc = window_proc,
            .hInstance = GetModuleHandle(0),
            .hCursor = LoadCursor(0, IDC_ARROW),
            .lpszClassName = title.data()};

        if (!RegisterClassExA(&window_class))
            throw std::runtime_error("failed registering window class");

        // Create game window
        auto window_handle = CreateWindowExA(0, title.data(), title.data(),
                                             WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                             1200, 800, 0, 0, GetModuleHandle(0), 0);

        if (window_handle == NULL)
            throw std::runtime_error("failed creating window");

        return window_handle;
    }

    void init()
    {

        // ShowWindow(game_window_handle, 1);
    }
}