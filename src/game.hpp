#pragma once

/*
 * Game logic
 */

#include "platform.hpp"

namespace prosper
{
    struct GameState
    {
        bool paused{};
        int seed{};               // Game rng seed
        int tick_count{};         // how many ticks have been ran (simulation count)
        float tick_interval{};    // how much time passes per tick
        float time{};             // ingame time
        float last_update_time{}; // how much time since last gamestate update
    };

    // Create default gamestate
    inline static auto create_new_gamestate()
    {
        GameState gamestate;

        gamestate.seed = 0;
        gamestate.tick_interval = 1.f / 64.f;

        return gamestate;
    }
}

namespace prosper
{
    // Translate user input into actions
    void process_input(GameState &gamestate, Key key, KeyState keystate);

    // Render according to whats in gamestate
    void process_rendering(GameState &gamestate, Renderer *renderer);

    // Run a single game tick
    void process_tick(GameState &gamestate);
}
