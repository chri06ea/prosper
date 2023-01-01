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
        constexpr auto sun_vertices = generate_quad_vertices(0.1, 0.9, 0.05, 0.05);
        renderer->draw({.vertices = sun_vertices});

        // Draw ground
        constexpr auto ground_vertices = generate_quad_vertices(0, 0, 1, 0.2);
        renderer->draw({.vertices = ground_vertices});

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