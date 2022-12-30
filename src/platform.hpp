#pragma once

/*
 * Platform independant code
 */

#include <functional>

#include "window.hpp"
#include "renderer.hpp"

#include <iostream>
#define LOG_INFORMATION(x) std::cout << x << std::endl;

namespace prosper
{
    // Platform independant method for creating a window

    struct WindowOptions
    {
    };

    Window *create_window(WindowEventHandler event_handler, WindowOptions options = {});

    // Platform independant method for creating a renderer.
    Renderer *create_renderer(Window *window, RendererType type = RendererType::OpenGL);
};
