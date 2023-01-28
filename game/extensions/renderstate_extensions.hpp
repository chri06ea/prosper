#pragma once

#include <platform/models/vector.hpp>
#include <platform/extensions/matrix_extensions.hpp>

#include <game/models/renderstate.hpp>

namespace lib
{
	auto push_vertex(RenderState& rs, float v)
	{
		rs.vertex_buffer[rs.num_vertices++] = v;
	}
	template<typename ...Floats>
	auto push_vertex(RenderState& rs, const Floats& ...v)
	{
		(..., (rs.vertex_buffer[rs.num_vertices++] = v));
	}
	auto push_orthographic_screen_coordinate_vertex(RenderState& rs, float x, float y)
	{
		const auto ndc_coordinates = project_orthographic(x, y, rs.viewport_width, rs.viewport_height);
		rs.vertex_buffer[rs.num_vertices++] = ndc_coordinates.x;
		rs.vertex_buffer[rs.num_vertices++] = ndc_coordinates.y;
	}

	inline void push_square(RenderState& rs)
	{
		push_vertex(rs, -1.f, -1.f);
		push_vertex(rs, -1.f, +1.f);
		push_vertex(rs, +1.f, +1.f);
		push_vertex(rs, +1.f, -1.f);

		rs.index_buffer[rs.num_indices++] = 0;
		rs.index_buffer[rs.num_indices++] = 1;
		rs.index_buffer[rs.num_indices++] = 2;

		rs.index_buffer[rs.num_indices++] = 2;
		rs.index_buffer[rs.num_indices++] = 3;
		rs.index_buffer[rs.num_indices++] = 0;
	}

	inline void push_sprite_fullscreen(RenderState& rs, int sprite_id)
	{
		push_vertex(rs, -1.f, -1.f);
		push_vertex(rs, -1.f, +1.f);
		push_vertex(rs, +1.f, +1.f);
		push_vertex(rs, +1.f, -1.f);

		rs.index_buffer[rs.num_indices++] = 0;
		rs.index_buffer[rs.num_indices++] = 1;
		rs.index_buffer[rs.num_indices++] = 2;

		rs.index_buffer[rs.num_indices++] = 2;
		rs.index_buffer[rs.num_indices++] = 3;
		rs.index_buffer[rs.num_indices++] = 0;
	}

	inline void push_sprite_world(RenderState& rs, int sprite_id, float x, float y)
	{
		const auto w = 100, h = 100;

		push_orthographic_screen_coordinate_vertex(rs, x, y);
		push_orthographic_screen_coordinate_vertex(rs, x + w, y);
		push_orthographic_screen_coordinate_vertex(rs, x + w, y + h);
		push_orthographic_screen_coordinate_vertex(rs, x, y + h);

		rs.index_buffer[rs.num_indices++] = 0;
		rs.index_buffer[rs.num_indices++] = 1;
		rs.index_buffer[rs.num_indices++] = 2;

		rs.index_buffer[rs.num_indices++] = 2;
		rs.index_buffer[rs.num_indices++] = 3;
		rs.index_buffer[rs.num_indices++] = 0;
	}

	inline void push_sprite_world(RenderState& rs, int sprite_id, const Vector& v)
	{
		push_sprite_world(rs, sprite_id, v.x, v.y);
	}
}