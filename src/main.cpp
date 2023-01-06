#include <game/game.hpp>

using namespace prosper;

int main()
{
	Platform platform{};

	Game game(platform);

	game.run();
}