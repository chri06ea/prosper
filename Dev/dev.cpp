#include <iostream>
#include <game/game.hpp>

#include <platform/services/impl/win32_platform.hpp>
#include <platform/services/impl/win32_filesystem.hpp>
#include <platform/services/impl/win32_window.hpp>
#include <platform/services/impl/opengl_renderer.hpp>

using namespace lib;

RenderState rs;
EngineState es;
GameState gs;

int main()
{
	auto platform = Win32Platform();
	auto filesystem = Win32FileSystem();
	auto window = Win32WindowFactory().create([](auto event){});

	window->init_graphics();

	auto renderer = OpenGL();

	Game game(platform, filesystem, *window, renderer);
	game.main(gs, es, rs);
}
