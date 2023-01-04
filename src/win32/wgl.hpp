#pragma once

/*
 * Windows specific implementation for initializing rendering contexts
 */

namespace prosper
{
    void init_opengl_rendering_context_with_wgl(void* device_context);
}
