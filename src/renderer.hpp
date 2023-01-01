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
        constexpr Vertex()
        {
        }

        constexpr Vertex(float x, float y, float z, float r, float g, float b, float a)
        {
            position = {x, y, z};

            color[0] = r;
            color[1] = g;
            color[2] = b;
            color[3] = a;
        }

        FVec3 position;
        float color[4];
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

        //
        virtual void on_resize(int width, int height) = 0;
    };

    // @brief Helper function to generate vertex data for a quad
    // Coordinate range is [0, 1], eg generate_quad_vertices(0, 0, 1, 1, ...) for fullscreen
    constexpr auto generate_quad_vertices = [](float x, float y, float w, float h,
                                               float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f)
        -> std::array<Vertex, VERTICES_PER_QUAD>
    {
        // TODO aspect ratio ?
        // Remap coordinates from 0->1 to -1->1
        const auto nx0 = -1.f + x * 2;
        const auto ny0 = -1.f + y * 2;
        const auto nw = w * 2;
        const auto nh = h * 2;
        const auto nx1 = nx0 + nw;
        const auto ny1 = ny0 + nh;
        return {
            {
                {nx0, ny0, 0.f, r, g, b, a},
                {nx0, ny1, 0.f, r, g, b, a},
                {nx1, ny1, 0.f, r, g, b, a},
                {nx1, ny0, 0.f, r, g, b, a},
            }};
    };
}