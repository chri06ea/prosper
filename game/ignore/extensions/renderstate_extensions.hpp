#pragma once

#include <platform/models/vector.hpp>
#include <platform/extensions/matrix_extensions.hpp>

namespace lib
{
	auto push_vertex_data(RenderState& rs, float v)
	{
		rs.vertex_buffer[rs._num_vertices++] = v;
	}
	template<typename ...Floats>
	auto push_vertex_data(RenderState& rs, const Floats& ...v)
	{
		(..., (rs.vertex_buffer[rs._num_vertices++] = v));
	}
	auto push_orthographic_screen_coordinate_vertex(RenderState& rs, float x, float y)
	{
		const auto ndc_coordinates = project_orthographic(x, y, rs.viewport_width, rs.viewport_height);
		rs.vertex_buffer[rs._num_vertices++] = ndc_coordinates.x;
		rs.vertex_buffer[rs._num_vertices++] = ndc_coordinates.y;
	}

	inline void push_square(RenderState& rs)
	{
		push_vertex_data(rs, -1.f, -1.f);
		push_vertex_data(rs, -1.f, +1.f);
		push_vertex_data(rs, +1.f, +1.f);
		push_vertex_data(rs, +1.f, -1.f);

		rs.index_buffer[rs._num_indices++] = 0;
		rs.index_buffer[rs._num_indices++] = 1;
		rs.index_buffer[rs._num_indices++] = 2;

		rs.index_buffer[rs._num_indices++] = 2;
		rs.index_buffer[rs._num_indices++] = 3;
		rs.index_buffer[rs._num_indices++] = 0;
	}

	inline void push_sprite_fullscreen(RenderState& rs, int sprite_id)
	{
		push_vertex_data(rs, -1.f, -1.f);
		push_vertex_data(rs, -1.f, +1.f);
		push_vertex_data(rs, +1.f, +1.f);
		push_vertex_data(rs, +1.f, -1.f);

		rs.index_buffer[rs._num_indices++] = 0;
		rs.index_buffer[rs._num_indices++] = 1;
		rs.index_buffer[rs._num_indices++] = 2;

		rs.index_buffer[rs._num_indices++] = 2;
		rs.index_buffer[rs._num_indices++] = 3;
		rs.index_buffer[rs._num_indices++] = 0;
	}

	inline void push_sprite_world(RenderState& rs, int sprite_id, float x, float y)
	{
		const auto w = 100, h = 100;

		push_orthographic_screen_coordinate_vertex(rs, x, y);
		push_orthographic_screen_coordinate_vertex(rs, x + w, y);
		push_orthographic_screen_coordinate_vertex(rs, x + w, y + h);
		push_orthographic_screen_coordinate_vertex(rs, x, y + h);

		rs.index_buffer[rs._num_indices++] = 0;
		rs.index_buffer[rs._num_indices++] = 1;
		rs.index_buffer[rs._num_indices++] = 2;

		rs.index_buffer[rs._num_indices++] = 2;
		rs.index_buffer[rs._num_indices++] = 3;
		rs.index_buffer[rs._num_indices++] = 0;
	}

	inline void push_sprite_world(RenderState& rs, int sprite_id, const Vector3& v)
	{
		push_sprite_world(rs, sprite_id, v.x, v.y);
	}
}