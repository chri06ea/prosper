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

	class IGameRenderer
	{
	public:

		virtual void initialize() = 0;

		virtual void begin_scene(RenderState& rs) = 0;

		virtual void end_scene(RenderState& rs) = 0;
	};

	class IGameEngine
	{
	public:

		virtual void initialize() = 0;

		virtual void begin_scene(RenderState& rs) = 0;

		virtual void end_scene(RenderState& rs) = 0;
	};

	class IGameImpl
	{
	public:

		virtual void simulate() = 0;
	};

	class Game
	{
		IGameRenderer& _game_renderer;

	public:

		Game(IPlatform& platform, IFileSystem& filesystem,
			IWindow& window, IRenderer& renderer);

		void main(GameState& gs, EngineState& es, RenderState& rs);

		void render(GameState& gs, EngineState& es, RenderState& rs);

		void simulate(GameState& gs, EngineState& es);

		void initialize(RenderState& rs);

		void initialize(EngineState& es);

		void initialize(GameState& gs);

	private:
		IPlatform& _platform;
		IFileSystem& _filesystem;
		IRenderer& _renderer;
		IWindow& _window;

		void begin_scene(RenderState& rs);

		void end_scene(RenderState& rs);
	};
}