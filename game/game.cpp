#include "game.hpp"

#include <game/assets/assets.hpp>

#include <platform/extensions/filesystem_extensions.hpp>
#include <game/extensions/extensions.hpp>
#include <game/extensions/renderstate_extensions.hpp>

namespace lib
{


	Game::Game(IPlatform& platform, IFileSystem& filesystem, IWindow& window, IRenderer& renderer)
		:
		_platform(platform),
		_filesystem(filesystem),
		_window(window),
		_renderer(renderer)
	{

	}

	void Game::main(GameState& gs, EngineState& es, RenderState& rs)
	{
		initialize(es);
		initialize(rs);
		initialize(gs);

		while(true)
		{
			_window.process_messages();

			es.current_tick = _platform.get_tick_count();
			es.time = (float) (es.current_tick - es.start_tick) / (float) es.ticks_per_second;

			if(const auto time_since_simulation = es.time - es.last_simulation_time;
				time_since_simulation >= gs.simulation_interval)
			{
				simulate(gs, es);

			}

			render(gs, es, rs);

			_window.swap_buffers();
		}
	}



	class GameRendererV1 : public IGameRenderer
	{
	public:
		IRenderer& _renderer;

		VAO vao;
		VBO vbo;
		EBO ebo;

		Shader shader;
		GPUTextureHandle background_texture;

		float vertex_buffer[0x1000];
		size_t vertex_buffer_size{};
		size_t num_vertices{};

		int index_buffer[0x1000]{};
		size_t index_buffer_size{};
		size_t num_indices{};

		size_t viewport_width{800}, viewport_height{800};

		IFileSystem& _filesystem;

		GameRendererV1(IRenderer& renderer, IFileSystem& filesystem)
			: _renderer(renderer), _filesystem(filesystem)
		{

		}

		virtual void initialize() override
		{
			vao = _renderer.create_vao();
			_renderer.bind_vao(vao);

			vbo = _renderer.create_vertex_buffer(sizeof(vertex_buffer));
			ebo = _renderer.create_index_buffer(sizeof(index_buffer));

			_renderer.set_vertex_attribute(0, GPUTypeId::Float, 2);
			_renderer.set_vertex_attribute(1, GPUTypeId::Float, 4);
			_renderer.set_vertex_attribute(2, GPUTypeId::Float, 2);
			_renderer.set_vertex_attribute(3, GPUTypeId::Float, 2);

			shader = _renderer.create_shader(glsl_vertex_shader, glsl_fragment_shader);
			_renderer.use_shader(shader);

			auto image = load_image(_filesystem, "C:/Users/workpc/Desktop/prosper/prosper/assets/wall.jpg");

			if(!image) CRITICAL_ERROR("No image");

			background_texture = _renderer.load_texture(image->data, image->width, image->height, image->num_channels);
			_renderer.bind_texture(background_texture);
		}

		virtual void begin_scene(RenderState& rs)
		{
		}

		virtual void end_scene(RenderState& rs)
		{

		}

	};

	void Game::initialize(RenderState& rs)
	{
		// Setup sprite rendering
		rs.vao = _renderer.create_vao();
		_renderer.bind_vao(rs.vao);

		rs.vbo = _renderer.create_vertex_buffer(sizeof(rs.vertex_buffer));
		rs.ebo = _renderer.create_index_buffer(sizeof(rs.index_buffer));

		_renderer.set_vertex_attribute(0, GPUTypeId::Float, 2);
		_renderer.set_vertex_attribute(1, GPUTypeId::Float, 4);
		_renderer.set_vertex_attribute(2, GPUTypeId::Float, 2);
		_renderer.set_vertex_attribute(3, GPUTypeId::Float, 2);

		rs.shader = _renderer.create_shader(glsl_vertex_shader, glsl_fragment_shader);
		_renderer.use_shader(rs.shader);

		auto image = load_image(_filesystem, "C:/Users/workpc/Desktop/prosper/prosper/assets/wall.jpg");

		if(!image)
			CRITICAL_ERROR("No image");

		rs.background_texture = _renderer.load_texture(image->data, image->width, image->height, image->num_channels);
		_renderer.bind_texture(rs.background_texture);
	}

	void Game::initialize(EngineState& es)
	{
		es.ticks_per_second = _platform.get_ticks_per_second();
		es.start_tick = _platform.get_tick_count();
		es.current_tick = es.start_tick;
		es.last_simulation_time = -1.f;
		es.time = {};
	}

	void Game::initialize(GameState& gs)
	{
		gs.simulation_interval = 1.f / 64.f;
	}

	void Game::render(GameState& gs, EngineState& es, RenderState& rs)
	{
		begin_scene(rs);

		push_sprite_fullscreen(rs, rs.background_texture);

		end_scene(rs);
	}

	void Game::begin_scene(RenderState& rs)
	{
		rs.num_indices = 0;
		rs.num_vertices = 0;
	}

	void Game::end_scene(RenderState& rs)
	{
		_renderer.clear(0.1f, 0.5f, 0.1f, 1.f);
		_renderer.bind_vao(rs.vao);
		_renderer.write_buffer(GPUBufferType::VBO, rs.vbo, rs.vertex_buffer, rs.num_vertices * sizeof(*rs.vertex_buffer));
		_renderer.write_buffer(GPUBufferType::EBO, rs.ebo, rs.index_buffer, rs.num_indices * sizeof(*rs.index_buffer));
		_renderer.draw_elements(GPUElementType::Triangle, rs.num_indices);
	}

	void Game::simulate(GameState& gs, EngineState& es)
	{
		for(auto& entity : gs.entity_list)
		{

		}

		//for(Entity entity = 0; entity < gs.MaxEntities; entity++)
		//{
		/*	auto& position = gs.get_component<PositionComponent>(entity);

			if(!gs.has_component<VelocityComponent>(entity))
				continue;

			auto& velocity = gs.get_component<VelocityComponent>(entity);

			if(!gs.has_component<MovespeedComponent>(entity))
			{
				auto& movespeed = gs.get_component<MovespeedComponent>(entity);


				position.x += velocity.x * movespeed;
				position.y += velocity.y * movespeed;
			}


			position.x += 0.1f;
			position.y += 0.1f;*/
			//}

		es.last_simulation_time = es.time;
	}
};