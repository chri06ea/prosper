#pragma once

namespace prosper
{
    /// @brief rendering interface
    class Renderer
    {
    public:
        virtual void draw_sprite(int x, int y) = 0;
    };

    Renderer *create_renderer();
}
