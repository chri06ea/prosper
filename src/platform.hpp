#pragma once

/*
 * Platform independant code
 */

#include <functional>
#include <vector>
#include <algorithm>
#include <array>
#include <stdexcept>

// include before windows, since windows headers export stuff like this:
// #define near 
#include <math/math.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "window.hpp"
#include "renderer.hpp"
#include "sound.hpp"

#include <iostream>

#define LOG_INFORMATION(x) std::cout << x << std::endl;

#define CRITICAL_ERROR(msg) LOG_INFORMATION(msg)

namespace prosper
{
    // Platform independant method for creating a window

    struct WindowOptions
    {
    };

    Window *create_window(WindowEventHandler event_handler, WindowOptions options = {});

    // Platform independant method for creating a renderer.
    Renderer *create_renderer(Window *window, RendererType type = RendererType::OpenGL);

    // Platform independant method for creating a sound player
    Sound *create_sound();

    // Get the tick count according the machine the game is running on
    uint64_t get_platform_tick_count();

    // Amount of ticks per second
    uint64_t get_platform_ticks_per_second();

    
};
