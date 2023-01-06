#pragma once

#include <core/core.hpp>

#include "entity.hpp"

namespace prosper
{
	struct TravelingMerchant : Entity
	{
		enum class State
		{
			Initial,
			Visiting,
			Leaving,
		} state;

		Vector<float, 3> target_position{};

		float visit_start_time{};
		float visit_end_time{};
		float move_speed;

		TravelingMerchant();

		virtual void update(GameState& gamestate) override;
	};
}