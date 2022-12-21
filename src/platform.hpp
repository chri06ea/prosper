#pragma once

#include "core.hpp"
#include "renderer.hpp"

namespace prosper
{
    namespace platform
    {

        /// @brief call when an unrecoverable error happens. will terminate the process
        /// @param message
        [[noreturn]] void emergency_shutdown(std::string_view message);

        /// @brief Initialize the platform layer
        /// @return true if successfully initialized
        bool initialize_platform();

        /// @brief process user input (keyboard, mouse, controller, whatever)
        void process_input();

        // switch to backbuffer
        void swap_buffers();
    }
}

//! NOTE This is to satisfy vscodes intellisense. Delete when more than 1 platform implemented
#ifndef WIN32
#define WIN32
#endif