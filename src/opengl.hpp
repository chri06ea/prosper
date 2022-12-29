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

        virtual void draw() override;

        virtual const ShaderProgram create_shader_program(const ShaderSource &shader_source) override;

        virtual void use_shader_program(const ShaderProgram &shader_program) override;

        virtual TextureHandle load_texture(const void *data, int width, int height, int num_channels) override;

    private:
        unsigned int _vao, _vbo, _ibo, _shader_program;
    };
}
