#include "topdownsim_game.h"


namespace lib
{
	TopdownSim::TopdownSim(IGameRenderer& game_renderer, IPlatform& platform, IFileSystem& filesystem, IWindow& window, GameState& gamestate)
		: 
		_game_renderer(game_renderer),
		_platform(platform),
		_filesystem(filesystem),
		_window(window),
		_gamestate(gamestate)
	{
		current_tick = 0;
		start_tick = _platform.get_tick_count();
		ticks_per_second = _platform.get_ticks_per_second();
		time = 0.f;
		last_simulation_time = -1.f;
		simulation_interval = 1.f / 64.f;
	}

	void TopdownSim::run()
	{
		initialize();

		while(true)
		{
			_window.process_messages();

			current_tick = _platform.get_tick_count();
			time = (float) (current_tick - start_tick) / (float) ticks_per_second;

			if(const auto time_since_simulation = time - last_simulation_time;
				time_since_simulation >= simulation_interval)
			{
				for(auto entity : _gamestate.entity_list)
				{

				}
			}

			_game_renderer.render(_gamestate);

			_window.swap_buffers();
		}
	}

	#define add_entity(...) 1

	void TopdownSim::initialize()
	{
		add_entity();
	}

	void TopdownSim::simulate()
	{
	}
}