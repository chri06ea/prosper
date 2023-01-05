#pragma once

#include "renderer_base.hpp"


#define UNIT_TO_NDC_COORD(x) -1.f + x * 2

namespace prosper
{
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

			auto unit_to_ndc_coordinates = []()
			{

			};

			auto draw_simple_triangle_at_center = [&]()
			{
				data.push_vertex(+0.25f, -0.25f);
				data.push_vertex(-0.25f, -0.25f);
				data.push_vertex(-0.25f, +0.25f);

				data.push_indices(0, 1, 2);
			};

			auto draw_at_normalized_world_coordinate = [&](float x0, float y0, float x1, float y1)
			{
				data.push_vertex(x0, y0);
				data.push_vertex(x0, y1);
				data.push_vertex(x1, y1);

				data.push_indices(0, 1, 2);
			};

			auto draw_triangle_at_ndc_simplified = [&](float x, float y, float w, float h)
			{
				const auto x0 = -1.f + x * 2;
				const auto y0 = -1.f + y * 2;
				const auto nw = w * 2;
				const auto nh = h * 2;
				const auto x1 = x0 + nw;
				const auto y1 = y0 + nh;

				data.push_vertex(x0, y0);
				data.push_vertex(x0, y1);
				data.push_vertex(x1, y1);

				data.push_indices(0, 1, 2);
			};

			auto draw_quad = [&](float x, float y, float w, float h)
			{
				const auto nx0 = -1.f + x * 2;
				const auto ny0 = -1.f + y * 2;
				const auto nw = w * 2;
				const auto nh = h * 2;
				const auto nx1 = nx0 + nw;
				const auto ny1 = ny0 + nh;

				data.push_vertex(nx0, ny0);
				data.push_vertex(nx0, ny1);
				data.push_vertex(nx1, ny1);
				data.push_vertex(nx1, ny0);

				data.push_indices(0, 1, 2);
				data.push_indices(2, 3, 0);
			};


			dev->bind_vao(_vao);
			dev->write_buffer(GPUBufferType::VBO, _vbo, data.vertex_data);
			dev->write_buffer(GPUBufferType::EBO, _ebo, data.index_data);
			dev->draw_elements(GPUElementType::Triangle, data.index_data.size);

			data.clear();
		}
	};
};