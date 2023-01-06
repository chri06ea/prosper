#pragma once

#include <core/core.hpp>

#include "gamestate.hpp"

namespace prosper
{
	struct Entity
	{
		Vector3f position{100.f, 100.f, 0.f};

		virtual void update(GameState& gamestate) = 0;
	};
}