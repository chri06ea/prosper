#pragma once

#include <platform/platform.hpp>

#include "engine_state.hpp"

namespace prosper
{
	// DI for interacting with game from engine
	struct EngineGame
	{
		virtual void run_simulation(const EngineState& engine_state) = 0;

		virtual void on_window_event(WindowEventType event, const WindowEvent& context) = 0;
	};
}