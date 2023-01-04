#pragma once

#include "renderer_base.hpp"

namespace prosper
{
	// renders some static rendering data
	struct BasicRenderer : RendererBase
	{
		VAO _vao{};
		VBO _vbo{};
		EBO _ebo{};

		inline static float vertex_data[]{

			// Triangle 1
			-0.25f,  -0.25f,
			-0.25f,   0.25f,
			 0.25f,  -0.25f,

			 // Triangle 2
			 -0.25f - 0.5f,  -0.25f - 0.5f,
			 -0.25f - 0.5f,   0.25f - 0.5f,
			 +0.25f - 0.5f,  -0.25f - 0.5f,

			 // Triangle 3
			 -0.25f - 0.75f,  -0.25f - 0.75f,
			 -0.25f - 0.75f,   0.25f - 0.75f,
			 +0.25f - 0.75f,  -0.25f - 0.75f,
		};

		inline static int index_data[]{
			// Triangle 1
			0, 1, 2,
			// Triangle 2
			3, 4, 5,
			// Triangle 3
			6, 7, 8
		};

		BasicRenderer(RenderDevice* render_device, Allocator& allocator) : RendererBase(render_device, allocator)
		{
			_vao = dev->create_vao();

			_vbo = dev->create_vertex_buffer(sizeof(vertex_data), {
				.constant = true,
				.initial_data = vertex_data
			});

			_ebo = dev->create_index_buffer(sizeof(index_data), {
				.constant = true,
				.initial_data = index_data
			});

			dev->set_vertex_attribute(0, GPUTypeId::Float, 2);
		}

		void render()
		{
			dev->bind_vao(_vao);
			dev->draw_elements(GPUElementType::Triangle, sizeof(index_data) / sizeof(decltype(*index_data)));
		}
	};
};