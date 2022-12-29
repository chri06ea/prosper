#include "win32_window.hpp"

#include <stdexcept>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "wgl.hpp"

namespace prosper
{
    namespace
    {
        struct WindowContext
        {
            HWND window_handle;
            HDC device_context;
        };

        std::unordered_map<void *, WindowContext> window_contexts;

        // Win32 window callback
        LRESULT CALLBACK window_proc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
        {
            LRESULT result{};

            switch (msg)
            {
            default:
                result = DefWindowProcA(window, msg, wparam, lparam);
                break;
            }

            return result;
        }
    }

    Win32Window::Win32Window()
    {
    }

    Win32Window *Win32Window::create()
    {
        const auto title = "hello";

        // Setup and create game window class
        WNDCLASSEXA window_class{
            .cbSize = sizeof(WNDCLASSEXA),
            .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
            .lpfnWndProc = window_proc,
            .hInstance = GetModuleHandle(0),
            .hCursor = LoadCursor(0, IDC_ARROW),
            .lpszClassName = title};

        if (!RegisterClassExA(&window_class))
            throw std::runtime_error("failed registering window class");

        // Create game window
        auto window_handle = CreateWindowExA(0, title, title,
                                             WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                             1200, 800, 0, 0, GetModuleHandle(0), 0);

        if (window_handle == NULL)
            throw std::runtime_error("failed creating window");

        // Show it

        // Acquire device context
        auto device_context = GetDC(window_handle);

        // Store context
        window_contexts[window_handle] = {
            .window_handle = window_handle,
            .device_context = device_context};

        auto window = new Win32Window();

        window->_context_handle = window_handle;

        return window;
    }

    void Win32Window::destroy(Win32Window *window)
    {
        throw std::underflow_error("win32window::destroy missing implementation");
    }

    void Win32Window::process_messages()
    {
        MSG msg = {};
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void Win32Window::show()
    {
        ShowWindow(window_contexts[_context_handle].window_handle, SW_NORMAL);
    }

    void Win32Window::swap_buffers()
    {
        // Not too sure about this.
        // On OpenGL this call is required (it's a winapi call. kinda same thing on mac and android)
        // On Vulkan this call is exposed thru the renderer..
        if (!SwapBuffers(window_contexts[_context_handle].device_context))
            throw std::runtime_error("failed SwapBuffers call");
    }

    void Win32Window::init_opengl()
    {
        init_opengl_rendering_context_with_wgl(window_contexts[_context_handle].device_context);
    }
}