#include "game.hpp"
#include "core.hpp"

#include <functional>

namespace prosper
{
    bool initialize_gamestate(GameState &gs)
    {
        gs.seed = 0;
        gs.tick_interval = 1.f / 64.f;

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

    void update_gamestate(GameState &gs)
    {
        // Advance 1 tick
        gs.tick_count++;

        // Calculate time according to current tick
        gs.time = gs.tick_count * gs.tick_interval;

        // Handle income
        {
            for (size_t i = 0; i < GameState::ResourceMax; i++)
            {
                gs.resources[i] += gs.resource_incomes[i];
            }
        }

        // Handle event
        {
            constexpr GameEvent game_event = static_cast<GameEvent>(1 % GameEvent::NumGameEvents);

            switch (game_event)
            {
            case GameEvent::Lucky:
                push_notification(
                    {.title = "Lucky",
                     .description = "You found a golden coin",
                     .callback = [&]()
                     {
                         gs.resources[GameState::Gold] += 100;
                     }});
                break;

            default:
                break;
            }
        }

        // Remember the last time we updated
        gs.last_update_time = gs.time;
    }

    void render_gamestate(GameState &gs, Renderer &renderer)
    {
        renderer.draw_sprite(0, 0);

        platform::swap_buffers();
    }
}