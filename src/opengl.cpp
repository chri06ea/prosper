#include "opengl.hpp"

#include "core.hpp" // For LOG macros
#include <glad/glad.h>

namespace prosper
{
    void OpenGLRenderer::init()
    {
    }

    void OpenGLRenderer::set_projection(const Matrix<float, 4> &projection)
    {
    }

    void OpenGLRenderer::draw_sprite(const Sprite &sprite)
    {
    }

    void OpenGLRenderer::render()
    {
    }

    ShaderProgram OpenGLRenderer::compile_shader(const ShaderSource &shader_source)
    {
        auto _compile_shader = [](std::string_view shader_source, std::string_view shader_type)
        {
            // Translate shader type string to integer value
            int shader_type_id{};
            if (shader_type == "vertex")
                shader_type_id = GL_VERTEX_SHADER;
            else if (shader_type == "fragment")
                shader_type_id = GL_FRAGMENT_SHADER;
            else
                throw std::runtime_error("Invalid shader type");
            // Compile that shit
            const auto source = shader_source.data();
            const auto shader = glCreateShader(shader_type_id);
            glShaderSource(shader, 1, &source, NULL);
            glCompileShader(shader);
            // Error checking
            int compilation_status;
            char compilation_log[512];
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_status);
            if (!compilation_status)
            {
                glGetShaderInfoLog(shader, 512, NULL, compilation_log);
                LOG_ERROR("ERROR::SHADER::COMPILATION_FAILED: " << compilation_log);
            };
            return shader;
        };
        // Compile shaders
        const auto vertex_shader = _compile_shader(shader_source.vertex_shader_source, "vertex");
        const auto fragment_shader = _compile_shader(shader_source.fragment_shader_source, "fragment");
        // Link shaders into shader program
        auto program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);
        // Error checking
        int compilation_status;
        char compilation_log[512];
        glGetProgramiv(program, GL_LINK_STATUS, &compilation_status);
        if (!compilation_status)
        {
            glGetProgramInfoLog(program, 512, NULL, compilation_log);
            LOG_ERROR("ERROR::SHADER::PROGRAM::COMPILATION_FAILED: " << compilation_log);
        }
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return {.shader_handle = program};
    }
    void OpenGLRenderer::use_shader_program(const ShaderProgram &shader_program)
    {
        glUseProgram(shader_program.shader_handle);
    }
}