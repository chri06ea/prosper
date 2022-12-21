#include "core.hpp"
#include "game.hpp"
#include "platform.hpp"
#include "renderer.hpp"

using namespace prosper;
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

    auto renderer = create_renderer();

    if (!renderer)
    {
        emergency_shutdown("create_renderer initialization failed");
    }

    while (true)
    {
        process_input();

        update_gamestate(gs);

        render_gamestate(gs, *renderer);
    }

    return 0;
}