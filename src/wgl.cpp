#include "wgl.hpp"

#include <stdexcept>

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

#define GL_TRUE 1

namespace prosper
{
    HGLRC(__stdcall *wglCreateContextAttribsARB)
    (HDC hdc, HGLRC hShareContext, const int *attribList);

    BOOL(__stdcall *wglChoosePixelFormatARB)
    (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

    void init_wgl_function_pointers()
    {

        // This is dumb but.. in order to initialize OpenGL context properly, we need an actual OpenGL context,
        // otherwise, wglGetProcAddress will return null pointers..
        // And in order to create a opengl context, we need to set a pixel format, which can only be done ONCE per window.
        // Therefore we create a dummy window

        static constexpr auto window_title = "_wgl_init_window";
        static constexpr auto window_class_name = "_wgl_init_window";

        // acquire handle/base address of this process
        const auto app_instance = GetModuleHandle(0);

        // register a window class. this is needed to create the window
        WNDCLASSEXA window_class{
            .cbSize = sizeof(WNDCLASSEXA),
            .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
            .lpfnWndProc = DefWindowProcA,
            .hInstance = app_instance,
            .hCursor = LoadCursor(0, IDC_ARROW),
            .lpszClassName = window_class_name};

        if (!RegisterClassExA(&window_class))
            throw std::runtime_error("failed registering window class");

        // create the window
        auto window_handle = CreateWindowExA(0, window_class_name, window_title,
                                             WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                             1200, 800, 0, 0, app_instance, 0);

        if (window_handle == NULL)
            throw std::runtime_error("failed creating window");

        // acquire device context for the window
        auto device_context = GetDC(window_handle);

        // setup whatever pixel format desc
        PIXELFORMATDESCRIPTOR pixel_format_desc = {
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

        // chose and set pixel format
        int pixel_format = ChoosePixelFormat(device_context, &pixel_format_desc);

        if (!pixel_format)
            throw std::runtime_error("ChoosePixelFormat failed");

        if (!SetPixelFormat(device_context, pixel_format, &pixel_format_desc))
            throw std::runtime_error("SetPixelFormat failed");

        // create rendering context
        HGLRC rendering_context = wglCreateContext(device_context);

        if (!rendering_context)
            throw std::runtime_error("wglCreateContext failed");

        // set rendering context
        if (!wglMakeCurrent(device_context, rendering_context))
            throw std::runtime_error("wglMakeCurrent failed");

        // initialize the function pointers
        wglCreateContextAttribsARB = (decltype(wglCreateContextAttribsARB))wglGetProcAddress("wglCreateContextAttribsARB");
        if (!wglCreateContextAttribsARB)
            throw std::runtime_error("failed obtaining wglCreateContextAttribsARB ptr");

        wglChoosePixelFormatARB = (decltype(wglChoosePixelFormatARB))wglGetProcAddress("wglChoosePixelFormatARB");
        if (!wglChoosePixelFormatARB)
            throw std::runtime_error("failed obtaining wglCreateContextAttribsARB ptr");

        // cleanup. now that we have the pointers, we can destroy everything
        if (!wglMakeCurrent(device_context, 0))
            throw std::runtime_error("failed wglMakeCurrent");

        if (!wglDeleteContext(rendering_context))
            throw std::runtime_error("failed wglDeleteContext");

        if (!ReleaseDC(window_handle, device_context))
            throw std::runtime_error("failed ReleaseDC");

        if (!DestroyWindow(window_handle))
            throw std::runtime_error("failed DestroyWindow");
    }

    void init_opengl_rendering_context(HDC device_context)
    {
        // make sure function pointers are initialized
        if (!wglCreateContextAttribsARB || wglChoosePixelFormatARB)
            init_wgl_function_pointers();

        // use wgl to set up pixel format attributes
        int pixel_format_attributes[] = {
            WGL_DRAW_TO_WINDOW_ARB, 1,
            WGL_SUPPORT_OPENGL_ARB, 1,
            WGL_DOUBLE_BUFFER_ARB, 1,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, 32,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,
            0};

        int pixel_format;
        UINT num_formats;
        if (!wglChoosePixelFormatARB(device_context, pixel_format_attributes, 0, 1, &pixel_format, &num_formats))
            throw std::runtime_error("failed wglChoosePixelFormatARB");

        // then use those attributes to get a pixel format descriptor
        PIXELFORMATDESCRIPTOR pixel_format_desc;
        if (!DescribePixelFormat(device_context, pixel_format, sizeof(pixel_format_desc), &pixel_format_desc))
            throw std::runtime_error("failed DescribePixelFormat");

        // now set pixel format
        if (!SetPixelFormat(device_context, pixel_format, &pixel_format_desc))
            throw std::runtime_error("failed SetPixelFormat");

        int gl_attributes[] = {
            // major version
            WGL_CONTEXT_MAJOR_VERSION_ARB,
            3,
            // minor version
            WGL_CONTEXT_MINOR_VERSION_ARB,
            3,
            // drawing mode. can either be CORE PROFILE or immediate mode.
            // immediate mode if you wanna do stuff the old way 'glBegin glEnd'
            // core profile is the new thing (batching, glDrawElements, glDrawArrays), but doesnt support the above
            WGL_CONTEXT_PROFILE_MASK_ARB,
            WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
            // null terminator
            0,
        };

        // create the rendering context
        auto opengl_rendering_context = wglCreateContextAttribsARB(device_context, 0, gl_attributes);

        if (!opengl_rendering_context)
            throw std::runtime_error("failed wglCreateContextAttribsARB");

        // set the rendering context
        if (!wglMakeCurrent(device_context, opengl_rendering_context))
            throw std::runtime_error("failed wglMakeCurrent");
    }
}
