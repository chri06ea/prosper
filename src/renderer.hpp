#pragma once

#include <math/math.hpp>

namespace prosper
{
    struct Sprite
    {
    };

    class Renderer
    {
    public:
        // Initializes the sprite renderer
        virtual void init() = 0;

        // Sets the projection matrix for the renderer
        virtual void set_projection(const Matrix<float, 4> &projection) = 0;

        // Adds a sprite to the renderer
        virtual void draw_sprite(const Sprite &sprite) = 0;

        // Renders all sprites in the renderer
        virtual void render() = 0;
    };

    Renderer *create_renderer();
}
