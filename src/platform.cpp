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

        const char *vertexShaderSource = "#version 330 core\n"
                                         "layout (location = 0) in vec3 aPos;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                         "}\0";
        const char *fragmentShaderSource = "#version 330 core\n"
                                           "out vec4 FragColor;\n"
                                           "void main()\n"
                                           "{\n"
                                           "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                           "}\n\0";

        class OpenGLRenderer : public Renderer
        {
        public:
            void initialize()
            {
                // build and compile our shader program
                // ------------------------------------
                // vertex shader
                unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
                glCompileShader(vertexShader);
                // check for shader compile errors
                int success;
                char infoLog[512];
                glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                              << infoLog << std::endl;
                }
                // fragment shader
                unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
                glCompileShader(fragmentShader);
                // check for shader compile errors
                glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                              << infoLog << std::endl;
                }
                // link shaders
                shaderProgram = glCreateProgram();
                glAttachShader(shaderProgram, vertexShader);
                glAttachShader(shaderProgram, fragmentShader);
                glLinkProgram(shaderProgram);
                // check for linking errors
                glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                              << infoLog << std::endl;
                }
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);

                // set up vertex data (and buffer(s)) and configure vertex attributes
                // ------------------------------------------------------------------
                float vertices[] = {
                    -0.5f, -0.5f, 0.0f, // left
                    0.5f, -0.5f, 0.0f,  // right
                    0.0f, 0.5f, 0.0f    // top
                };

                unsigned int VBO;
                glGenVertexArrays(1, &VAO);
                glGenBuffers(1, &VBO);
                // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
                glBindVertexArray(VAO);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
                glEnableVertexAttribArray(0);

                // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
                // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
                glBindVertexArray(0);
            }

            virtual void draw_sprite() override final
            {
                glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // render
                // ------
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // draw our first triangle
                glUseProgram(shaderProgram);
                glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }

            // switch to backbuffer
            virtual void swap() override final
            {
                SwapBuffers(game_dc);
            }

        private:
            unsigned int shaderProgram, VAO;
        };

        Renderer *
        create_renderer()
        {
            static OpenGLRenderer opengl_renderer;

            opengl_renderer.initialize();

            return &opengl_renderer;
        }

#endif

    }
}