#include "game.hpp"
#include "core.hpp"

namespace prosper
{
    namespace game
    {
        bool initialize_gamestate(GameState &gs)
        {
            // Load textures



            return true;
        }

        void update_gamestate(GameState &gs)
        {
            // Advance 1 tick
            gs.tick_count++;

            // Calculate time according to current tick
            gs.time = gs.tick_count * gs.tick_interval;

            // Remember the last time we updated
            gs.last_update_time = gs.time;
        }

        void render_gamestate(GameState &gs, platform::Renderer &renderer)
        {
            renderer.draw_sprite();

            auto draw_background = [&]()
            {
                static int background_sprite_id = 0;
                renderer.draw_sprite();
            };

            renderer.swap();
        }
    }
}