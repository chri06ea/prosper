#pragma once

/*
 * Game logic
 */

#include <platform/platform.hpp>

namespace prosper
{
	class Game
	{
	public:
		// Constructor
		Game(Platform& platform);

		// Run the game
		void run();

		// Renders the game
		void run_rendering();

		// Run a game tick
		void run_simulation();

		// process a window input event
		void on_window_event(WindowEventType event, const WindowEvent& context);

		struct State
		{
			size_t tick_count = 0ull;

			float ticks_per_second = 64.f;

			static constexpr auto world_dimensions = 1000.f;

			size_t game_simulation_count{};

			float game_time{};
			float game_time_step = 1.f;

			float last_event_time{};
			float event_cooldown = 5.f;
		} state;

	private:
		Platform& _platform;
	};
}

