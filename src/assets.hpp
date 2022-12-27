#pragma once

// GLSL sprite renderer vertex shader
static inline const char *spr_vs = "#version 330 core\n"
                                   "layout (location = 0) in vec3 aPos;\n"
                                   "layout (location = 1) in vec3 aColor;\n"
                                   "layout (location = 2) in vec2 aTexCoord;\n"
                                   "\n"
                                   "out vec3 ourColor;\n"
                                   "out vec2 TexCoord;\n"
                                   "\n"
                                   "void main()\n"
                                   "{\n"
                                   "	gl_Position = vec4(aPos, 1.0);\n"
                                   "	ourColor = aColor;\n"
                                   "	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
                                   "}\n";

// GLSL sprite renderer fragment shader
static inline const char *spr_fs = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "\n"
                                   "in vec3 ourColor;\n"
                                   "in vec2 TexCoord;\n"
                                   "\n"
                                   "// texture sampler\n"
                                   "uniform sampler2D Texture;\n"
                                   "\n"
                                   "void main()\n"
                                   "{\n"
                                   "	FragColor = texture(Texture, TexCoord) * vec4(ourColor, 0.2);  \n"
                                   "}\n";
