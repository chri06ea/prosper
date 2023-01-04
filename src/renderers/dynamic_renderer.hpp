#pragma once

#include "renderer_base.hpp"


namespace prosper {
	// renders some dynamic rendering data
	struct BasicDynamicRenderer : RendererBase
	{
		VAO _vao{};
		VBO _vbo{};
		EBO _ebo{};

		RenderData data;

		static constexpr auto vertex_buffer_size = 0x10000;
		static constexpr auto index_buffer_size = 0x10000;

		BasicDynamicRenderer(RenderDevice* render_device, Allocator& allocator) : RendererBase(render_device, allocator), data(allocator)
		{
			_vao = dev->create_vao();
			_vbo = dev->create_vertex_buffer(vertex_buffer_size);
			_ebo = dev->create_index_buffer(index_buffer_size);

			dev->set_vertex_attribute(0, GPUTypeId::Float, 2);
		}

		void render()
		{
			dev->clear(0.1f, 0.1f, 0.1f, 1.f);

			for(float i{}; i < 5.f; i++)
			{
				const float offset = (i * 0.3f) + -1.f;

				data.push_vertex(+0.25f + offset, -0.25f + offset);
				data.push_vertex(-0.25f + offset, -0.25f + offset);
				data.push_vertex(-0.25f + offset, +0.25f + offset);

				const auto index_offset = (int) i * 3;
				data.push_indices(0 + index_offset, 1 + index_offset, 2 + index_offset);
			}

			dev->bind_vao(_vao);
			dev->write_buffer(GPUBufferType::VBO, _vbo, data.vertices);
			dev->write_buffer(GPUBufferType::EBO, _ebo, data.indices);
			dev->draw_elements(GPUElementType::Triangle, data.indices.size);

			data.clear();
		}
	};
};