#include "game.hpp"

#include <platform/platform.hpp>
#include <renderers/renderers.hpp>

#include <ui.hpp>

#include "traveling_merchant.hpp"

namespace prosper
{
	void Game::run_simulation(const EngineState& engine_state)
	{
		game_time = engine_state.time;

		// World event

		if(game_time - last_event_time > event_cooldown)
		{
			LOG_INFORMATION("Event!!!");

			last_event_time = game_time;
		}

		game_simulation_count++;
		game_time += game_time_step;
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
	}
}