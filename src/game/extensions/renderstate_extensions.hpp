#pragma once

#include <platform/models/vector.hpp>
#include <game/models/renderstate.hpp>

namespace lib
{
	inline void push_sprite_fullscreen(RenderState& rs, int sprite_id)
	{
	}

	inline void push_sprite_world(RenderState& rs, int sprite_id, float x, float y)
	{

	}

	inline void push_sprite_world(RenderState& rs, int sprite_id, const Vector& v)
	{
		push_sprite_world(rs, sprite_id, v.x, v.y);
	}
}