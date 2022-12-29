#include "platform.hpp"

#include <stdexcept>
#include <win32/win32_window.hpp>

namespace prosper
{
    Window *create_window(WindowEventHandler event_handler, WindowOptions options)
    {
        return Win32Window::create();
    }

    Renderer *create_renderer(Window *window, RendererType type)
    {
        switch (type)
        {
        case RendererType::OpenGL:
        {
        }

        default:
        {
            throw std::underflow_error("invalid/unimplemented renderer type");
        }
        }
    }
};