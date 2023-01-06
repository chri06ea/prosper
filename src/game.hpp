#pragma once

/*
 * Game logic
 */

#include <platform/platform.hpp>

namespace prosper
{

	struct GameState
	{
		size_t tick_count = 0ull;

		float ticks_per_second = 64.f;

		static constexpr auto world_dimensions = 1000.f;

		size_t game_simulation_count{};

		float game_time{};
		float game_time_step = 1.f;

		float last_event_time{};
		float event_cooldown = 5.f;
	};

	struct Entity
	{
		Vector3f position{100.f, 100.f, 0.f};

		virtual void update(GameState& gamestate) = 0;
	};

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

		float move_speed = 1.f;

		virtual void update(GameState& gamestate) override
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
					if(gamestate.game_simulation_count % (int)gamestate.ticks_per_second == 0)
						LOG_INFORMATION("Visiting.. Leaving in " << visit_end_time - gamestate.game_time << " seconds.");

					if(visit_end_time <= gamestate.game_time)
						state = State::Leaving;

					// Move towards world center. This is where our base is
					target_position = {400,400,0.f};



					break;
				}
				case State::Leaving:
				{
					if(gamestate.game_simulation_count % (int)gamestate.ticks_per_second == 0)
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

			//position = target_position * move_speed;
		}
	};

	class Game
	{
	public:
		// Constructor
		Game(Platform& platform);

		// Run the game
		void run();

		// Renders the game
		void run_rendering();

		// Run a game tick
		void run_simulation();

		// process a window input event
		void on_window_event(WindowEventType event, const WindowEvent& context);


	private:
		Platform& _platform;

		GameState state;
	};
}

