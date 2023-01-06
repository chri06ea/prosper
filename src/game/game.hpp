#pragma once

/*
 * Game logic
 */

#include <platform/platform.hpp>

#include "gamestate.hpp"

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


	private:
		Platform& _platform;

		GameState state;
	};
}

