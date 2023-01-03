#include "game.hpp"

namespace prosper
{

	void process_input(GameState &gamestate, Key key, KeyState keystate)
	{
	}

	void process_rendering(GameState &gamestate, Renderer *renderer)
	{
		renderer->begin_frame();

		// Draw scenary

		// Draw sun

		// Draw ground
		draw_box(renderer, 500, 500, 50, 50);

		// Draw buildings

		// Draw entities

		static float p = 0.f;

		const auto entity_vertices = make_quad_vertices(
			p, 0.1f, 0.05f, 0.05f, // Position
			0.2f, 0.2f, 0.2f, 1.f  // Color
		);

		p += 0.001;

		renderer->draw({.vertices = entity_vertices});

		// Draw overlay (bottom bar, text, popup etc)

		renderer->end_frame();
	}

	void process_tick(GameState &gamestate)
	{
		// Calculate ingame time
		gamestate.simulation_time = (float)(gamestate.platform_tick - gamestate.platform_initial_tick) / (float)gamestate.platform_ticks_per_second;

		// Advance 1 game tick
		gamestate.simulation_count++;
	}
}