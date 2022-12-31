#include "game.hpp"

namespace prosper
{
    void process_input(GameState &gamestate, Key key, KeyState keystate)
    {
    }

    void process_rendering(GameState &gamestate, Renderer *renderer)
    {
        renderer->begin_frame();

        // Draw scenary (pretty much the background)

        // Draw sun

        // Draw ground

        renderer->draw({.vertices = {{
                            {-1.00f, -1.00f, 0.f}, // Bottom left
                            {+1.00f, -1.00f, 0.f}, // Bottom right
                            {+1.00f, -0.25f, 0.f}, // Top right
                            {-1.00f, -0.25f, 0.f}, // Top left
                        }}});

        // Draw buildings

        // Draw entities

        // Draw overlay things (bottom bar, text, popup etc)

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