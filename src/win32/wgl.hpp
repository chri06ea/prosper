#pragma once

/*
 * Windows specific implementation for initializing rendering contexts
 */

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace prosper
{
    void init_opengl_rendering_context_with_wgl(HDC device_context);
}
