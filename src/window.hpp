#pragma once

#include <functional>

namespace prosper
{
    enum class WindowEvent
    {
        Input
    };

    // Input event data

    enum Key
    {

    };

    enum KeyState
    {

    };

    using OnInput = std::function<void(Key, KeyState)>;

    // Window event context data union

    union WindowEventContext
    {
        struct InputEvent
        {
            Key key;
            KeyState keystate;
        } input;
    };

    // Called while handling window messages.

    using WindowEventHandler = std::function<void(WindowEvent, const WindowEventContext &)>;

    /// Window abstraction
    class Window
    {
    public:
        // Processes/flushes window messages, and calls appropiate callbacks
        virtual void process_messages() = 0;

        // Pops open the window
        virtual void show() = 0;

        // Swaps to back buffer
        virtual void swap_buffers() = 0;

        // Initialize opengl rendering context
        //? this method PROBABLY fits here. initializing a context require a whole lot of platform code
        //? which is dependant on the windows handle device context etc.
        virtual void init_opengl() = 0;
    };
}