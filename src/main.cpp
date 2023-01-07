#include <game/game.hpp>
#include <engine/engine.hpp>

using namespace prosper;

int main()
{
	Game game;

	Platform platform{};
	Engine engine(platform, game);

	engine.run_game();
}