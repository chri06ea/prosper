#pragma once

#include <platform/platform.hpp>

#include "engine_game.hpp"

namespace prosper
{
	class Engine : public EngineState
	{
	public:
		Engine(Platform& platform, EngineGame& game);

		void run_game();

	private:
		EngineGame& game;
		Platform& platform;
	};
}