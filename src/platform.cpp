#include "platform.hpp"

#include <array>
#include <stdexcept>
#include <win32/win32_window.hpp>
#include <opengl.hpp>

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
            static OpenGLRenderer renderer;

            window->init_opengl();

            renderer.init();

            return &renderer;
        }
        }
        throw std::underflow_error("invalid/unimplemented renderer type");
    }
};