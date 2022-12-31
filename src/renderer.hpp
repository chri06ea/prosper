#pragma once

#include <string>
#include <math/math.hpp>

namespace prosper
{
    // A quad require 4 points (upper left, upper right, bottom left, bottom right)
    const auto VERTICES_PER_QUAD = 4u;

    // Edges needed to connect a quad. Think of this as 'how many pen strokes would i need to draw this IRL'. In computer graphics, everything is triangles. Each triangles requires 3 pen strokes. A quad consists of two triangles. Therefore 6.
    const auto INDICES_PER_QUAD = 6u;

    // Supported renders
    enum class RendererType
    {
        OpenGL,
        // TODO: Vulkan,
    };

    using TextureHandle = unsigned int;

    struct Sprite
    {
    };

    // Data for a single vertex. Passed to the gpu
    struct Vertex
    {
        Vertex()
        {
        }

        Vertex(float x, float y, float z)
        {
            position = {x, y, z};
        }

        FVec3 position;
    };

    struct Mesh
    {
        std::array<Vertex, VERTICES_PER_QUAD> vertices;
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

    // Renderer abstraction.
    class Renderer
    {
    public:
        // Initializes the renderer
        virtual void init() = 0;

        // Signals the beginning of the frame. must be called before drawing
        virtual void begin_frame() = 0;

        // Signals the end of the frame. must be called before swapping to back buffer
        virtual void end_frame() = 0;

        // draw something
        virtual void draw(const Mesh &mesh) = 0;

        // Compile a shader source into shader program
        virtual const ShaderProgram create_shader_program(const ShaderSource &shader_source) = 0;

        // Use a shader a shader program
        virtual void use_shader_program(const ShaderProgram &shader_program) = 0;

        // Load a texture onto the gpu
        virtual TextureHandle load_texture(const void *data, int width, int height, int num_channels) = 0;
    };
}