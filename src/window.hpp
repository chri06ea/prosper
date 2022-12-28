#pragma once

#include "renderer.hpp"

namespace prosper
{
    // Window class.
    class Window
    {
    public:
        // initialize the window
        void init();

        // Setup opengl renderer, and return interface
        Renderer *create_opengl_renderer();
    };
}