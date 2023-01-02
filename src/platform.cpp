#include "platform.hpp"

#include <array>
#include <stdexcept>
#include <win32/win32_window.hpp>
#include <win32/win32_sound.hpp>
#include <opengl.hpp>

namespace prosper
{
    Window *create_window(WindowEventHandler event_handler, WindowOptions options)
    {
        return Win32Window::create(event_handler);
    }

    Renderer *create_renderer(Window *window, RendererType type)
    {
        switch (type)
        {
        case RendererType::OpenGL:
        {
            static OpenGLRenderer renderer;

            window->init_opengl();

            renderer.init();

            return &renderer;
        }
        }
        throw std::underflow_error("invalid/unimplemented renderer type");
    }

    Sound *create_sound()
    {
        return Win32Sound::create();
    }

    uint64_t get_platform_tick_count()
    {
        LARGE_INTEGER tick_count;

        if (!QueryPerformanceCounter(&tick_count))
            throw std::runtime_error("QueryPerformanceCounter call failed");

        return tick_count.QuadPart;
    }

    uint64_t get_platform_ticks_per_second()
    {
        LARGE_INTEGER frequency;

        if (!QueryPerformanceFrequency(&frequency))
            throw std::runtime_error("QueryPerformanceCounter call failed");

        return frequency.QuadPart;
    }
};