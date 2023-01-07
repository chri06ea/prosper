#pragma once

/*
 * Game logic
 */

#include <platform/platform.hpp>
#include <engine/engine.hpp>

#include "gamestate.hpp"

namespace prosper
{
	class Game : public EngineGame, public GameState
	{
	public:
		virtual void run_simulation(const EngineState& engine_state) override;

		virtual void on_window_event(WindowEventType event, const WindowEvent& context) override;
	};
}

