#pragma once

#include "core.hpp"

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

        /// @brief rendering interface
        class Renderer
        {
        public:
            virtual void draw_sprite() = 0;

            // switch to backbuffer
            virtual void swap() = 0;
        };

        /// @brief initializes and returns a rendering interface
        /// @return render interface ptr
        Renderer *create_renderer();
    }
}

//! NOTE This is to satisfy vscodes intellisense. Delete when more than 1 platform implemented
#ifndef WIN32
#define WIN32
#endif