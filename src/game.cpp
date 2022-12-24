#include "game.hpp"
#include "core.hpp"

#include <functional>

namespace prosper
{
    bool initialize_gamestate(GameState &gs)
    {
        gs.seed = 0;
        gs.tick_interval = 1.f / 64.f;

        for (size_t i = 0; i < GameState::ResourceMax; i++)
        {
            gs.resources[i] = 100;
            gs.resource_incomes[i] = 0;
        }

        return true;
    }

    struct Notification
    {
        const char *title, *description;
        std::function<void()> callback;
    };

    void push_notification(Notification notification)
    {
        LOG_INFORMATION(notification.title << notification.description);
    }

    void push_ok_reject_notification(Notification notification)
    {
    }

    void update_gamestate(GameState &gs)
    {
        LOG_INFORMATION("Tick " << gs.tick_count << " Time " << gs.time << " State " << gs.state);

        switch (gs.state)
        {
        case GameState::Initial:
        {
            push_notification({
                .title = "Welcome",
                .description = "Do your best to prosper, settler",
            });

            gs.state = GameState::Ingame;
            break;
        };
        case GameState::Ingame:
        {
            // Advance 1 tick
            gs.tick_count++;

            // Calculate time according to current tick
            gs.time = gs.tick_count * gs.tick_interval;

            // Handle citizen ai
            {
                for (auto &citizen : gs.citizens)
                {
                    citizen.state = GameState::Citizen::Idling;
                }
            }

            // Handle income
            {
                for (size_t i = 0; i < GameState::ResourceMax; i++)
                {
                    gs.resources[i] += gs.resource_incomes[i];
                }
            }

            // Handle deathcons
            {
                if (gs.resources[GameState::Food] <= 0)
                {
                    // Starved to death
                    gs.state = GameState::GameOver;
                    gs.death_reason = "starvation";
                    return;
                }
            }

            // Handle event
            {
                GameEvent game_event = static_cast<GameEvent>(rand() % GameEvent::NumGameEvents);

                switch (game_event)
                {
                case GameEvent::Lucky:
                    gs.resources[GameState::Gold] += 100;

                    push_notification(
                        {.title = "Lucky",
                         .description = "You found a golden coin",
                         .callback = [&]() {
                         }});

                    break;
                case GameEvent::LostTraveler:
                {
                    int roll = rand();

                    push_ok_reject_notification(
                        {.title = "Lost traveler",
                         .description = "A lost traveler seeks aid",
                         .callback = [&]()
                         {
                             return false;
                         }});
                }
                break;
                default:
                    break;
                }
            }

            // Remember the last time we updated
            gs.last_update_time = gs.time;
            break;
        }
        }
    }

    void render_gamestate(GameState &gs, Renderer &renderer)
    {
        renderer.draw_sprite({});

        renderer.render();

        platform::swap_buffers();
    }
}