#pragma once

#include <renderer_base.hpp>

namespace prosper
{
	// renders some static rendering data
	struct BasicRenderer : RendererBase
	{
		VAO _vao{};
		VBO _vbo{};
		EBO _ebo{};

		inline static float vertex_data[]{

			// Triangle 1
			-0.25f,  -0.25f,
			-0.25f,   0.25f,
			 0.25f,  -0.25f,

			 // Triangle 2
			 -0.25f - 0.5f,  -0.25f - 0.5f,
			 -0.25f - 0.5f,   0.25f - 0.5f,
			 +0.25f - 0.5f,  -0.25f - 0.5f,

			 // Triangle 3
			 -0.25f - 0.75f,  -0.25f - 0.75f,
			 -0.25f - 0.75f,   0.25f - 0.75f,
			 +0.25f - 0.75f,  -0.25f - 0.75f,
		};

		inline static int index_data[]{
			// Triangle 1
			0, 1, 2,
			// Triangle 2
			3, 4, 5,
			// Triangle 3
			6, 7, 8
		};

		BasicRenderer(RenderDevice* render_device) : RendererBase(render_device)
		{
			_vao = dev->create_vao();

			_vbo = dev->create_vertex_buffer(sizeof(vertex_data), {
				.constant = true,
				.initial_data = vertex_data
			});

			_ebo = dev->create_index_buffer(sizeof(index_data), {
				.constant = true,
				.initial_data = index_data
			});

			dev->set_vertex_attribute(0, GPUTypeId::Float, 2);
		}

		void render()
		{
			dev->bind_vao(_vao);
			dev->draw_elements(GPUElementType::Triangle, sizeof(index_data) / sizeof(decltype(*index_data)));
		}
	};

	// renders some dynamic rendering data
	struct BasicDynamicRenderer : RendererBase
	{
		VAO _vao{};
		VBO _vbo{};
		EBO _ebo{};

		inline static float vertex_data[]{

			// Triangle 1
			-0.25f,  -0.25f,
			-0.25f,   0.25f,
			 0.25f,  -0.25f,
		};

		inline static int index_data[]{
			// Triangle 1
			0, 1, 2,
		};

		BasicDynamicRenderer(RenderDevice* render_device) : RendererBase(render_device)
		{
			_vao = dev->create_vao();

			_vbo = dev->create_vertex_buffer(sizeof(vertex_data));

			_ebo = dev->create_index_buffer(sizeof(index_data));

			dev->set_vertex_attribute(0, GPUTypeId::Float, 2);
		}

		void render()
		{
			dev->bind_vao(_vao);
			vertex_data[0] -= 0.001f;
			vertex_data[1] -= 0.001f;
			dev->write_buffer(GPUBufferType::VBO, _vbo, vertex_data);
			dev->write_buffer(GPUBufferType::EBO, _vbo, index_data);
			dev->draw_elements(GPUElementType::Triangle, sizeof(index_data) / sizeof(decltype(*index_data)));
		}
	};

	struct QuadRenderer : RendererBase
	{
		static constexpr auto MAX_QUADS = 10000;

		static constexpr auto VERTICES_PER_QUAD = 4u;
		static constexpr auto INDICES_PER_QUAD = 6u;

		VAO _vao{};
		VBO _vbo{};
		EBO _ebo{};

		QuadRenderer(RenderDevice* render_device) : RendererBase(render_device)
		{
			_vao = dev->create_vao();
			_vbo = dev->create_vertex_buffer(MAX_QUADS * VERTICES_PER_QUAD * sizeof(float));
			_ebo = dev->create_index_buffer(MAX_QUADS * INDICES_PER_QUAD * sizeof(unsigned int));

			dev->set_vertex_attribute(0, GPUTypeId::Float, 3);
		}

		struct QuadRenderOptions
		{
			Color<float> color_modulation{1.f,1.f,1.f,1.f};
		};

		// Push a quad to be drawn, when the 'render' method is called
		void push(int x, int y, int w, int h, const QuadRenderOptions& options = {})
		{
			// Translate screen coordinates to device coordinates
			const auto tl = screen_to_normalized_device_coordinates(x, y);
			const auto tr = screen_to_normalized_device_coordinates(x + w, y);
			const auto bl = screen_to_normalized_device_coordinates(x, y + h);
			const auto br = screen_to_normalized_device_coordinates(x + w, y + h);

			// Push vertex data
			data.push_vertex(tl.x(), tl.y(), 0.f); // options.color_modulation);
			data.push_vertex(tr.x(), tr.y(), 0.f); // options.color_modulation);
			data.push_vertex(bl.x(), bl.y(), 0.f); // options.color_modulation);
			data.push_vertex(br.x(), br.y(), 0.f); // options.color_modulation);

			// Push index data
			const auto index_offset = data.indices.count * INDICES_PER_QUAD;
			data.push_indices(0 + index_offset, 1 + index_offset, 2 + index_offset);
			data.push_indices(2 + index_offset, 3 + index_offset, 0 + index_offset);
		}

		// Render our data
		void render()
		{
			dev->bind_vao(_vao);
			dev->write_buffer(GPUBufferType::VBO, _vbo, data.vertices);
			dev->write_buffer(GPUBufferType::EBO, _ebo, data.indices);
			dev->draw_elements(GPUElementType::Triangle, data.indices.count);

			// Clear the cpu-side buffers
			data.clear();
		}
	};
}