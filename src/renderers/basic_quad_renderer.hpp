#pragma once

#include "renderer_base.hpp"

namespace prosper {


	struct QuadRenderer : RendererBase
	{
		static constexpr auto MAX_QUADS = 10000;

		static constexpr auto VERTICES_PER_QUAD = 4u;
		static constexpr auto INDICES_PER_QUAD = 6u;

		VAO _vao{};
		VBO _vbo{};
		EBO _ebo{};

		QuadRenderer(RenderDevice* render_device, Allocator& allocator) : RendererBase(render_device, allocator)
		{
			_vao = dev->create_vao();
			_vbo = dev->create_vertex_buffer(MAX_QUADS * VERTICES_PER_QUAD * sizeof(float));
			_ebo = dev->create_index_buffer(MAX_QUADS * INDICES_PER_QUAD * sizeof(unsigned int));

			dev->set_vertex_attribute(0, GPUTypeId::Float, 3);
		}

		struct QuadRenderOptions
		{
			Color<float> color_modulation{1.f,1.f,1.f,1.f};
		};

		// Push a quad to be drawn, when the 'render' method is called
		void push(int x, int y, int w, int h, const QuadRenderOptions& options = {})
		{
			// Translate screen coordinates to device coordinates
			const auto tl = screen_to_normalized_device_coordinates(x, y);
			const auto tr = screen_to_normalized_device_coordinates(x + w, y);
			const auto bl = screen_to_normalized_device_coordinates(x, y + h);
			const auto br = screen_to_normalized_device_coordinates(x + w, y + h);

			// Push vertex data
			data.push_vertex(tl.x(), tl.y(), 0.f); // options.color_modulation);
			data.push_vertex(tr.x(), tr.y(), 0.f); // options.color_modulation);
			data.push_vertex(bl.x(), bl.y(), 0.f); // options.color_modulation);
			data.push_vertex(br.x(), br.y(), 0.f); // options.color_modulation);

			// Push index data
			const auto index_offset = data.indices.count * INDICES_PER_QUAD;
			data.push_indices(0 + index_offset, 1 + index_offset, 2 + index_offset);
			data.push_indices(2 + index_offset, 3 + index_offset, 0 + index_offset);
		}

		// Render our data
		void render()
		{
			dev->bind_vao(_vao);
			dev->write_buffer(GPUBufferType::VBO, _vbo, data.vertices);
			dev->write_buffer(GPUBufferType::EBO, _ebo, data.indices);
			dev->draw_elements(GPUElementType::Triangle, data.indices.count);

			// Clear the cpu-side buffers
			data.clear();
		}
	};
}