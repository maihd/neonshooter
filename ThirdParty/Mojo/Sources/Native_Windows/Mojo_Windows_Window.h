#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#undef MessageBox
#undef min
#undef max

#pragma comment(lib, "GDI32.lib")
#pragma comment(lib, "OpenGL32.lib")

inline namespace Mojo
{
    namespace Window
    {
        extern HWND  _mainWindow;
        extern HDC   _mainDevice;
        extern HGLRC _mainContext;
        
        extern int   _windowFlags;
        extern int   _windowedWidth;
        extern int   _windowedHeight;
    }
}