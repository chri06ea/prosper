#pragma once

namespace lib
{
	struct EngineState
	{
		size_t start_tick{};
		size_t current_tick{};
		size_t ticks_per_second{};

		float time{};
		float last_simulation_time{-1.f};
	};
}