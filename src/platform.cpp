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

        class OpenGLRenderer : Renderer
        {
        public:
            virtual void draw_sprite() override final
            {
            }

            // switch to backbuffer
            virtual void swap() override final
            {
                SwapBuffers(game_dc);
            }
        };

        Renderer *
        create_renderer()
        {
            return nullptr;
        }

#endif

    }
}