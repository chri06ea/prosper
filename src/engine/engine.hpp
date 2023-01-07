#pragma once

#include <platform/platform.hpp>

#include "engine_game.hpp"

namespace prosper
{
	class GameRenderer
	{
	public:
		
	};

	class Engine : public EngineState
	{
	public:
		Engine(Platform& platform, EngineGame& game);

		void run_game();

	private:

		void render_ui();

		void render();

		EngineGame& game;
		Platform& platform;
	};
}