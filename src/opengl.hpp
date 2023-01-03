#pragma once

/*
 * OpenGL renderer implementation
 */

#include "platform.hpp"

namespace prosper
{
	class OpenGLRenderer : public Renderer
	{
	public:
		virtual void init() override;

		virtual void begin_frame() override;

		virtual void end_frame() override;

		virtual void draw(const Mesh& mesh) override;

		virtual const Shader create_shader(const ShaderInfo& shader_info) override;

		virtual void use_shader(const Shader& shader) override;

		virtual TextureHandle load_texture(const void* data, int width, int height, int num_channels) override;

		virtual void set_viewport(const Viewport& viewport) override;

		virtual const Viewport& get_viewport() const override;

	private:
		unsigned int _vao,   // holds vertex attributes
			_vbo,            // holds the actual vertex data
			_ebo,            // holds indicies to connect verticies
			_shader; // holds the shader


		// The data to be sent to the gpu
		std::vector<Vertex> _vertices;

		Viewport _viewport{};

	};
}
