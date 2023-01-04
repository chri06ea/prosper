#pragma once


/*
 * Platform independant methods
 */

#include <core/core.hpp>


#include "window.hpp"
#include "render_device.hpp"
#include "sound.hpp"

namespace prosper
{
	struct WindowOptions
	{
	};

	struct Platform
	{
		Window* create_window(WindowEventHandler event_handler, WindowOptions options = {});

		RenderDevice* get_render_device();

		SoundDevice* get_sound_device();

		uint64_t get_platform_tick_count();

		uint64_t get_platform_ticks_per_second();
	};
};
