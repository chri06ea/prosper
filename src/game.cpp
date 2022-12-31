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

        renderer->draw({.vertices = {{
                            // Center box
                            {-0.25f, -0.25f, 0.f}, // Bottom left
                            {0.25f, -0.25f, 0.f},  // Bottom right
                            {0.25f, 0.25f, 0.f},   // Top right
                            {-0.25f, 0.25f, 0.f},  // Top left
                        }}});

        renderer->draw({.vertices{{
            // Top right box
            {-0.25f + 0.75f, -0.25f + 0.75f, 0.f}, // Bottom left
            {0.25f + 0.75f, -0.25f + 0.75f, 0.f},  // Bottom right
            {0.25f + 0.75f, 0.25f + 0.75f, 0.f},   // Top right
            {-0.25f + 0.75f, 0.25f + 0.75f, 0.f},  // Top left
        }}});

        // Draw sun

        // Draw ground

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