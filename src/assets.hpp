#pragma once

// GLSL sprite renderer vertex shader
static inline const char *spr_vs = "#version 330 core\n"
                                   "layout (location = 0) in vec3 a_pos;\n"
                                   "layout (location = 1) in vec4 a_color;\n"
                                   //"layout (location = 2) in vec2 aTexCoord;\n"
                                   "out vec4 a_color1;\n"
                                   //"out vec2 TexCoord;\n"
                                   "void main()\n"
                                   "{\n"
                                   "	gl_Position = vec4(a_pos, 1.0);\n"
                                   "	a_color1 = a_color;\n"
                                   //"	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
                                   "}\n";

// GLSL sprite renderer fragment shader
static inline const char *spr_fs = "#version 330 core\n"
                                   "in vec4 a_color1;\n"
                                   "out vec4 FragColor;\n"
                                   //"in vec2 TexCoord;\n"
                                   //"// texture sampler\n"
                                   //"uniform sampler2D Texture;\n"
                                   "void main()\n"
                                   "{\n"
                                   "	FragColor = a_color1;  \n"
                                   "}\n";


constexpr auto spr_vs2 = R"(
#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec4 a_color;

out vec4 a_color_;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    a_color_ = a_color;
    gl_Position = projection * model * vec4(a_pos.xy, 0.0, 1.0);
})";

constexpr auto spr_fs2 = R"(
#version 330 core

in vec4 a_color_;

out vec4 color;

void main()
{    
    color = a_color_;
})";