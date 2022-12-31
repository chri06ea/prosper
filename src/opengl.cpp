#include "opengl.hpp"

#include <glad/glad.h>

// Wrapper for doing opengl calls, which will also print error (if any)
#define GL_CALL(CALL)                                  \
    CALL;                                              \
    if (auto glErr = glGetError(); glErr)              \
    {                                                  \
        LOG_INFORMATION(#CALL << " Error: " << glErr); \
    }

namespace prosper
{
    const auto MAX_QUAD_COUNT = 1000u;                                // Maximum allowed count of quads(gpu can only support so many)
    const auto MAX_VERTEX_COUNT = MAX_QUAD_COUNT * VERTICES_PER_QUAD; // Highest allowed vertex count
    const auto MAX_INDEX_COUNT = MAX_QUAD_COUNT * INDICES_PER_QUAD;   // Each quad is drawn as 2 triangles. To do this, 6 lines is needed (3 for each triangle)

    void OpenGLRenderer::init()
    {

        if (!gladLoadGL())
            throw std::runtime_error("Failed initializing GLAD (OpenGL function pointers)");

        //* Setup VAO
        GL_CALL(glGenVertexArrays(1, &_vao));
        GL_CALL(glBindVertexArray(_vao));
        {
            //* Setup VBO
            GL_CALL(glGenBuffers(1, &_vbo));
            GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
            GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_QUAD_COUNT, NULL, GL_DYNAMIC_DRAW));

            //* Setup EBO
            GL_CALL(glGenBuffers(1, &_ebo));
            GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
            static unsigned int indices[MAX_INDEX_COUNT];
            for (size_t indency_index = 0, vertex_index = 0; indency_index < MAX_INDEX_COUNT;
                 indency_index += INDICES_PER_QUAD, vertex_index += VERTICES_PER_QUAD)
            {
                //? 'Indency' is not a real word. However, it refers to the 'index(number) of the index(indices)'

                // 'Connnect' the first triangle
                indices[indency_index + 0] = vertex_index + 0; // 0-1-2 forms first triangle
                indices[indency_index + 1] = vertex_index + 1;
                indices[indency_index + 2] = vertex_index + 2;

                // ... second triangle
                indices[indency_index + 3] = vertex_index + 2; // 2-3-0 forms second triangle
                indices[indency_index + 4] = vertex_index + 3; // Notice how indicies are 'reused'
                indices[indency_index + 5] = vertex_index + 0;
            }
            GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
        }
        // Specify VBO data layout. (This has to be done AFTER initializing the vbo. The order of calls)
        // TODO: Query attribute locations.
        GL_CALL(glEnableVertexAttribArray(0))
        GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0))
        GL_CALL(glBindVertexArray(0));
    }

    void OpenGLRenderer::begin_frame()
    {
        // Clear screen
        // GL_CALL(glClearColor(1.f, 0.f, 1.f, 1.f));
        // GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        // Clear vertices from previous frame
        _vertices.clear();

        // Preallocate vector
        _vertices.reserve(MAX_VERTEX_COUNT);
    }

    void OpenGLRenderer::end_frame()
    {
        // Bind the VAO.
        GL_CALL(glBindVertexArray(_vao));
        // Send vertex data to the GPU. The VAO holds the VBO binding, so we can just copy
        GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size() * sizeof(Vertex), _vertices.data()));
        // Draw all vertex data.
        GL_CALL(glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0)); //? It's also possible to draw as GL_QUADS. Not sure what's better.
        GL_CALL(glBindVertexArray(0));
    }

    void OpenGLRenderer::draw(const Mesh &mesh)
    {
        _vertices.insert(_vertices.end(), std::begin(mesh.vertices), std::end(mesh.vertices));
    }

    const ShaderProgram OpenGLRenderer::create_shader_program(const ShaderSource &shader_source)
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
                throw std::runtime_error(compilation_log);
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
            throw std::runtime_error(compilation_log);
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

    TextureHandle OpenGLRenderer::load_texture(const void *data, int width, int height, int num_channels)
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
};