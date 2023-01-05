#pragma once

#include "renderer_base.hpp"

namespace prosper {


	struct BasicQuadRenderer : RendererBase
	{
		static constexpr auto MAX_QUADS = 10000;

		static constexpr auto VERTICES_PER_QUAD = 4u;
		static constexpr auto INDICES_PER_QUAD = 6u;

		VAO _vao{};
		VBO _vbo{};
		EBO _ebo{};

		BasicQuadRenderer(RenderDevice* render_device, Allocator& allocator) : RendererBase(render_device, allocator)
		{
			_vao = dev->create_vao();
			_vbo = dev->create_vertex_buffer(MAX_QUADS * VERTICES_PER_QUAD * sizeof(float));
			_ebo = dev->create_index_buffer(MAX_QUADS * INDICES_PER_QUAD * sizeof(unsigned int));

			dev->set_vertex_attribute(0, GPUTypeId::Float, 2);
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
			//data.push_vertex(tl.x(), tl.y()); // options.color_modulation);
			//data.push_vertex(tr.x(), tr.y()); // options.color_modulation);
			//data.push_vertex(bl.x(), bl.y()); // options.color_modulation);
			//data.push_vertex(br.x(), br.y()); // options.color_modulation);

			data.push_vertex(-1.00f,-1.00f); // options.color_modulation);
			data.push_vertex(+1.00f,-1.00f); // options.color_modulation);
			data.push_vertex(-1.00f,+1.00f); // options.color_modulation);
			data.push_vertex(+1.00f,+1.00f); // options.color_modulation);


			// Push index data
			const auto index_offset = 0;
			data.push_indices(0 + index_offset, 1 + index_offset, 2 + index_offset);
			data.push_indices(2 + index_offset, 3 + index_offset, 0 + index_offset);
		}

		// Render our data
		void render()
		{
			std::array<float, 30> fs;

			memcpy(fs.data(), data.vertex_data, data.vertex_data.size);

			dev->bind_vao(_vao);
			dev->write_buffer(GPUBufferType::VBO, _vbo, data.vertex_data);
			dev->write_buffer(GPUBufferType::EBO, _ebo, data.index_data);
			dev->draw_elements(GPUElementType::Triangle, 7);

			// Clear the cpu-side buffers
			data.clear();
		}
	};
}