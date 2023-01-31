#include "orthographic_game_renderer.hpp"

#include <platform/extensions/matrix_extensions.hpp>

namespace lib
{
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

	OrthographicGameRenderer::OrthographicGameRenderer(IRenderer& renderer, IFileSystem& file_system) :
		_renderer(renderer),
		_file_system(file_system)
	{
		_renderer.begin_setup();

		_vbo = _renderer.create_vertex_buffer(_sizeof_vertex_buffer);
		_renderer.bind_vertex_buffer(_vbo);

		_ibo = _renderer.create_index_buffer(_sizeof_index_buffer);
		_renderer.bind_index_buffer(_ibo);

		_shader = _renderer.create_shader(glsl_vertex_shader, glsl_fragment_shader, {
			{TypeId::Float, 2},
			{TypeId::Float, 4},
			{TypeId::Float, 2},
			{TypeId::Float, 2}
			});

		load_texture("./wall.jpg", _atlas_texture);

		_renderer.bind_texture(_atlas_texture);

		_renderer.end_setup();
	}

	void OrthographicGameRenderer::load_texture(const char* img, unsigned int& texture)
	{
		auto image = get_image(_file_system, img);
		texture = _renderer.create_texture(image.data, image.width, image.height);
	}

	#define has_position(x) true
	#define get_position(x) Vector3{}
	#define has_texture(x) true
	#define get_texture(x) unsigned int{};

	void OrthographicGameRenderer::render(const GameState& gs)
	{
		// Begin scene
		{
			_num_indices = 0;
			_num_vertices = 0;
			_num_edges = 0;
			_num_sprites = 0;
		}

		// Scene
		{
			push_sprite_fullscreen(_atlas_texture);

			for(auto& entity : gs.entity_list)
			{
				if(!has_position(entity))
					continue;

				const auto& pos = get_position(entity);

				if(!has_texture(entity))
					continue;

				const auto& texture = get_texture(entity);

				push_sprite(pos.x, pos.y);
			}

			push_sprite(100.f, 100.f);

			push_sprite(300.f, 400.f);
		}

		// Endscene
		{
			_renderer.clear(0.1f, 0.5f, 0.1f, 1.f);
			_renderer.use_shader(_shader);

			_renderer.bind_vertex_buffer(_vbo);
			_renderer.write_vertex_buffer(_vbo, _vertex_buffer, _num_vertices * _sizeof_vertex);

			_renderer.bind_index_buffer(_ibo);
			_renderer.write_index_buffer(_ibo, _index_buffer, _num_indices * _sizeof_index);

			_renderer.draw_indexed(_num_indices);
		}
	}


	inline void OrthographicGameRenderer::push_sprite(
		float x, float y
	)
	{
		const auto scale = 0.5f;
		const auto _window_width = 800;
		const auto _window_height = 800;
		const auto sprite_width = 512 * scale;
		const auto sprite_height = 512 * scale;

		auto tl = project_orthographic(x, y, _window_width, _window_height);
		auto tr = project_orthographic(x + sprite_width, y, _window_width, _window_height);
		auto bl = project_orthographic(x, y + sprite_height, _window_width, _window_height);
		auto br = project_orthographic(x + sprite_width, y + sprite_height, _window_width, _window_height);

		push_sprite(
			{
				tl.x, tl.y,
				tr.x, tr.y,
				bl.x, bl.y,
				br.x, br.y,
			},
			{
				1.f, 1.f, 1.f, 1.f,
			},
			{
				0.f, 0.f,
			},
			{
				512, 512
			});
	}


	inline void OrthographicGameRenderer::push_sprite(
		const float(&position)[8],
		const float(&color)[4],
		const float(&texture_pos)[2],
		const float(&texture_size)[2]
	)
	{
		{
			_vertex_buffer[_num_vertices++] = position[0];
			_vertex_buffer[_num_vertices++] = position[1];
			_vertex_buffer[_num_vertices++] = color[0];
			_vertex_buffer[_num_vertices++] = color[1];
			_vertex_buffer[_num_vertices++] = color[2];
			_vertex_buffer[_num_vertices++] = color[3];
			_vertex_buffer[_num_vertices++] = 1.f;
			_vertex_buffer[_num_vertices++] = 1.f;
			_vertex_buffer[_num_vertices++] = texture_size[0];
			_vertex_buffer[_num_vertices++] = texture_size[1];
		}
		{
			_vertex_buffer[_num_vertices++] = position[2];
			_vertex_buffer[_num_vertices++] = position[3];
			_vertex_buffer[_num_vertices++] = color[0];
			_vertex_buffer[_num_vertices++] = color[1];
			_vertex_buffer[_num_vertices++] = color[2];
			_vertex_buffer[_num_vertices++] = color[3];
			_vertex_buffer[_num_vertices++] = 0.f;
			_vertex_buffer[_num_vertices++] = 1.f;
			_vertex_buffer[_num_vertices++] = texture_size[0];
			_vertex_buffer[_num_vertices++] = texture_size[1];
		}
		{
			_vertex_buffer[_num_vertices++] = position[4];
			_vertex_buffer[_num_vertices++] = position[5];
			_vertex_buffer[_num_vertices++] = color[0];
			_vertex_buffer[_num_vertices++] = color[1];
			_vertex_buffer[_num_vertices++] = color[2];
			_vertex_buffer[_num_vertices++] = color[3];
			_vertex_buffer[_num_vertices++] = 1.0f;
			_vertex_buffer[_num_vertices++] = 0.0f;
			_vertex_buffer[_num_vertices++] = texture_size[0];
			_vertex_buffer[_num_vertices++] = texture_size[1];
		}
		{
			_vertex_buffer[_num_vertices++] = position[6];
			_vertex_buffer[_num_vertices++] = position[7];
			_vertex_buffer[_num_vertices++] = color[0];
			_vertex_buffer[_num_vertices++] = color[1];
			_vertex_buffer[_num_vertices++] = color[2];
			_vertex_buffer[_num_vertices++] = color[3];
			_vertex_buffer[_num_vertices++] = 0.0f;
			_vertex_buffer[_num_vertices++] = 0.0f;
			_vertex_buffer[_num_vertices++] = texture_size[0];
			_vertex_buffer[_num_vertices++] = texture_size[1];
		}

		_index_buffer[_num_indices + 0] = _num_edges + 0;
		_index_buffer[_num_indices + 1] = _num_edges + 1;
		_index_buffer[_num_indices + 2] = _num_edges + 3;
		_index_buffer[_num_indices + 3] = _num_edges + 2;
		_index_buffer[_num_indices + 4] = _num_edges + 3;
		_index_buffer[_num_indices + 5] = _num_edges + 0;

		_num_sprites++;
		_num_indices += 6;
		_num_edges += 4;
	}


	void OrthographicGameRenderer::push_sprite_fullscreen(TextureHandle)
	{
		push_sprite(
			{
				-1.0f, -1.0f,
				-1.0f, +1.0f,
				+1.0f, -1.0f,
				+1.0f, +1.0f
			},
			{
				1.f, 1.f, 0.f, 0.2f
			},
			{
				1.f, 1.f,
			},
			{
				512.f, 512.f
			}
			);
	}
}