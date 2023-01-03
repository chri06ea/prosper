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

		virtual const ShaderProgram create_shader_program(const ShaderSource& shader_source) override;

		virtual void use_shader_program(const ShaderProgram& shader_program) override;

		virtual TextureHandle load_texture(const void* data, int width, int height, int num_channels) override;

		virtual void on_resize(int width, int height) override;

		virtual size_t get_viewport_width() override;

		virtual size_t get_viewport_height() override;

	private:
		unsigned int _vao,   // holds vertex attributes
			_vbo,            // holds the actual vertex data
			_ebo,            // holds indicies to connect verticies
			_shader_program, // holds the shader
			_viewport_width, _viewport_height; // Viewport dimensions

		// The data to be sent to the gpu
		std::vector<Vertex> _vertices;
	};
}
