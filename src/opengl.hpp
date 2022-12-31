#pragma once

/*
 * OpenGL renderer implementation
 */

#include "platform.hpp"

namespace prosper
{
    class OpenGLRenderer : public Renderer
    {
    public:
        virtual void init() override;

        virtual void begin_frame() override;

        virtual void end_frame() override;

<<<<<<< Updated upstream
        virtual void draw() override;
=======
        virtual void draw(const Mesh &mesh) override;
>>>>>>> Stashed changes

        virtual const ShaderProgram create_shader_program(const ShaderSource &shader_source) override;

        virtual void use_shader_program(const ShaderProgram &shader_program) override;

        virtual TextureHandle load_texture(const void *data, int width, int height, int num_channels) override;

    private:
        unsigned int _vao,   // holds vertex attributes
            _vbo,            // holds the actual vertex data
            _ebo,            // holds indicies to connect verticies
            _shader_program; // holds the shader
    };
}
