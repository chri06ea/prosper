#include <iostream>

#include <platform/services/impl/win32_platform.hpp>
#include <platform/services/impl/win32_filesystem.hpp>
#include <platform/services/impl/win32_window.hpp>
#include <platform/services/impl/opengl_renderer.hpp>

#include <game/services/impl/topdownsim_game.h>
#include <game/services/impl/orthographic_game_renderer.hpp>

using namespace lib;
GameState gamestate;

int main()
{
	auto platform = Win32Platform();
	auto filesystem = Win32FileSystem();
	auto window = Win32Window("Dev", [](auto event) {});
	window.init_graphics();

	auto renderer = OpenGLRenderer();
	auto orthographic_game_renderer = OrthographicGameRenderer(renderer, filesystem);
	auto game = TopdownSim(orthographic_game_renderer, platform, filesystem, window, gamestate);

	game.run();
}
