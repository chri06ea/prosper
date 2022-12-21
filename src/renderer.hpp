#pragma once

namespace prosper
{
    /// @brief rendering interface
    class Renderer
    {
    public:
        virtual void draw_sprite() = 0;
    };

    Renderer *create_renderer();
}
