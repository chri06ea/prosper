#pragma once
#include "../renderer.hpp"
namespace lib
{
	class OpenGLRenderer : public IRenderer
	{
	public:
		OpenGLRenderer();

		virtual void begin_setup() override;
		virtual void end_setup() override;

		virtual unsigned int create_vertex_buffer(size_t size, bool constant = false, const void* initial_data = nullptr) override;
		virtual void write_vertex_buffer(unsigned int vbo, const void* data, size_t data_size) override;
		virtual void bind_vertex_buffer(unsigned int vbo) override;

		virtual unsigned int create_index_buffer(size_t size, bool constant = false, const void* initial_data = nullptr) override;
		virtual void write_index_buffer(unsigned int ibo, const void* data, size_t data_size) override;
		virtual void bind_index_buffer(unsigned int ibo) override;

		virtual ShaderHandle create_shader(const String& vertex_shader_source, const String& fragment_shader_source, const ShaderAttributes& attributes) override;
		void setup_shader_attributes(const lib::ShaderAttributes& attributes);
		virtual void use_shader(ShaderHandle shader) override;

		virtual TextureHandle create_texture(const void* data, int width, int height) override;
		virtual void bind_texture(TextureHandle texture_handle) override;

		virtual void draw_indexed(size_t indices_count) override;

		virtual void set_viewport(const Viewport& viewport) override;

		virtual void clear(float r, float g, float b, float a) override;

	private:
		unsigned int _vao{};
	};
}