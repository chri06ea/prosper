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
		void run_tick();

		// process a window input event
		void on_window_event(WindowEventType event, const WindowEvent& context);

		struct State
		{
			uint64_t tick_count = 0ull;

			float ticks_per_second = 64.f;
		} state;

	private:
		Platform& _platform;
	};
}

