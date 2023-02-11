#include "opengl_renderer.hpp"

#include <platform/thirdparty/glad/glad.h>

#ifndef RELEASE
// Wrapper for doing opengl calls, which will also print error (if any)
#define GL_CALL(CALL)                                  \
    CALL;                                              \
    if (auto glErr = glGetError(); glErr)              \
    {                                                  \
        std::cout << #CALL << " Error: " << glErr << std::endl; \
    }
#endif

auto _typeid_glid = [](TypeId id)
{
	switch(id)
	{
		case TypeId::Float:
		{
			return GL_FLOAT;
		}
		default: CRITICAL_ERROR("Invalid element type");
	};
};

namespace lib
{
	OpenGLRenderer::OpenGLRenderer()
	{
		if(!gladLoadGL())
			CRITICAL_ERROR("Glad init failed");

		GL_CALL(glGenVertexArrays(1, &_vao));
	}

	void OpenGLRenderer::begin_setup()
	{
		GL_CALL(glBindVertexArray(_vao));
	}

	void OpenGLRenderer::end_setup()
	{
		GL_CALL(glBindVertexArray(0));
	}

	unsigned int OpenGLRenderer::create_vertex_buffer(size_t size, bool constant, const void* initial_data)
	{
		VBO vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, initial_data, constant ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
		return vbo;
	}

	void OpenGLRenderer::write_vertex_buffer(unsigned int vbo, const void* data, size_t data_size)
	{
		GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizei) data_size, data));
	}

	void OpenGLRenderer::bind_vertex_buffer(unsigned int vbo)
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	}

	unsigned int OpenGLRenderer::create_index_buffer(size_t size, bool constant, const void* initial_data)
	{
		EBO ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, initial_data, constant ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
		return ibo;
	}

	void OpenGLRenderer::write_index_buffer(unsigned int ibo, const void* data, size_t data_size)
	{
		GL_CALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, (GLsizei) data_size, data));
	}

	void OpenGLRenderer::bind_index_buffer(unsigned int ibo)
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	}

	ShaderHandle OpenGLRenderer::create_shader(const String& vertex_shader_source, const String& fragment_shader_source, const ShaderAttributes& attributes)
	{
		auto _compile_shader = [](std::string_view shader_source, std::string_view shader_type)
		{
			// Translate shader type string to opengl shader type id
			int shader_type_id{};
			if(shader_type == "vertex")
				shader_type_id = GL_VERTEX_SHADER;
			else if(shader_type == "fragment")
				shader_type_id = GL_FRAGMENT_SHADER;
			else
				CRITICAL_ERROR("Invalid shader type");
			// Compile that shit
			const auto source = shader_source.data();
			const auto shader = glCreateShader(shader_type_id);
			GL_CALL(glShaderSource(shader, 1, &source, NULL));
			GL_CALL(glCompileShader(shader));
			// Error checking
			int compilation_status;
			char compilation_log[512];
			GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_status));
			if(!compilation_status)
			{
				GL_CALL(glGetShaderInfoLog(shader, 512, NULL, compilation_log));
				CRITICAL_ERROR(compilation_log);
			};
			return shader;
		};
		// Compile shaders
		const auto vertex_shader = _compile_shader(vertex_shader_source, "vertex");
		const auto fragment_shader = _compile_shader(fragment_shader_source, "fragment");
		// Link shaders into shader program
		auto program = glCreateProgram();
		GL_CALL(glAttachShader(program, vertex_shader));
		GL_CALL(glAttachShader(program, fragment_shader));
		GL_CALL(glLinkProgram(program));
		// Error checking
		int compilation_status;
		char compilation_log[512];
		GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &compilation_status));
		if(!compilation_status)
		{
			GL_CALL(glGetProgramInfoLog(program, 512, NULL, compilation_log));
			CRITICAL_ERROR(compilation_log);
		}
		// delete the shaders as they're linked into our program now and no longer necessary
		GL_CALL(glDeleteShader(vertex_shader));
		GL_CALL(glDeleteShader(fragment_shader));

		setup_shader_attributes(attributes);

		return {program};
	}

	void OpenGLRenderer::setup_shader_attributes(const lib::ShaderAttributes& attributes)
	{
		size_t stride{};

		for(const auto& attribute : attributes)
		{
			if(!attribute.count)
				break;

			stride += typeid_size(attribute.type) * attribute.count;
		}

		size_t offset{}, index{};

		for(const auto& attribute : attributes)
		{
			if(!attribute.count)
				break;

			GL_CALL(glEnableVertexAttribArray(index));
			GL_CALL(glVertexAttribPointer(index, (GLuint) attribute.count, _typeid_glid(attribute.type), GL_FALSE, (GLuint) stride, (const void*) offset));

			offset += attribute.count * typeid_size(attribute.type);
			index++;
		}
	}

	void OpenGLRenderer::use_shader(ShaderHandle shader)
	{
		glUseProgram(shader);
	}

	TextureHandle OpenGLRenderer::create_texture(const void* data, int width, int height)
	{
		// Generate and bind texture
		unsigned int texture;
		GL_CALL(glGenTextures(1, &texture));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));
		// set the texture wrapping/filtering options (on the currently bound texture object)
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		// generate the texture (and mipmap)
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
		GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
		return texture;
	}

	void OpenGLRenderer::bind_texture(TextureHandle texture_handle)
	{
		GL_CALL(glBindTexture(GL_TEXTURE_2D, texture_handle));

	}

	void OpenGLRenderer::draw_indexed(size_t indices_count)
	{
		//Move when more than 1 vao needed
		glBindVertexArray(_vao);

		GL_CALL(glDrawElements(GL_TRIANGLES, (GLsizei) indices_count, GL_UNSIGNED_INT, 0));
	}

	void OpenGLRenderer::set_viewport(const Viewport& viewport)
	{
		GL_CALL(glViewport(viewport.x, viewport.y, viewport.w, viewport.h));
	}

	void OpenGLRenderer::clear(float r, float g, float b, float a)
	{
		GL_CALL(glClearColor(r, g, b, a));

		glClear(GL_COLOR_BUFFER_BIT);
	}
}