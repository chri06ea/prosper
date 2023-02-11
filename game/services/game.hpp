#pragma once

#include <platform/services/platform.hpp>
#include <platform/services/window.hpp>
#include <platform/services/renderer.hpp>
#include <platform/services/filesystem.hpp>

#include <game/models/gamestate.hpp>
//#include <game/models/enginestate.hpp>
//#include <game/models/renderstate.hpp>

#include "game_renderer.hpp"

namespace lib
{
	class IGame
	{
	public:

		virtual void run() = 0;
	};
}