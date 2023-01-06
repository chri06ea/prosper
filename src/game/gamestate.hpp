#pragma once

namespace prosper
{
	struct GameState
	{
		size_t tick_count = 0ull;

		float ticks_per_second = 64.f;

		static constexpr auto world_dimensions = 1000.f;

		size_t game_simulation_count{};

		float game_time{};
		float game_time_step = 1.f;

		float last_event_time{};
		float event_cooldown = 5.f;
	};
}