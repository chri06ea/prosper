#pragma once

#include "platform.hpp" // for 'Renderer'

namespace prosper
{
    struct GameState
    {
        int tick_count{};         // how many ticks have been ran (simulation count)
        float tick_interval{};    // how much time passes per tick
        float time{};             // ingame time
        float last_update_time{}; // how much time since last gamestate update
    };

    bool initialize_gamestate(GameState &gs);

    /// @brief Update the world state. Time will be advanced by 'gamestate.tick_interval'
    /// @param gs
    void update_gamestate(GameState &gs);

    /// @brief Renders the game**
    /// @param gs
    void render_gamestate(GameState &gs, Renderer &renderer);
}