#include "game.hpp"

namespace prosper
{
    void process_input(GameState &gamestate, Key key, KeyState keystate)
    {
    }

    void process_rendering(GameState &gamestate, Renderer *renderer)
    {

        renderer->begin_frame();

        renderer->draw();

        renderer->end_frame();
    }

    void process_tick(GameState &gamestate)
    {
        // Advance 1 tick
        gamestate.tick_count++;

        // Calculate ingame time
        gamestate.time = gamestate.tick_count * gamestate.tick_interval;
    }
}