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
		Vector<float, 3> position;
		Vector<float, 4> color[4];
	};

	struct Mesh
	{
		std::array<Vertex, VERTICES_PER_QUAD> vertices;
	};

	struct ShaderInfo
	{
		// Source code of the vertex shader
		std::string vertex_shader_source;

		// Source code of the fragment shader
		std::string fragment_shader_source;
	};

	using Shader = unsigned int;

	using Viewport = Rect<int>;

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
		virtual void draw(const Mesh& mesh) = 0;

		// Compile a shader source into shader program
		virtual const Shader create_shader(const ShaderInfo& shader_info) = 0;

		// Use a shader a shader program
		virtual void use_shader(const Shader& shader_program) = 0;

		// Load a texture onto the gpu
		virtual TextureHandle load_texture(const void* data, int width, int height, int num_channels) = 0;

		virtual const Viewport& get_viewport() const = 0;

		virtual void set_viewport(const Viewport& viewport) = 0;
	};


	inline std::array<float, 2> screen_to_normalized_device_coordinates(const Viewport& viewport, float x, float y)
	{
		static const auto model = TranslationMatrix(0.f, 0.f, 0.f);
		const auto projection_matrix = OrthographicMatrix(0, viewport.w, 0.f, viewport.h, -1.f, 1.f);
		const auto ndc = (projection_matrix * model) * Matrix<float, 1, 4>{x, y, 0.f, 1.f};
		return {ndc(0, 0), ndc(1, 0)};
	}

	inline void draw_box(Renderer* renderer, float x, float y, float w, float h)
	{
		const auto viewport = renderer->get_viewport();

		const auto top_left = screen_to_normalized_device_coordinates(viewport, x, y);
		const auto top_right = screen_to_normalized_device_coordinates(viewport, x + w, y);
		const auto bottom_left = screen_to_normalized_device_coordinates(viewport, x, y + h);
		const auto bottom_right = screen_to_normalized_device_coordinates(viewport, x + w, y + h);

		std::array<Vertex, VERTICES_PER_QUAD> vertices{
			{
				{{top_left[0], top_left[1], 0.f}, {1, 1, 1, 1}},
				{{top_right[0], top_right[1], 0.f}, {1, 1, 1, 1}},
				{{bottom_right[0], bottom_right[1], 0.f}, {1, 1, 1, 1}},
				{{bottom_left[0], bottom_left[1], 0.f}, {1, 1, 1, 1}},
			}};

		renderer->draw({.vertices = vertices});
	}
	constexpr auto make_quad_vertices = [](float x, float y, float w, float h,
		float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f)
		->std::array<Vertex, VERTICES_PER_QUAD>
	{
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