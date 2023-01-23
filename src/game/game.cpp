#include "game.hpp"

#include <platform/extensions/filesystem_extensions.hpp>
#include <game/extensions/extensions.hpp>
#include <game/extensions/renderstate_extensions.hpp>

namespace lib
{
	void Game::initialize(RenderState& rs)
	{
		// Setup sprite rendering
		rs.vao = _renderer.create_vao();
		rs.vbo = _renderer.create_vertex_buffer(sizeof(rs.vertex_buffer));
		rs.ebo = _renderer.create_index_buffer(sizeof(rs.index_buffer));

		_renderer.set_vertex_attribute(0, GPUTypeId::Float, 2);
		_renderer.set_vertex_attribute(1, GPUTypeId::Float, 4);
		_renderer.set_vertex_attribute(2, GPUTypeId::Float, 2);
		_renderer.set_vertex_attribute(3, GPUTypeId::Float, 2);

		const char* glsl_vertex_shader = R"(
				#version 330 core
                layout (location = 0) in vec2 a_pos;
                layout (location = 1) in vec4 a_color;
                layout (location = 2) in vec2 a_texture_coords;
                layout (location = 3) in vec2 a_texture_size;
				out vec4 a_color_;
				out vec2 a_texture_coords_;
                void main()
                {
					a_color_ = a_color;
					a_texture_coords_ = a_texture_coords;

					gl_Position = vec4(a_pos, 0.0, 1.0);
                };
			)";

		const char* glsl_fragment_shader = R"(
				#version 330 core
				in vec4 a_color_;
				in vec2 a_texture_coords_;
				out vec4 color;
				uniform sampler2D image;
                void main()
                {
					color = vec4(a_color_) * texture(image, a_texture_coords_);
                };
			)";


		rs.shader = _renderer.create_shader(glsl_vertex_shader, glsl_fragment_shader);
		_renderer.use_shader(rs.shader);

		auto image = load_image(_filesystem, "./assets/wall.jpg");

		if(!image)
			CRITICAL_ERROR("No image");

		rs.texture = _renderer.load_texture(image->data, image->width, image->height, image->num_channels);
		_renderer.bind_texture(rs.texture);
	}

	void Game::initialize(EngineState& es)
	{
		es.ticks_per_second = _platform.get_ticks_per_second();
		es.start_tick = _platform.get_tick_count();
		es.current_tick = es.start_tick;
		es.last_simulation_time = -1.f;
		es.time = {};
	}

	void Game::render(EngineState& es, GameState& gs, RenderState& rs)
	{
		push_sprite_fullscreen(rs, rs.texture);

		for(Entity entity = 0; entity < gs.MaxEntities; entity++)
		{
			if(!gs.has_component<PositionComponent>(entity))
				continue;

			auto& position = gs.get_component<PositionComponent>(entity);

			push_sprite_world(rs, rs.texture, position.x, position.y);
		}

		_renderer.clear(0.1f, 0.1f, 0.1f, 1.f);
		_renderer.bind_vao(rs.vao);
		_renderer.write_buffer(GPUBufferType::VBO, rs.vbo, rs.vertex_buffer, rs.vertex_buffer_size);
		_renderer.write_buffer(GPUBufferType::EBO, rs.ebo, rs.index_buffer, rs.index_buffer_size);
		_renderer.draw_elements(GPUElementType::Triangle, rs.num_indices);
	}

	void Game::simulate(EngineState& es, GameState& gs)
	{
		for(Entity entity = 0; entity < gs.MaxEntities; entity++)
		{
			if(!gs.has_component<PositionComponent>(entity))
				continue;

			if(!gs.has_component<VelocityComponent>(entity))
				continue;

			if(!gs.has_component<MovespeedComponent>(entity))
			{
				auto& movespeed = gs.get_component<MovespeedComponent>(entity);
			}

			auto& position = gs.get_component<PositionComponent>(entity);
			auto& velocity = gs.get_component<VelocityComponent>(entity);
		}
	}

	void Game::update(GameState& gs, EngineState& es, RenderState& rs)
	{
		es.current_tick = _platform.get_tick_count();
		es.time = (float) (es.current_tick - es.start_tick) / (float) es.ticks_per_second;

		if(const auto time_since_simulation = es.time - es.last_simulation_time;
			time_since_simulation >= gs.simulation_interval)
		{
			simulate(es, gs);

			es.last_simulation_time = es.time;
		}

		render(es, gs, rs);
	}
};