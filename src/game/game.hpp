#pragma once

#include <platform/services/platform.hpp>
#include <platform/services/window.hpp>
#include <platform/services/renderer.hpp>
#include <platform/services/filesystem.hpp>
#include <game/models/gamestate.hpp>
#include <game/models/enginestate.hpp>
#include <game/models/renderstate.hpp>

namespace lib
{
	class Game
	{
	public:

		void initialize(RenderState& rs);

		void initialize(EngineState& es);

		void render(EngineState& es, GameState& gs, RenderState& rs);

		void simulate(EngineState& es, GameState& gs);

		void update(GameState& gs, EngineState& es, RenderState& rs);

	private:

		IPlatform& _platform;
		IRenderer& _renderer;
		IFileSystem& _filesystem;
	};
}