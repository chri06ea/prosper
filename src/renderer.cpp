#include "renderer.hpp"
#include "core.hpp"
#include <Windows.h>
#include <glad/glad.h> // for opengl types and function ptr initializor.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace prosper
{
    class OpenGLRenderer : public Renderer
    {
    private:
        const char *spr_vs = "#version 330 core\n"
                             "layout (location = 0) in vec3 aPos;\n"
                             "layout (location = 1) in vec3 aColor;\n"
                             "layout (location = 2) in vec2 aTexCoord;\n"
                             "\n"
                             "out vec3 ourColor;\n"
                             "out vec2 TexCoord;\n"
                             "\n"
                             "void main()\n"
                             "{\n"
                             "	gl_Position = vec4(aPos, 1.0);\n"
                             "	ourColor = aColor;\n"
                             "	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
                             "}\n";

        const char *spr_fs = "#version 330 core\n"
                             "out vec4 FragColor;\n"
                             "\n"
                             "in vec3 ourColor;\n"
                             "in vec2 TexCoord;\n"
                             "\n"
                             "// texture sampler\n"
                             "uniform sampler2D Texture;\n"
                             "\n"
                             "void main()\n"
                             "{\n"
                             "	FragColor = texture(Texture, TexCoord) * vec4(ourColor, 0.2);  \n"
                             "}\n";

    public:
        unsigned int _texture, _texture1;

        static unsigned int create_shader_program(const char *vertex_shader_src, const char *fragment_shader_src)
        {
            // compile shaders
            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            // vertex Shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vertex_shader_src, NULL);
            glCompileShader(vertex);
            // print compile errors if any
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                LOG_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << infoLog);
            };

            // fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fragment_shader_src, NULL);
            glCompileShader(fragment);
            // print compile errors if any
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                LOG_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << infoLog);
            };

            // shader Program
            auto ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            // print linking errors if any
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
                LOG_ERROR("ERROR::SHADER::PROGRAM::COMPILATION_FAILED: " << infoLog);
            }

            // delete the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);

            return ID;
        }

        static unsigned int load_texture(std::string_view path)
        {
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            // set the texture wrapping/filtering options (on the currently bound texture object)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // load and generate the texture
            int width, height, nrChannels;
            unsigned char *data = stbi_load(path.data(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                LOG_ERROR("Failed to load texture");
            }
            stbi_image_free(data);

            return texture;
        }
        void init()
        {
            _shaderProgram = create_shader_program(spr_vs, spr_fs);
            _texture = load_texture("./assets/wall.jpg");
            // set up vertex data (and buffer(s)) and configure vertex attributes
            // ------------------------------------------------------------------
            float vertices[] = {
                // positions          // colors           // texture coords
                1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
                1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
                -1.0f, 1.f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // top left
            };
            unsigned int indices[] = {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
            };
            unsigned int VBO, EBO;
            glGenVertexArrays(1, &_vao);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(_vao);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0);
            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            // texture coord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);
        }

        virtual void draw_sprite(const Sprite &sprite) override final
        {
            glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // bind textures on corresponding texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _texture);

            glUseProgram(_shaderProgram);
            glBindVertexArray(_vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // glDrawArrays(GL_TRIANGLES, 0, 6);
            // glBindVertexArray(0); // no need to unbind it every time
        }

        // Sets the projection matrix for the renderer
        virtual void set_projection(const Matrix<float, 4> &projection)
        {
        }

        // Renders all sprites in the renderer
        virtual void render()
        {
        }

    private:
        unsigned int _vao, _shaderProgram;
    };

    Renderer *create_renderer()
    {
        static OpenGLRenderer opengl_renderer;

        opengl_renderer.init();

        return &opengl_renderer;
    }
}