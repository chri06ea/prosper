#pragma once

/*
 * Game logic
 */

#include "platform.hpp"

namespace prosper
{
    struct GameState
    {
        // The current tick count of the platform
        uint64_t platform_tick{};

        // The platform tick the moment the gamestate was initialized.
        uint64_t platform_initial_tick{};

        // The amount of ticks that
        uint64_t platform_ticks_per_second{};

        // The last platform tick the game was simulated
        uint64_t platform_last_simulation_tick{};

        // The amount of game ticks
        uint64_t simulation_count{};

        // The amount of game ticks per second
        uint64_t simulations_per_second{};

        // The time during simulation (calculated from platform ticks)
        float simulation_time{};
    };

    // Create default gamestate
    inline static auto create_new_gamestate()
    {
        GameState gamestate;

        gamestate.platform_initial_tick = get_platform_tick_count();
        gamestate.platform_ticks_per_second = get_platform_ticks_per_second();

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
