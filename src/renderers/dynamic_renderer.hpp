#pragma once

#include "renderer_base.hpp"


namespace prosper {
	// renders some dynamic rendering data
	struct BasicDynamicRenderer : RendererBase
	{
		VAO _vao{};
		VBO _vbo{};
		EBO _ebo{};

		inline static float vertex_data[]{

			// Triangle 1
			-0.25f,  -0.25f,
			-0.25f,   0.25f,
			 0.25f,  -0.25f,
		};

		inline static int index_data[]{
			// Triangle 1
			0, 1, 2,
		};

		BasicDynamicRenderer(RenderDevice* render_device, Allocator& allocator) : RendererBase(render_device, allocator)
		{
			_vao = dev->create_vao();

			_vbo = dev->create_vertex_buffer(sizeof(vertex_data));

			_ebo = dev->create_index_buffer(sizeof(index_data));

			dev->set_vertex_attribute(0, GPUTypeId::Float, 2);
		}

		void render()
		{
			dev->bind_vao(_vao);
			vertex_data[0] -= 0.001f;
			vertex_data[1] -= 0.001f;
			dev->write_buffer(GPUBufferType::VBO, _vbo, vertex_data);
			dev->write_buffer(GPUBufferType::EBO, _vbo, index_data);
			dev->draw_elements(GPUElementType::Triangle, sizeof(index_data) / sizeof(decltype(*index_data)));
		}
	};
};