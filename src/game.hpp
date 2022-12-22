#pragma once

#include "platform.hpp" // for 'Renderer'

namespace prosper
{
    enum GameEvent
    {
        None,
        Lucky, // You found a golden coin on the ground.

        NumGameEvents,
    };

    struct GameState
    {
        enum State
        {
            Initial,
            Town,
            StateMax
        };
        State state{};

        // Engine
        int seed{};               // Game rng seed
        int tick_count{};         // how many ticks have been ran (simulation count)
        float tick_interval{};    // how much time passes per tick
        float time{};             // ingame time
        float last_update_time{}; // how much time since last gamestate update

        // Game
        enum Resource
        {
            Gold,
            Food,
            ResourceMax,
        };
        int resources[ResourceMax];
        int resource_incomes[ResourceMax];
    };

    bool initialize_gamestate(GameState &gs);

    /// @brief Update the world state. Time will be advanced by 'gamestate.tick_interval'
    /// @param gs
    void update_gamestate(GameState &gs);

    /// @brief Renders the game**
    /// @param gs
    void render_gamestate(GameState &gs, Renderer &renderer);
}