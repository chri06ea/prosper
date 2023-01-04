#pragma once

/*
 * OpenGL renderer implementation
 */

#include "platform.hpp"

namespace prosper
{
	class OpenGL : public RenderDevice
	{
	public:
		OpenGL();

		virtual VAO create_vao() override;

		virtual void bind_vao(VAO vao) override;

		virtual VBO create_vertex_buffer(size_t size, const GPUBufferOptions& options = {}) override;

		virtual EBO create_index_buffer(size_t size, const GPUBufferOptions& options = {}) override;

		virtual void bind_buffer(GPUBufferType type, GPUBufferHandle buffer) override;

		virtual void unbind_buffer(GPUBufferType type) override;

		virtual void write_buffer(GPUBufferType type, GPUBufferHandle buffer, const Buffer& data) override;

		virtual void draw_elements(GPUElementType type, size_t indices_count) override;

		virtual const Viewport& get_viewport() const override;

		virtual void set_viewport(const Viewport& viewport) override;

		virtual const Shader create_shader(const ShaderSource& shader_source) override;

		virtual void use_shader(const Shader& shader) override;

		virtual GPUTextureHandle load_texture(const Buffer& data, int width, int height, int num_channels) override;

	private:
		Viewport _viewport{};
		
		struct VertexAttributeData
		{
			GPUTypeId type_id;
			size_t num_types;
		};

		std::unordered_map<size_t, VertexAttributeData> _vertex_attributes;

		// Inherited via RenderDevice
		virtual void set_vertex_attribute(size_t index, GPUTypeId type_id, size_t num_types) override;
	};
}
