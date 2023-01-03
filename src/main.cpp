#include "game.hpp"

using namespace prosper;

int main()
{
	prosper::Renderer* renderer{};
	prosper::Window* window{};
	prosper::Sound* sound{};

	auto gamestate = create_new_gamestate();

	// event handler responsible for forwarding window events to the game layer
	auto window_event_handler = [&](WindowEvent event, const WindowEventContext& context)
	{
		if(event == WindowEvent::Input)
		{
			process_input(gamestate, context.input.key, context.input.keystate);
		}
		else if(event == WindowEvent::Resize)
		{
			renderer->set_viewport({.x = 0, .y = 0, .w = context.resize.width, .h = context.resize.height});
		}
	};

	window = create_window(window_event_handler);

	renderer = create_renderer(window);

	sound = create_sound();

	window->show();

	const auto start_tick = get_platform_tick_count();
	const auto ticks_per_second = get_platform_ticks_per_second();

	while(true)
	{
		const auto current_tick = get_platform_tick_count();

		gamestate.simulation_time = (float) (current_tick - start_tick) / (float) ticks_per_second;

		window->process_messages();

		process_tick(gamestate);

		process_rendering(gamestate, renderer);

		window->swap_buffers();
	}
}