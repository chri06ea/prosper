#include "game.hpp"

#include <platform/platform.hpp>
#include <renderers/renderers.hpp>

#include <ui.hpp>

#include "traveling_merchant.hpp"

namespace prosper
{
	static TravelingMerchant merchant;

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

		const auto platform_start_tick_count = _platform.get_platform_tick_count();
		const auto platform_ticks_per_second = _platform.get_platform_ticks_per_second();

		auto last_simulation_time = -1.f;
		const auto simulation_interval = 1.f / 64.f;

		state.game_time_step = simulation_interval;

		while(true)
		{
			const auto platform_tick = _platform.get_platform_tick_count();

			const auto simulation_time = (float) (platform_tick - platform_start_tick_count) / (float) platform_ticks_per_second;

			window->flush_messages();

			if(const auto time_since_simulation = simulation_time - last_simulation_time;
				time_since_simulation >= simulation_interval)
			{
				run_simulation();

				// TODO
				last_simulation_time = simulation_time;
			}

			run_rendering();

			window->swap_buffers();
		}
	}

	void Game::run_rendering()
	{
		static SpriteRenderer test_renderer(&_platform);

		auto push_background = [&]()
		{
			test_renderer.push_ndc(
				{-1.00f,+1.00f},
				{-1.00f,-1.00f},
				{+1.00f,+1.00f},
				{+1.00f,-1.00f},
				{0.0f,1.f,0.f,1.f});
		};

		auto push_ui = []()
		{
			std::function<void(UIElement&)> push_ui_element = [](UIElement& elem)
			{
				switch(elem.type)
				{
					case UIElement::Type::Frame:
					{
						const auto& data = elem.data.frame;

						test_renderer.push(elem.x, elem.y, elem.w, elem.h, data.color);

						break;
					}

					default:
					{
						test_renderer.push(elem.x, elem.y, elem.w, elem.h, {1.f, 0.f, 1.f, 1.f});
					}
				}
			};

			UIElement elem{.x = 0, .y = 0, .w = 100, .h = 100};

			push_ui_element(elem);

			test_renderer.push(300, 300, 100, 100, {1.f, 0.f, 1.f, 1.f});
			test_renderer.push(500, 500, 40, 40, {1.f, 1.f, 1.f, 1.f});
		};

		push_background();

		push_ui();


		// world space to screen space
		test_renderer.push(merchant.position[0], merchant.position[1], 25, 25);
		test_renderer.render();
	}

	void Game::run_simulation()
	{
		merchant.update(state);

		// World event

		if(state.game_time - state.last_event_time > state.event_cooldown)
		{
			LOG_INFORMATION("Event!!!");

			state.last_event_time = state.game_time;
		}

		state.game_simulation_count++;
		state.game_time += state.game_time_step;
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