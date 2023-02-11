#include "win32_platform.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace lib
{
	uint64_t Win32Platform::get_tick_count()
	{
		LARGE_INTEGER tick_count;

		if(!QueryPerformanceCounter(&tick_count))
			CRITICAL_ERROR("QueryPerformanceCounter call failed");

		return tick_count.QuadPart;
	}

	uint64_t Win32Platform::get_ticks_per_second()
	{
		LARGE_INTEGER frequency;

		if(!QueryPerformanceFrequency(&frequency))
			CRITICAL_ERROR("QueryPerformanceCounter call failed");

		return frequency.QuadPart;
	}
}