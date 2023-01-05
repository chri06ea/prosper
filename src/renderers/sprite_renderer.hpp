#pragma once

#include <platform/platform.hpp>

#include "renderer_base.hpp"


#define UNIT_TO_NDC_COORD(x) -1.f + x * 2

namespace prosper
{
	struct SpriteRenderer : RendererBase
	{
		VAO _vao{};
		VBO _vbo{};
		EBO _ebo{};
		Shader _shader{};

		static constexpr auto vertex_buffer_size = 0x10000;
		static constexpr auto index_buffer_size = 0x10000;

		static constexpr auto vertices_per_quad = 4u;
		static constexpr auto indices_per_quad = 6u;

		SpriteRenderer(Platform* platform) : RendererBase(platform)
		{
			_vao = dev->create_vao();
			_vbo = dev->create_vertex_buffer(vertex_buffer_size);
			_ebo = dev->create_index_buffer(index_buffer_size);

			{
				size_t attribute_index{};

				dev->set_vertex_attribute(attribute_index++, GPUTypeId::Float, 2);
				dev->set_vertex_attribute(attribute_index++, GPUTypeId::Float, 4);
				dev->set_vertex_attribute(attribute_index++, GPUTypeId::Float, 2);
				dev->set_vertex_attribute(attribute_index++, GPUTypeId::Float, 2);
			}

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
					color = vec4(a_color_); // * texture(image, a_texture_coords_);
                };
			)";


			_shader = dev->create_shader({.vertex_shader_source = glsl_vertex_shader, .fragment_shader_source = glsl_fragment_shader});

			dev->use_shader(_shader);

			//auto texture = dev->load_texture(data, width, height, num_channels);
			//dev->bind_texture(texture);
		}

		size_t _num_sprites_pushed{};

		void push_ndc(
			const Vector<float, 2>& tl,
			const Vector<float, 2>& tr,
			const Vector<float, 2>& bl,
			const Vector<float, 2>& br,
			const Color& color = {0.f,0.f,1.f,1.f},
			const Vector<float, 2>& texture_coords = {0.f,0.f},
			const Vector<float, 2>& texture_size = {0.f,0.f}
		)
		{
			data.push_vertex(tl, color, 1.f, 1.f, texture_size);
			data.push_vertex(tr, color, 1.f, 0.f, texture_size);
			data.push_vertex(br, color, 0.f, 0.f, texture_size);
			data.push_vertex(bl, color, 1.f, 1.f, texture_size);

			const int i = (int)(_num_sprites_pushed * vertices_per_quad);
			data.push_indices(0 + i, 1 + i, 2 + i);
			data.push_indices(2 + i, 3 + i, 0 + i);

			_num_sprites_pushed++;
		}

		void push(int x, int y, int w, int h,
			const Color& color = {0.f,0.f,1.f,1.f})
		{
			// Translate screen coordinates to device coordinates
			const auto tl = screen_to_normalized_device_coordinates(x, y);
			const auto tr = screen_to_normalized_device_coordinates(x + w, y);
			const auto bl = screen_to_normalized_device_coordinates(x, y + h);
			const auto br = screen_to_normalized_device_coordinates(x + w, y + h);

			push_ndc(tl, tr, bl, br, color);
		}

		void render()
		{
			dev->clear(0.1f, 0.1f, 0.1f, 1.f);

			dev->bind_vao(_vao);
			dev->write_buffer(GPUBufferType::VBO, _vbo, data.vertex_data);
			dev->write_buffer(GPUBufferType::EBO, _ebo, data.index_data);
			dev->draw_elements(GPUElementType::Triangle, data.num_indices);

			_num_sprites_pushed = 0;

			data.clear();
		}
	};
};