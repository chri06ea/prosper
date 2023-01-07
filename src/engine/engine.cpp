#include "engine.hpp"

prosper::Engine::Engine(Platform& platform, EngineGame& game) : platform(platform), game(game)
{

}

void prosper::Engine::run_game()
{
	auto window_event_handler = [&](WindowEventType event, const WindowEvent& context)
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
			platform.get_render_device()->set_viewport({
				.x = resize.left, .y = resize.top,
				.w = resize.right - resize.left, .h = resize.bottom - resize.top
				});
		}
		game.on_window_event(event, context);
	};

	auto window = platform.create_window(window_event_handler);

	window->init_opengl();

	window->show();

	start_tick_count = platform.get_platform_tick_count();
	ticks_per_second = platform.get_platform_ticks_per_second();

	auto last_simulation_time = -1.f;
	const auto simulation_interval = 1.f / 64.f;

	while(true)
	{
		tick_count = platform.get_platform_tick_count();

		time = (float) (tick_count - start_tick_count) / (float) ticks_per_second;

		window->flush_messages();

		if(const auto time_since_simulation = time - last_simulation_time;
			time_since_simulation >= simulation_interval)
		{
			game.run_simulation(*this);

			// TODO: Accumulate lost time
			last_simulation_time = time;
		}

		render();

		window->swap_buffers();
	}
}

#include <ui.hpp>
#include <renderers/sprite_renderer.hpp>


void prosper::Engine::render_ui()
{
	static SpriteRenderer test_renderer(&platform);

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
	test_renderer.render();

}

void prosper::Engine::render()
{
	render_ui();
}
