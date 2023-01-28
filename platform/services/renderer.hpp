#pragma once

#include <platform/core/defs.hpp>
#include <platform/models/viewport.hpp>

namespace lib
{
	struct RenderData
	{
		List<float> vertices;
		List<int> indices;
	};

	class IRenderer
	{
	public:

		virtual VAO create_vao() = 0;

		virtual void bind_vao(VAO vao) = 0;

		virtual VBO create_vertex_buffer(size_t size, const GPUBufferOptions& options = {}) = 0;

		virtual EBO create_index_buffer(size_t size, const GPUBufferOptions& options = {}) = 0;

		virtual void bind_buffer(GPUBufferType type, GPUBufferHandle gpu_buffer_handle) = 0;

		virtual void unbind_buffer(GPUBufferType type) = 0;

		virtual void write_buffer(GPUBufferType type, GPUBufferHandle gpu_buffer_handle, const void* buffer, size_t size) = 0;

		virtual void draw_elements(GPUElementType type, size_t indices_count) = 0;

		virtual const Viewport& get_viewport() const = 0;

		virtual void set_viewport(const Viewport&) = 0;

		virtual const Shader create_shader(const String& vertex_shader_source, const String& fragment_shader_source) = 0;

		virtual void use_shader(const Shader& shader) = 0;

		virtual GPUTextureHandle load_texture(const void* data, int width, int height, int num_channels) = 0;

		virtual void bind_texture(GPUTextureHandle background_texture) = 0;

		virtual void set_vertex_attribute(size_t index, GPUTypeId type_id, size_t num_types) = 0;

		virtual void clear(float r, float g, float b, float a) = 0;
	};

	DECLARE_ABSTRACT_FACTORY(IRenderer)
}
