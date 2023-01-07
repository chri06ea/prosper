#pragma once

#include "entity.hpp"

namespace prosper
{
	struct Player : Entity
	{
		// Inherited via Entity
		virtual void update(GameState& gamestate) override;
	};
}