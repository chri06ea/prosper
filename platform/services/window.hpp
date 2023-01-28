#pragma once

#include <platform/core/defs.hpp>

#include <platform/models/window_event.hpp>

namespace lib
{
	class IWindow
	{
	public:

		virtual void swap_buffers() = 0;

		virtual void process_messages() = 0;

		virtual void get_size(size_t& width, size_t& height) = 0;

		virtual void get_position(size_t& width, size_t& height) = 0;

		virtual void init_graphics() = 0;
	};

	using WindowEventHandler = Function<void(const WindowEvent&)>;

	DECLARE_ABSTRACT_FACTORY(IWindow, WindowEventHandler handler)
}