#pragma once

#include <core/core.hpp>
#include <span>

namespace prosper
{
	using VAO = unsigned int;
	using VBO = unsigned int;
	using EBO = unsigned int;

	struct ShaderSource
	{
		std::string vertex_shader_source;

		std::string fragment_shader_source;
	};

	using Shader = unsigned int;

	struct Viewport
	{
		int x, y, w, h;
	};

	enum class GPUBufferType
	{
		Unknown,
		VAO,
		VBO,
		EBO,
	};

	enum class GPUTypeId
	{
		Float
	};

	struct GPUBufferOptions
	{
		// Buffer contents stays constant
		bool constant{};

		void* initial_data{};
	};

	using GPUBufferHandle = unsigned int;
	using GPUTextureHandle = unsigned int;

	enum class GPUElementType
	{
		Triangle
	};

	struct RenderDevice
	{
		// Generate a vertex array object
		virtual VAO create_vao() = 0;
		virtual void bind_vao(VAO vao) = 0;

		// Generate a vertex buffer. Buffer will be bound
		virtual VBO create_vertex_buffer(size_t size, const GPUBufferOptions& options = {}) = 0;

		// Generate a element buffer object (index buffer)
		virtual EBO create_index_buffer(size_t size, const GPUBufferOptions& options = {}) = 0;

		// Bind a buffer.
		virtual void bind_buffer(GPUBufferType type, GPUBufferHandle gpu_buffer_handle) = 0;

		// Unbind a buffer.
		virtual void unbind_buffer(GPUBufferType type) = 0;

		// Write data to a buffer
		virtual void write_buffer(GPUBufferType type, GPUBufferHandle gpu_buffer_handle, const Buffer& data) = 0;

		// Draw elements
		virtual void draw_elements(GPUElementType type, size_t indices_count) = 0;

		// Access the viewport
		virtual const Viewport& get_viewport() const = 0;

		// Set the viewport
		virtual void set_viewport(const Viewport&) = 0;

		// Compile a shader source into shader program
		virtual const Shader create_shader(const ShaderSource& shader_source) = 0;

		// Use a shader a shader program
		virtual void use_shader(const Shader& shader) = 0;

		// Load a texture onto the gpu
		virtual GPUTextureHandle load_texture(const void* data, int width, int height, int num_channels) = 0;

		virtual void bind_texture(GPUTextureHandle texture) = 0;

		// Set information about a vertex attribute / shader parameter
		virtual void set_vertex_attribute(size_t index, GPUTypeId type_id, size_t num_types) = 0;

		// Clears the viewport
		virtual void clear(float r, float g, float b, float a) = 0;
	};
};