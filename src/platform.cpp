#include "platform.hpp"

#ifdef WIN32
#include <Windows.h>
// WGL Constants (used for initializing opengl)
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001          // Core-profile mode
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002 // Immediate mode
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_TYPE_RGBA_ARB 0x202B
#endif

//! NOTE THIS INCLUDE BREAKS INTELLISE IN VSCODE DUE TO SHEER AMOUNT OF DEFINES
//! MUST BE INCLUDED AFTER WINDOWS
#include <glad/glad.h> // for opengl types and function ptr initializor.

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace prosper
{
    namespace platform
    {
#ifdef WIN32
        /// @brief call when an unrecoverable error happens. will terminate the process
        /// @param message
        [[noreturn]] void emergency_shutdown(std::string_view message)
        {
            MessageBoxA(NULL, message.data(), "Emergency shutdown", MB_OK);

            exit(-1);
        }
        // Platform globals
        HWND game_window_handle{};
        HDC game_dc;
        HGLRC game_gldc{};

        // Win32 window callback
        LRESULT CALLBACK
        window_proc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
        {
            LRESULT result{};

            switch (msg)
            {
            case WM_CLOSE:
            case WM_DESTROY:
                PostQuitMessage(0);
                break;
            default:
                result = DefWindowProcA(window, msg, wparam, lparam);
                break;
            }

            return result;
        }

        /// @brief Initialize the platform layer
        /// @return true if successfully initialized
        bool initialize_platform()
        {
            /*
             * Create game window
             */
            {
                // Setup and create game window class
                WNDCLASSEXA game_window_class{
                    .cbSize = sizeof(WNDCLASSEXA),
                    .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
                    .lpfnWndProc = window_proc,
                    .hInstance = GetModuleHandle(0),
                    .hCursor = LoadCursor(0, IDC_ARROW),
                    .lpszClassName = "Prosper"};

                if (!RegisterClassExA(&game_window_class))
                {
                    return false;
                }

                // Create game window
                game_window_handle = CreateWindowExA(0, game_window_class.lpszClassName, "Prosper",
                                                     WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                                     1200, 800, 0, 0, GetModuleHandle(0), 0);

                if (game_window_handle == NULL)
                {
                    return false;
                }
            }

            /*
             * Initialize opengl for window

             */
            {
                // OpenGL context can only be intialized once per window, and we need an OpenGL(wgl) function pointers
                // in order to discover pixel format to initialize our opengl context properly. Therefore we create a dummy window first

                WNDCLASSEXA window_class{
                    .cbSize = sizeof(WNDCLASSEXA),
                    .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
                    .lpfnWndProc = DefWindowProc,
                    .hInstance = GetModuleHandle(0),
                    .hCursor = LoadCursor(0, IDC_ARROW),
                    .lpszClassName = "_opengl_dummy_window"};

                if (!RegisterClassExA(&window_class))
                    return false;

                HWND dummy_window = CreateWindowExA(0, window_class.lpszClassName,
                                                    "_opengl_dummy_window", 0,
                                                    CW_USEDEFAULT, CW_USEDEFAULT,
                                                    CW_USEDEFAULT, CW_USEDEFAULT,
                                                    0, 0, window_class.hInstance, 0);

                if (!dummy_window)
                {
                    return false;
                }

                // Acquire context and initialize opengl context for dummy window

                HDC dummy_dc = GetDC(dummy_window);

                PIXELFORMATDESCRIPTOR dummy_pfd = {
                    .nSize = sizeof(PIXELFORMATDESCRIPTOR),
                    .nVersion = 1,
                    .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
                    .iPixelType = PFD_TYPE_RGBA,
                    .cColorBits = 32,
                    .cAlphaBits = 8,
                    .cDepthBits = 24,
                    .cStencilBits = 8,
                    .iLayerType = PFD_MAIN_PLANE,
                };

                int dummy_pixel_format = ChoosePixelFormat(dummy_dc, &dummy_pfd);

                if (!dummy_pixel_format)
                {
                    return false;
                }

                if (!SetPixelFormat(dummy_dc, dummy_pixel_format, &dummy_pfd))
                {
                    return false;
                }

                HGLRC dummy_context = wglCreateContext(dummy_dc);

                if (!dummy_context)
                {
                    return false;
                }

                if (!wglMakeCurrent(dummy_dc, dummy_context))
                {
                    return false;
                }

                // Obtain wgl function pointers

                HGLRC(__stdcall * wglCreateContextAttribsARB)
                (HDC hdc, HGLRC hShareContext, const int *attribList);
                wglCreateContextAttribsARB = (decltype(wglCreateContextAttribsARB))wglGetProcAddress("wglCreateContextAttribsARB");

                if (!wglCreateContextAttribsARB)
                {
                    emergency_shutdown("failed obtaining wglCreateContextAttribsARB ptr");
                }

                BOOL(__stdcall * wglChoosePixelFormatARB)
                (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
                wglChoosePixelFormatARB = (decltype(wglChoosePixelFormatARB))wglGetProcAddress("wglChoosePixelFormatARB");

                if (!wglCreateContextAttribsARB)
                {
                    emergency_shutdown("failed obtaining wglChoosePixelFormatARB ptr");
                }

                // Destroy and cleanup dummy window

                wglMakeCurrent(dummy_dc, 0);
                wglDeleteContext(dummy_context);
                ReleaseDC(dummy_window, dummy_dc);
                DestroyWindow(dummy_window);

                // Now, initialize opengl context for game window
                game_dc = GetDC(game_window_handle);

                int pixel_format_attributes[] = {
                    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
                    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                    WGL_COLOR_BITS_ARB, 32,
                    WGL_DEPTH_BITS_ARB, 24,
                    WGL_STENCIL_BITS_ARB, 8,
                    0};

                int pixel_format;
                UINT num_formats;

                if (!wglChoosePixelFormatARB(game_dc, pixel_format_attributes, 0, 1, &pixel_format, &num_formats))
                {
                    return false;
                }

                if (!num_formats)
                {
                    return false;
                }

                PIXELFORMATDESCRIPTOR pfd;
                DescribePixelFormat(game_dc, pixel_format, sizeof(pfd), &pfd);

                if (!SetPixelFormat(game_dc, pixel_format, &pfd))
                {
                    return false;
                }

                int gl33_attribs[] = {
                    WGL_CONTEXT_MAJOR_VERSION_ARB,
                    3,
                    WGL_CONTEXT_MINOR_VERSION_ARB,
                    3,
                    WGL_CONTEXT_PROFILE_MASK_ARB,

                    // Swap this one to select immediate / core profile drawing
                    WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                    0,
                };

                game_gldc = wglCreateContextAttribsARB(game_dc, 0, gl33_attribs);

                if (!game_gldc)
                {
                    return false;
                }

                if (!wglMakeCurrent(game_dc, game_gldc))
                {
                    return false;
                }
            }

            /*
             * Show window
             */
            {
                ShowWindow(game_window_handle, 1);
            }

            /*
             * Initialize opengl function pointers
             */

            if (!gladLoadGL())
            {
                return false;
            }

            return true;
        }

        /// @brief process user input (keyboard, mouse, controller, whatever). non blocking
        void process_input()
        {
            MSG msg = {};
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        class OpenGLRenderer : public Renderer
        {
        private:
            const char *spr_vs = "#version 330 core\n"
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

            const char *spr_fs = "#version 330 core\n"
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

        public:
            unsigned int _texture, _texture1;

            static unsigned int create_shader_program(const char *vertex_shader_src, const char *fragment_shader_src)
            {
                // compile shaders
                unsigned int vertex, fragment;
                int success;
                char infoLog[512];

                // vertex Shader
                vertex = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vertex, 1, &vertex_shader_src, NULL);
                glCompileShader(vertex);
                // print compile errors if any
                glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                              << infoLog << std::endl;
                };

                // fragment Shader
                fragment = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(fragment, 1, &fragment_shader_src, NULL);
                glCompileShader(fragment);
                // print compile errors if any
                glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                    std::cout << "ERROR::SHADER::fragment::COMPILATION_FAILED\n"
                              << infoLog << std::endl;
                };

                // shader Program
                auto ID = glCreateProgram();
                glAttachShader(ID, vertex);
                glAttachShader(ID, fragment);
                glLinkProgram(ID);
                // print linking errors if any
                glGetProgramiv(ID, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(ID, 512, NULL, infoLog);
                    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                              << infoLog << std::endl;
                }

                // delete the shaders as they're linked into our program now and no longer necessary
                glDeleteShader(vertex);
                glDeleteShader(fragment);

                return ID;
            }

            static unsigned int load_texture(std::string_view path)
            {
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                // set the texture wrapping/filtering options (on the currently bound texture object)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // load and generate the texture
                int width, height, nrChannels;
                unsigned char *data = stbi_load(path.data(), &width, &height, &nrChannels, 0);
                if (data)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                else
                {
                    std::cout << "Failed to load texture" << std::endl;
                }
                stbi_image_free(data);

                return texture;
            }
            void init()
            {
                _shaderProgram = create_shader_program(spr_vs, spr_fs);
                _texture = load_texture("./assets/wall.jpg");
                // set up vertex data (and buffer(s)) and configure vertex attributes
                // ------------------------------------------------------------------
                float vertices[] = {
                    // positions          // colors           // texture coords
                    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
                    1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
                    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
                    -1.0f, 1.f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // top left
                };
                unsigned int indices[] = {
                    0, 1, 3, // first triangle
                    1, 2, 3  // second triangle
                };
                unsigned int VBO, EBO;
                glGenVertexArrays(1, &_vao);
                glGenBuffers(1, &VBO);
                glGenBuffers(1, &EBO);

                glBindVertexArray(_vao);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

                // position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
                glEnableVertexAttribArray(0);
                // color attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                // texture coord attribute
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
                glEnableVertexAttribArray(2);
            }

            virtual void draw_sprite() override final
            {
                glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // bind textures on corresponding texture units
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, _texture);

                glUseProgram(_shaderProgram);
                glBindVertexArray(_vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // glDrawArrays(GL_TRIANGLES, 0, 6);
                // glBindVertexArray(0); // no need to unbind it every time
            }

            // switch to backbuffer
            virtual void swap() override final
            {
                SwapBuffers(game_dc);
            }

        private:
            unsigned int _vao, _shaderProgram;
        };

        Renderer *
        create_renderer()
        {
            static OpenGLRenderer opengl_renderer;

            opengl_renderer.init();

            return &opengl_renderer;
        }

#endif
    }
}