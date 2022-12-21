#include "core.hpp"
#include "game.hpp"
#include "platform.hpp"

using namespace prosper::game;
using namespace prosper::platform;

int main()
{
    if (!initialize_platform())
    {
        emergency_shutdown("platform initialization failed");
    }

    GameState gs;

    if (!initialize_gamestate(gs))
    {
        emergency_shutdown("GameState initialization failed");
    }

    while (true)
    {
        process_input();

        update_gamestate(gs);

        LOG_INFORMATION(gs.tick_count);
    }

    return 0;
}