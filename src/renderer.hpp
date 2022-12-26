#pragma once

#include <math/math.hpp>
#include <string>
#include <unordered_map>

namespace prosper
{
    struct Sprite
    {
    };

    // 'Uncompiled' shader
    struct ShaderSource
    {
        // Source code of the vertex shader
        std::string vertex_shader_source;

        // Source code of the fragment shader
        std::string fragment_shader_source;
    };

    // 'Compiled' shader
    struct ShaderProgram
    {
        // A handle the the compiled shader
        unsigned int shader_handle;
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

        // Compile a shader source into shader program
        virtual const ShaderProgram compile_shader(const ShaderSource &shader_source) = 0;

        // Use a shader a shader program
        virtual void use_shader_program(const ShaderProgram &shader_program) = 0;
    };

    Renderer *create_renderer();
}
