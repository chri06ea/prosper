#include "engine.hpp"

prosper::Engine::Engine(Platform& platform, EngineGame& game) : platform(platform), game(game)
{

}

void prosper::Engine::run_game()
{
	auto window_event_handler = [&](WindowEventType event, const WindowEvent& context)
	{
		game.on_window_event(event, context);
	};

	auto window = platform.create_window(window_event_handler);

	window->init_opengl();

	window->show();

	const auto platform_start_tick_count = platform.get_platform_tick_count();
	const auto platform_ticks_per_second = platform.get_platform_ticks_per_second();

	auto last_simulation_time = -1.f;
	const auto simulation_interval = 1.f / 64.f;

	//state.game_time_step = simulation_interval;

	while(true)
	{
		const auto platform_tick = platform.get_platform_tick_count();

		const auto simulation_time = (float) (platform_tick - platform_start_tick_count) / (float) platform_ticks_per_second;

		window->flush_messages();

		if(const auto time_since_simulation = simulation_time - last_simulation_time;
			time_since_simulation >= simulation_interval)
		{
			//run_simulation();
			game.run_simulation(*this);

			// TODO
			last_simulation_time = simulation_time;
		}

		//run_rendering();

		window->swap_buffers();
	}
}
