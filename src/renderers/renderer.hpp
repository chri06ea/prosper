#pragma once
#pragma once

#include "renderer_base.hpp"

#define UNIT_TO_NDC_COORD(x) -1.f + x * 2

namespace prosper
{
	struct SpriteRenderer : RendererBase
	{
		VAO _vao{};
		VBO _vbo{};
		EBO _ebo{};

		RenderData data;

		static constexpr auto vertex_buffer_size = 0x10000;
		static constexpr auto index_buffer_size = 0x10000;

		static constexpr auto vertices_per_quad = 4u;
		static constexpr auto indices_per_quad = 6u;

		SpriteRenderer(RenderDevice* render_device, Allocator& allocator) : RendererBase(render_device, allocator), data(allocator)
		{
			_vao = dev->create_vao();
			_vbo = dev->create_vertex_buffer(vertex_buffer_size);
			_ebo = dev->create_index_buffer(index_buffer_size);

			dev->set_vertex_attribute(0, GPUTypeId::Float, 2);
		}

		struct Data
		{

		};

		size_t _num_quads_pushed{};

		void push(int x, int y, int w, int h)
		{
			// Translate screen coordinates to device coordinates
			const auto tl = screen_to_normalized_device_coordinates(x, y);
			const auto tr = screen_to_normalized_device_coordinates(x + w, y);
			const auto bl = screen_to_normalized_device_coordinates(x, y + h);
			const auto br = screen_to_normalized_device_coordinates(x + w, y + h);

			data.push_vertex(tl);
			data.push_vertex(tr);
			data.push_vertex(br);
			data.push_vertex(bl);
	
			const int i = _num_quads_pushed * vertices_per_quad;
			data.push_indices(0 + i, 1 + i, 2 + i);
			data.push_indices(2 + i, 3 + i, 0 + i);

			_num_quads_pushed++;
		}

		void render()
		{
			dev->clear(0.1f, 0.1f, 0.1f, 1.f);

			dev->bind_vao(_vao);
			dev->write_buffer(GPUBufferType::VBO, _vbo, data.vertex_data);
			dev->write_buffer(GPUBufferType::EBO, _ebo, data.index_data);
			dev->draw_elements(GPUElementType::Triangle, data.num_indices);

			_num_quads_pushed = 0;

			data.clear();
		}
	};
};