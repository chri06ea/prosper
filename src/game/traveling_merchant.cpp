#include "traveling_merchant.hpp"

#pragma once

#include <core/core.hpp>

#include "entity.hpp"

#define baseclass Entity::

namespace prosper
{
	TravelingMerchant::TravelingMerchant()
	{
		move_speed = 1.f;
	}

	void TravelingMerchant::update(GameState& gamestate)
	{
		switch(state)
		{
			case State::Initial:
			{
				LOG_INFORMATION("Traveling merchant is visiting");

				// Traveling merchant just entered the map. Go visit our camp for awhile
				state = State::Visiting;

				visit_start_time = gamestate.game_time;

				visit_end_time = visit_start_time + 10.f;

				break;
			}
			case State::Visiting:
			{
				if(gamestate.game_simulation_count % (int) gamestate.ticks_per_second == 0)
					LOG_INFORMATION("Visiting.. Leaving in " << visit_end_time - gamestate.game_time << " seconds.");

				if(visit_end_time <= gamestate.game_time)
					state = State::Leaving;

				// Move towards world center. This is where our base is
				target_position = {400,400,0.f};

				break;
			}
			case State::Leaving:
			{
				if(gamestate.game_simulation_count % (int) gamestate.ticks_per_second == 0)
					LOG_INFORMATION("Leaving");
				target_position = {0,0,0.f};

				break;
			}
		}

		// Move
		const auto delta = target_position - position;
		const auto length = delta.length();
		if(length > 2.f)
		{
			const auto direction = delta.normalized();
			position += direction * move_speed;
		}
	}

}