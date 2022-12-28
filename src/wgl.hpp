#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace prosper
{
    // Initialized wgl function pointers exported by this file

    void init_opengl_rendering_context_with_wgl(HDC device_context);

    extern HGLRC(__stdcall *wglCreateContextAttribsARB)(HDC hdc, HGLRC hShareContext, const int *attribList);

    extern BOOL(__stdcall *wglChoosePixelFormatARB)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
}
