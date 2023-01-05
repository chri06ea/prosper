#include "game.hpp"

#include <platform/platform.hpp>

#include <renderers/renderers.hpp>

namespace prosper
{
	Game::Game(Platform& platform)
		: _platform(platform)
	{
	}

	void Game::run()
	{
		auto window_event_handler = [&](WindowEventType event, const WindowEvent& context)
		{
			on_window_event(event, context);
		};

		auto window = _platform.create_window(window_event_handler);

		window->init_opengl();

		window->show();

		while(true)
		{
			window->flush_messages();

			run_tick();

			run_rendering();

			window->swap_buffers();
		}
	}

	void Game::run_rendering()
	{
		static const auto render_device = _platform.get_render_device();
		static Allocator allocator;

		static SpriteRenderer test_renderer(render_device, allocator);
		test_renderer.push(0,0,100,100);
		test_renderer.push(300, 300, 100, 100);
		test_renderer.render();
	}

	void Game::run_tick()
	{
		auto& tick_count = state.tick_count;
		const auto& ticks_per_second = state.ticks_per_second;

		const auto tick_interval = 1.f / ticks_per_second;
		const auto tick_time = static_cast<float>(tick_count) * tick_interval;

		tick_count++;
	}

	void Game::on_window_event(WindowEventType event, const WindowEvent& context)
	{
		if(event == WindowEventType::Input)
		{
			if(context.input.type == InputEvent::Type::Key)
			{
				const auto& key = context.input.data.key_event.key;
				const auto& state = context.input.data.key_event.keystate;
			}
		}
		else if(event == WindowEventType::Resize)
		{
			const auto& resize = context.resize;
			_platform.get_render_device()->set_viewport({
				.x = resize.left, .y = resize.top,
				.w = resize.right - resize.left, .h = resize.bottom - resize.top
				});
		}
	}
}