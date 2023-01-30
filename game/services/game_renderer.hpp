#pragma once

#include <platform/services/renderer.hpp>

#include <game/models/gamestate.hpp>

namespace lib
{
	class IGameRenderer
	{
	public:

		virtual void render(const GameState&) = 0;
	};
}