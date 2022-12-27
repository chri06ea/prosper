#include "renderer.hpp"
#include "core.hpp"
#include <Windows.h>
#include <glad/glad.h> // for opengl types and function ptr initializor.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "opengl.hpp"

namespace prosper
{
    Renderer *create_renderer()
    {
        static OpenGLRenderer opengl_renderer;

        opengl_renderer.init();

        return &opengl_renderer;
    }
}