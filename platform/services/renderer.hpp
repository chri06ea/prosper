#pragma once

#include <platform/core/defs.hpp>
#include <platform/models/viewport.hpp>
#include <platform/models/renderdata.hpp>

namespace lib
{
	enum class GPUBufferType
	{
		Unknown,
		VAO,
		VBO,
		EBO,
	};

	struct GPUBufferOptions
	{
		// Buffer contents stays constant
		bool constant{};

		void* initial_data{};
	};

	using ShaderHandle = unsigned int;
	using BufferHandle = unsigned int;
	using TextureHandle = unsigned int;
	using ObjectHandle = unsigned int;

	using VAO = unsigned int;
	using VBO = unsigned int;
	using EBO = unsigned int;

	struct ShaderAttribute
	{
		TypeId type{};
		size_t count{};
	};

	using ShaderAttributes = ShaderAttribute[16];

	class IRenderer
	{
	public:
	
		virtual void begin_setup() = 0;
		virtual void end_setup() = 0;

		virtual unsigned int create_vertex_buffer(size_t size, bool constant = false, const void* initial_data = nullptr) = 0;
		virtual void write_vertex_buffer(unsigned int vbo, const void* data, size_t data_size) = 0;
		virtual void bind_vertex_buffer(unsigned int vbo) = 0;

		virtual unsigned int create_index_buffer(size_t size, bool constant = false, const void* initial_data = nullptr) = 0;
		virtual void write_index_buffer(unsigned int ibo, const void* data, size_t data_size) = 0;
		virtual void bind_index_buffer(unsigned int ibo) = 0;

		virtual ShaderHandle create_shader(const String& vertex_shader_source, const String& fragment_shader_source, const ShaderAttributes& attributes) = 0;
		virtual void use_shader(ShaderHandle shader) = 0;

		virtual TextureHandle create_texture(const void* data, int width, int height) = 0;
		virtual void bind_texture(TextureHandle texture_handle) = 0;

		virtual void draw_indexed(size_t indices_count) = 0;

		virtual void set_viewport(const Viewport& viewport) = 0;

		virtual void clear(float r, float g, float b, float a) = 0;
	};

	DECLARE_ABSTRACT_FACTORY(IRenderer)
}
