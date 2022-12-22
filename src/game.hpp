#pragma once

#include <vector>

#include "platform.hpp" // for 'Renderer'

namespace prosper
{
    enum GameEvent
    {
        None,
        Lucky,        // You found a golden coin on the ground.
        LostTraveler, // A lost traveler seeks refugee
        NumGameEvents,
    };

    struct GameState
    {
        enum State
        {
            Initial,
            Town,
            GameOver,
            Ingame,
            Paused,
            StateMax
        };
        State state{};

        // Engine
        bool paused{};
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

        struct Citizen
        {
            enum State
            {
                Idling
            };

            State state;
        };

        const static int CitizensMax = 100;
        Citizen citizens[CitizensMax];

        const static int GameEventsMax = 1000;
        GameEvent game_events[GameEventsMax];

        const char *death_reason{};
    };

    bool initialize_gamestate(GameState &gs);

    /// @brief Update the world state. Time will be advanced by 'gamestate.tick_interval'
    /// @param gs
    void update_gamestate(GameState &gs);

    /// @brief Renders the game**
    /// @param gs
    void render_gamestate(GameState &gs, Renderer &renderer);
}