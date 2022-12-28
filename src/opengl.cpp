#include "opengl.hpp"

#include "core.hpp" // For LOG macros
#include <glad/glad.h>

namespace prosper
{
    /*
     *   OpenGL Window
     */

    void OpenGLWindow::init()
    {
    }

    void OpenGLWindow::swap_buffers()
    {
    }

    /*
     *   OpenGL Renderer
     */
    // TODO: Query from GPU
    const auto MAX_QUAD_COUNT = 1000u;                                 // Maximum allowed count of quads(gpu can only support so many)
    const auto VERTICIES_PER_QUAD = 4u;                                // A quad require 4 points (upper left, upper right, bottom left, bottom right)
    const auto INDICIES_PER_QUAD = 6u;                                 // Edges needed to connect a quad. Think of this as 'how many pen strokes would i need to draw this IRL'. In computer graphics, everything is triangles. Each triangles requires 3 pen strokes. A quad consists of two triangles. Therefore 6.
    const auto MAX_VERTEX_COUNT = MAX_QUAD_COUNT * VERTICIES_PER_QUAD; // Highest allowed vertex count
    const auto MAX_INDEX_COUNT = MAX_QUAD_COUNT * INDICIES_PER_QUAD;   // Each quad is drawn as 2 triangles. To do this, 6 lines is needed (3 for each triangle)

    void OpenGLRenderer::init()
    {
        // Set up the VAO
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        // Set up the VBO
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW); // Allocate dynamic vertex buffer
        // Set up the parameters/input data layout for the shader
        // TODO: Query attribute locations. This code will break if attribute order is changed in shader!
        glEnableVertexAttribArray(0); // Core profile mode uses this override. In compatility mode, VAO must be provided as first argument.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, position));
        // Set up the IBO
        int indicies[MAX_INDEX_COUNT];
        //? 'Indency' is not a real word. However, it refers to the 'index(number) of the index(indicies)'
        for (size_t indency_index = 0, vertex_index = 0; indency_index < MAX_INDEX_COUNT; indency_index += INDICIES_PER_QUAD, vertex_index += VERTICIES_PER_QUAD)
        {
            // 'Connnect' the first triangle
            indicies[indency_index + 0] = vertex_index + 0; // Notice how indicies are 'reused'
            indicies[indency_index + 1] = vertex_index + 1; // 0-1-2 forms first triangle
            indicies[indency_index + 2] = vertex_index + 2;

            // ... second triangle
            indicies[indency_index + 3] = vertex_index + 2; // 2-3-0 forms second triangle
            indicies[indency_index + 4] = vertex_index + 3;
            indicies[indency_index + 5] = vertex_index + 0;
        }
        // Put indicies data on GPU
        glGenBuffers(1, &_ibo);
        glBindBuffer(GL_ARRAY_BUFFER, _ibo);
        glBufferData(_ibo, sizeof(indicies), indicies, GL_STATIC_DRAW);
    }

    void OpenGLRenderer::set_projection(const Matrix<float, 4> &projection)
    {
    }

    void OpenGLRenderer::draw_sprite(const Sprite &sprite)
    {
    }

    void OpenGLRenderer::render()
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);
        // Bind the VAO
        glBindVertexArray(_vao);
        // Use the shader program
        glUseProgram(_shader_program);
        // Draw the object
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Unbind the VAO
        glBindVertexArray(0);
    }

    const ShaderProgram OpenGLRenderer::compile_shader(const ShaderSource &shader_source)
    {
        auto _compile_shader = [](std::string_view shader_source, std::string_view shader_type)
        {
            // Translate shader type string to opengl shader type id
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

    unsigned int OpenGLRenderer::load_texture(const void *data, int width, int height, int num_channels)
    {
        // Generate and bind texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // generate the texture (and mipmap)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        return texture;
    }
}