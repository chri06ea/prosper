#pragma once

#include <platform/core/defs.hpp>

namespace lib
{
	struct RenderState
	{
		List<float> vertices;
		List<float> indices;
		/*VAO _vao;
		VBO _vbo;
		EBO _ebo;

		ShaderHandle _shader;
		TextureHandle _background_texture;*/

		//float vertex_buffer[0x1000];
		//size_t vertex_buffer_size{};
		//size_t _num_vertices{};

		//int index_buffer[0x1000]{};
		//size_t index_buffer_size{};
		//size_t _num_indices{};

		//size_t viewport_width{800}, viewport_height{800};
	};
}