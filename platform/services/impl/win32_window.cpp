#include "win32_window.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <platform/dependencies/wgl.hpp>

namespace lib
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
						.window_event_handler(WindowEvent{
							.type = WindowEvent::Type::Resize,
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
	HWND create_window(const char* const title, WindowEventHandler window_event_handler)
	{
		WNDCLASSEXA window_class{
			.cbSize = sizeof(WNDCLASSEXA),
			.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
			.lpfnWndProc = window_proc,
			.hInstance = GetModuleHandle(0),
			.hCursor = LoadCursor(0, IDC_ARROW),
			.lpszClassName = title};

		if(!RegisterClassExA(&window_class))
			CRITICAL_ERROR("failed registering window class");

		auto window_handle = CreateWindowExA(0, title, title,
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			800, 800, 0, 0, GetModuleHandle(0), 0);

		if(window_handle == NULL)
			CRITICAL_ERROR("failed creating window");

		auto device_context = GetDC(window_handle);

		// Store context
		window_contexts[window_handle] = {
			.window_handle = window_handle,
			.device_context = device_context,
			.window_event_handler = window_event_handler};

		return window_handle;
	}

	Win32Window::Win32Window(void* context_handle)
		: _context_handle(context_handle)
	{
	}

	void Win32Window::swap_buffers()
	{
		if(!SwapBuffers(window_contexts[_context_handle].device_context))
			CRITICAL_ERROR("failed SwapBuffers call");
	}

	void Win32Window::process_messages()
	{
		MSG msg = {};
		while(PeekMessage(&msg, window_contexts[_context_handle].window_handle, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Win32Window::get_size(size_t& width, size_t& height)
	{
	}

	void Win32Window::get_position(size_t& width, size_t& height)
	{
	}

	void Win32Window::init_graphics()
	{
		init_opengl_rendering_context_with_wgl(window_contexts[_context_handle].device_context);
	}

	Shared<lib::IWindow> Win32WindowFactory::create(lib::WindowEventHandler handler)
	{
		auto hwnd = create_window("win32window", handler);
		ShowWindow(hwnd, SW_SHOW);
		return make_shared<Win32Window>(hwnd);
	}
}
