#pragma once

/*
 * Platform independant code
 */

#include <functional>
#include <vector>
#include <algorithm>
#include <array>
#include <stdexcept>

#include "window.hpp"
#include "renderer.hpp"
#include "sound.hpp"

#include <iostream>
#define LOG_INFORMATION(x) std::cout << x << std::endl;

#include <Windows.h>
#define CRITICAL_ERROR(msg)\
{\
    MessageBoxA(NULL,msg,"Critical error", MB_OK);\
}\

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
