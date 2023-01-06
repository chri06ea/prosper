#include "opengl.hpp"

#include <thirdparty/glad/glad.h>

// Wrapper for doing opengl calls, which will also print error (if any)
#define GL_CALL(CALL)                                  \
    CALL;                                              \
    if (auto glErr = glGetError(); glErr)              \
    {                                                  \
        LOG_INFORMATION(#CALL << " Error: " << glErr); \
    }

namespace prosper
{
	OpenGL::OpenGL()
	{
		if(!gladLoadGL())
			CRITICAL_ERROR("Glad init failed");
	}

	VAO OpenGL::create_vao()
	{
		VAO vao;
		GL_CALL(glGenVertexArrays(1, &vao));
		bind_vao(vao);
		return vao;
	}

	void OpenGL::bind_vao(VAO vao)
	{
		GL_CALL(glBindVertexArray(vao));
	}

	VBO OpenGL::create_vertex_buffer(size_t size, const GPUBufferOptions& options)
	{
		VBO vbo;
		GL_CALL(glGenBuffers(1, &vbo));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, options.initial_data, options.constant ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		return vbo;
	}

	EBO OpenGL::create_index_buffer(size_t size, const GPUBufferOptions& options)
	{
		EBO ebo;
		GL_CALL(glGenBuffers(1, &ebo));
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
		GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, options.initial_data, options.constant ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		return ebo;
	}

	void OpenGL::bind_buffer(GPUBufferType type, GPUBufferHandle buffer)
	{
		uint16_t gl_buffer_type{};

		switch(type)
		{
			case GPUBufferType::VBO:
			{
				gl_buffer_type = GL_ARRAY_BUFFER;
				break;
			}
			case GPUBufferType::EBO:
			{
				gl_buffer_type = GL_ELEMENT_ARRAY_BUFFER;
				break;
			}
			default: CRITICAL_ERROR("Invalid buffer type");
		}

		GL_CALL(glBindBuffer(gl_buffer_type, buffer));
	}

	void OpenGL::unbind_buffer(GPUBufferType type)
	{
		bind_buffer(type, 0);
	}

	void OpenGL::write_buffer(GPUBufferType type, GPUBufferHandle buffer, const Buffer& data)
	{
		uint16_t gl_buffer_type{};

		switch(type)
		{
			case GPUBufferType::VBO:
			{
				gl_buffer_type = GL_ARRAY_BUFFER;
				break;
			}
			case GPUBufferType::EBO:
			{
				gl_buffer_type = GL_ELEMENT_ARRAY_BUFFER;
				break;
			}
			default: CRITICAL_ERROR("Invalid buffer type");
		};

		GL_CALL(glBufferSubData(gl_buffer_type, 0, (GLsizei) data.size, data));
	}

	void OpenGL::draw_elements(GPUElementType type, size_t indices_count)
	{
		uint16_t gl_element_type{};

		switch(type)
		{
			case GPUElementType::Triangle:
			{
				gl_element_type = GL_TRIANGLES;
				break;
			}
			default: CRITICAL_ERROR("Invalid element type");
		};

		GL_CALL(glDrawElements(gl_element_type, (GLsizei) indices_count, GL_UNSIGNED_INT, 0));
	}

	const Viewport& OpenGL::get_viewport() const
	{
		return _viewport;
	}
	void OpenGL::set_viewport(const Viewport& viewport)
	{
		_viewport = viewport;

		GL_CALL(glViewport(viewport.x, viewport.y, viewport.w, viewport.h));

	}

	const Shader OpenGL::create_shader(const ShaderSource& shader_source)
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
		const auto vertex_shader = _compile_shader(shader_source.vertex_shader_source, "vertex");
		const auto fragment_shader = _compile_shader(shader_source.fragment_shader_source, "fragment");
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
		return {program};
	}

	void OpenGL::use_shader(const Shader& shader)
	{
		glUseProgram(shader);
	}

	GPUTextureHandle OpenGL::load_texture(const void* data, int width, int height, int num_channels)
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

	void OpenGL::set_vertex_attribute(size_t index, GPUTypeId type_id, size_t num_types)
	{
		_vertex_attributes[index] = {type_id, num_types};

		// Calculate stride
		size_t stride{};

		for(auto index = 0u; index < _vertex_attributes.size(); index++)
		{
			const auto& vertex_attribute = _vertex_attributes[index];

			size_t type_size{};

			switch(vertex_attribute.type_id)
			{
				case GPUTypeId::Float:
				{
					type_size = sizeof(float);
					break;
				}
				default: CRITICAL_ERROR("Invalid GPUTypeId");
			};

			stride += type_size * vertex_attribute.num_types;
		}

		// Set all stored attributes

		size_t offset{};

		for(auto index = 0u; index < _vertex_attributes.size(); index++)
		{
			const auto& vertex_attribute = _vertex_attributes[index];

			uint16_t gl_type_id{};
			size_t type_size{};

			switch(vertex_attribute.type_id)
			{
				case GPUTypeId::Float:
				{
					gl_type_id = GL_FLOAT;
					type_size = sizeof(float);
					break;
				}
				default: CRITICAL_ERROR("Invalid element type");
			};

			GL_CALL(glEnableVertexAttribArray(index));
			GL_CALL(glVertexAttribPointer(index, (GLuint) vertex_attribute.num_types, gl_type_id, GL_FALSE, (GLuint) stride, (const void*) offset));

			offset += vertex_attribute.num_types * type_size;
		}
	}

	void OpenGL::clear(float r, float g, float b, float a)
	{
		GL_CALL(glClearColor(r, g, b, a));

		glClear(GL_COLOR_BUFFER_BIT);
	}
	void OpenGL::bind_texture(GPUTextureHandle texture)
	{
		GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));
	}
};