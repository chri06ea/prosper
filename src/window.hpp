#pragma once

#include <functional>

namespace prosper
{
	enum class WindowEventType
	{
		Input,  // Some button was pressed or released
		Resize, // Window resized
	};

	// Input event data

	enum class Key
	{

	};

	enum class KeyState
	{

	};

	using OnInput = std::function<void(Key, KeyState)>;

	// Window event context data union
	
	union InputEvent
	{
		enum class Type
		{
			Key,
		} type;

		union
		{
			struct
			{
				Key key;
				KeyState keystate;
			} key_event;
		} data;
	};

	struct ResizeEvent
	{
		int left, right, top, bottom, width, height;
	};

	union WindowEvent
	{
		InputEvent input;
		ResizeEvent resize;
	};

	constexpr auto _sizeof_window_event_context = sizeof(WindowEvent);

	// Called while handling window messages.

	using WindowEventHandler = std::function<void(WindowEventType, const WindowEvent&)>;

	/// Window abstraction
	class Window
	{
	public:
		// Processes/flushes window messages, and calls appropiate callbacks
		virtual void flush_messages() = 0;

		// Pops open the window
		virtual void show() = 0;

		// Swaps to back buffer
		virtual void swap_buffers() = 0;

		// Initialize opengl rendering context
		virtual void init_opengl() = 0;
	};
}