#pragma once

#include <cstdint>

namespace prosper
{
	struct EngineState
	{
		size_t tick_count{};

		size_t ticks_per_second{64};
	};
}