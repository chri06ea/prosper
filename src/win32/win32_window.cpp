#include "win32_window.hpp"

#include <stdexcept>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "wgl.hpp"

namespace prosper
{
	namespace
	{
		struct WindowContext
		{
			HWND window_handle;
			HDC device_context;
			WindowEventHandler window_event_handler;
		};

		std::unordered_map<void*, WindowContext> window_contexts;

		// Win32 window callback
		LRESULT CALLBACK window_proc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			LRESULT result{};

			switch(msg)
			{
				case WM_SIZE:
				{
					RECT client_rect;

					if(!GetClientRect(window, &client_rect))
						CRITICAL_ERROR("Failed getting client rect size");

					const auto width = (client_rect.right - client_rect.left);
					const auto height = (client_rect.bottom - client_rect.top);

					// Call the window event handler associated with this window
					window_contexts[window]
						.window_event_handler(WindowEventType::Resize, WindowEvent{
							.resize = {
								.left = client_rect.left,
								.right = client_rect.right,
								.top = client_rect.top,
								.bottom = client_rect.bottom,
								.width = width,
								.height = height}});
					break;
				}

				default:
					result = DefWindowProcA(window, msg, wparam, lparam);
					break;
			}

			return result;
		}
	}

	Win32Window::Win32Window()
	{
	}

	Win32Window* Win32Window::create(WindowEventHandler window_event_handler)
	{
		const auto title = "Prosper";

		// Setup and create game window class
		WNDCLASSEXA window_class{
			.cbSize = sizeof(WNDCLASSEXA),
			.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
			.lpfnWndProc = window_proc,
			.hInstance = GetModuleHandle(0),
			.hCursor = LoadCursor(0, IDC_ARROW),
			.lpszClassName = title};

		if(!RegisterClassExA(&window_class))
			CRITICAL_ERROR("failed registering window class");

		// Create game window
		auto window_handle = CreateWindowExA(0, title, title,
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			800, 800, 0, 0, GetModuleHandle(0), 0);

		if(window_handle == NULL)
			CRITICAL_ERROR("failed creating window");

		// Acquire device context
		auto device_context = GetDC(window_handle);

		// Store context
		window_contexts[window_handle] = {
			.window_handle = window_handle,
			.device_context = device_context,
			.window_event_handler = window_event_handler};

		auto window = new Win32Window();

		window->_context_handle = window_handle;

		return window;
	}

	void Win32Window::destroy(Win32Window* window)
	{
		throw std::underflow_error("win32window::destroy missing implementation");
	}

	void Win32Window::flush_messages()
	{
		MSG msg = {};
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Win32Window::show()
	{
		ShowWindow(window_contexts[_context_handle].window_handle, SW_NORMAL);
	}

	void Win32Window::swap_buffers()
	{
		if(!SwapBuffers(window_contexts[_context_handle].device_context))
			CRITICAL_ERROR("failed SwapBuffers call");
	}

	void Win32Window::init_opengl()
	{
		init_opengl_rendering_context_with_wgl(window_contexts[_context_handle].device_context);
	}
}