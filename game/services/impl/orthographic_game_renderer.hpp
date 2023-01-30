#pragma once

#include "../game_renderer.hpp"

#include <platform/services/renderer.hpp>
#include <platform/services/filesystem.hpp>

#include <platform/extensions/filesystem_extensions.hpp>

#include <game/models/gamestate.hpp>

namespace lib
{
	class OrthographicGameRenderer : public IGameRenderer
	{
	public:
		OrthographicGameRenderer(
			IRenderer& renderer,
			IFileSystem& file_system
		);

		virtual void render(const GameState&) override;

	private:
		IRenderer& _renderer;
		IFileSystem& _file_system;

		void push_sprite_fullscreen(TextureHandle);

		void push_sprite(float x, float y);

		void push_sprite(const float(&position)[8], const float(&color)[4], const float(&texture_pos)[2], const float(&texture_size)[2]);

		VAO _vao{};
		VBO _vbo{};
		EBO _ibo{};
		ShaderHandle _shader{};
		size_t _num_vertices{}, _num_indices{}, _num_sprites{}, _num_edges{};
		TextureHandle _background_texture{};

		using VertexType = float;
		using IndexType = int32_t;
		static constexpr auto _sizeof_vertex = sizeof(VertexType);
		static constexpr auto _sizeof_index = sizeof(IndexType);
		static constexpr auto _max_vertices = 0x1000;
		static constexpr auto _max_indices = 0x1000;
		static constexpr auto _sizeof_vertex_buffer = _sizeof_vertex * _max_vertices;
		static constexpr auto _sizeof_index_buffer = _sizeof_index * _max_indices;

		float _vertex_buffer[_max_vertices]{};
		int _index_buffer[_max_indices]{};
	};
}