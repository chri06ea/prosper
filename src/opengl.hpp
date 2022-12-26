#pragma once

#include "renderer.hpp"

namespace prosper
{
    class OpenGLRenderer : public Renderer
    {
    public:
        virtual void init() override;
        virtual void set_projection(const Matrix<float, 4> &projection) override;
        virtual void draw_sprite(const Sprite &sprite) override;
        virtual void render() override;
        virtual const ShaderProgram compile_shader(const ShaderSource &shader_source) override;
        virtual void use_shader_program(const ShaderProgram &shader_program) override;
    };
}