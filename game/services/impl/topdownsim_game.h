#pragma once

#include "../game.hpp"

namespace lib
{
	class TopdownSim : public IGame
	{
	public:
		TopdownSim(
			IGameRenderer& game_renderer,
			IPlatform& platform,
			IFileSystem& filesystem,
			IWindow& window,
			GameState& gamestate
		);

		virtual void run() override;

		void simulate();

	private:
		IGameRenderer& _game_renderer;
		IPlatform& _platform;
		IFileSystem& _filesystem;
		IWindow& _window;
		GameState& _gamestate;

		size_t current_tick;
		size_t start_tick;
		size_t ticks_per_second;
		float time;
		float last_simulation_time;
		float simulation_interval;
	};
}