#include "platform.hpp"

namespace prosper
{
    namespace platform
    {
#ifdef WIN32
        /// @brief call when an unrecoverable error happens. will terminate the process
        /// @param message
        [[noreturn]] void emergency_shutdown(std::string_view message)
        {
            MessageBoxA(NULL, message.data(), "Emergency shutdown", MB_OK);

            exit(-1);
        }
        void swap_buffers()
        {
            // SwapBuffers(game_dc);
        }
#endif
    }
}