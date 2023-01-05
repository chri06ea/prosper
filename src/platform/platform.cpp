#include "platform.hpp"

#include <array>
#include <stdexcept>
#include <win32/win32_window.hpp>
#include <win32/win32_sound.hpp>

#include <opengl/opengl.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../thirdparty/stb/stb_image.h"

namespace prosper
{
	Window* Platform::create_window(WindowEventHandler event_handler, WindowOptions options)
	{
		#ifdef WIN32
		return Win32Window::create(event_handler);
		#endif
	}

	RenderDevice* Platform::get_render_device()
	{
		static OpenGL opengl;
		return &opengl;
	}

	SoundDevice* Platform::get_sound_device()
	{
		return nullptr;
	}

	uint64_t Platform::get_platform_tick_count()
	{
		#ifdef WIN32
		LARGE_INTEGER tick_count;

		if(!QueryPerformanceCounter(&tick_count))
			CRITICAL_ERROR("QueryPerformanceCounter call failed");

		return tick_count.QuadPart;
		#endif
	}

	uint64_t Platform::get_platform_ticks_per_second()
	{
		#ifdef WIN32
		LARGE_INTEGER frequency;

		if(!QueryPerformanceFrequency(&frequency))
			CRITICAL_ERROR("QueryPerformanceCounter call failed");

		return frequency.QuadPart;
		#endif
	}
	Allocator* Platform::get_allocator()
	{
		static Allocator allocator;
		return &allocator;
	}
	unsigned char* Platform::load_image(const char* path, int& width, int& height, int& num_channels)
	{
		return stbi_load(path, &width,&height,&num_channels, 0);
	}
};