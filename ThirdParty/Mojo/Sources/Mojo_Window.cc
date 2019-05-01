#include <Mojo/GL.h>
#include <Mojo/Window.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#include <GL/glew.h>
#include <GL/wglew.h>

#include "./Mojo_Input.h"

#pragma comment(lib, "GDI32.lib")
#pragma comment(lib, "OpenGL32.lib")

// Allow compilation with old Windows SDK. MinGW doesn't have default _WIN32_WINNT/WINVER versions.
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef DBT_DEVNODES_CHANGED
#define DBT_DEVNODES_CHANGED 0x0007
#endif

inline namespace Mojo
{
    inline namespace Window_Variables
    {
        static HWND  _mainWindow;
        static HDC   _mainDevice;
        static HGLRC _mainContext;

        constexpr const char* WINDOW_CLASS = "__mojo_window__";
        constexpr const char* DUMMY_WINDOW = "__dummy_window__";

        static KeyCode s_keyCodeMap[2048];
        static KeyCode ConvertKeyCode(int nativeKey)
        {
            static bool s_loaded;
            if (s_loaded)
            {
                return s_keyCodeMap[nativeKey];
            }
            else
            {
                s_loaded = true;

                s_keyCodeMap[VK_SPACE] = KeyCode::Space;
                s_keyCodeMap[VK_BACK] = KeyCode::Backspace;
                s_keyCodeMap[VK_RETURN] = KeyCode::Return;
                s_keyCodeMap[VK_TAB] = KeyCode::Tab;

                s_keyCodeMap[VK_UP] = KeyCode::UpArrow;
                s_keyCodeMap[VK_DOWN] = KeyCode::DownArrow;
                s_keyCodeMap[VK_LEFT] = KeyCode::LeftArrow;
                s_keyCodeMap[VK_RIGHT] = KeyCode::RightArrow;

                s_keyCodeMap[VK_HOME] = KeyCode::Home;
                s_keyCodeMap[VK_END] = KeyCode::End;
                s_keyCodeMap[VK_PRIOR] = KeyCode::PageUp;
                s_keyCodeMap[VK_NEXT] = KeyCode::PageDown;
                s_keyCodeMap[VK_INSERT] = KeyCode::Insert;
                s_keyCodeMap[VK_DELETE] = KeyCode::Delete;

                s_keyCodeMap[VK_LCONTROL] = KeyCode::LeftControl;
                s_keyCodeMap[VK_RCONTROL] = KeyCode::RightControl;

                s_keyCodeMap[VK_LSHIFT] = KeyCode::LeftShift;
                s_keyCodeMap[VK_RSHIFT] = KeyCode::RightShift;

                s_keyCodeMap[VK_MULTIPLY] = KeyCode::NumpadMultiply;
                s_keyCodeMap[VK_DIVIDE] = KeyCode::NumpadDivide;
                s_keyCodeMap[VK_ADD] = KeyCode::NumpadAdd;
                s_keyCodeMap[VK_SUBTRACT] = KeyCode::NumpadSubtract;
                s_keyCodeMap[VK_NUMPAD0] = KeyCode::Numpad0;
                s_keyCodeMap[VK_NUMPAD1] = KeyCode::Numpad1;
                s_keyCodeMap[VK_NUMPAD2] = KeyCode::Numpad2;
                s_keyCodeMap[VK_NUMPAD3] = KeyCode::Numpad3;
                s_keyCodeMap[VK_NUMPAD4] = KeyCode::Numpad4;
                s_keyCodeMap[VK_NUMPAD5] = KeyCode::Numpad5;
                s_keyCodeMap[VK_NUMPAD6] = KeyCode::Numpad6;
                s_keyCodeMap[VK_NUMPAD7] = KeyCode::Numpad7;
                s_keyCodeMap[VK_NUMPAD8] = KeyCode::Numpad8;
                s_keyCodeMap[VK_NUMPAD9] = KeyCode::Numpad9;

                s_keyCodeMap[VK_OEM_PERIOD] = KeyCode::Period;
                s_keyCodeMap[VK_OEM_PLUS] = KeyCode::Plus;
                s_keyCodeMap[VK_OEM_MINUS] = KeyCode::Minus;
                s_keyCodeMap[VK_OEM_COMMA] = KeyCode::Comma;
                //s_keyCodeMap[vk_]

                s_keyCodeMap['0'] = KeyCode::Keypad0;
                s_keyCodeMap['1'] = KeyCode::Keypad1;
                s_keyCodeMap['2'] = KeyCode::Keypad2;
                s_keyCodeMap['3'] = KeyCode::Keypad3;
                s_keyCodeMap['4'] = KeyCode::Keypad4;
                s_keyCodeMap['5'] = KeyCode::Keypad5;
                s_keyCodeMap['6'] = KeyCode::Keypad6;
                s_keyCodeMap['7'] = KeyCode::Keypad7;
                s_keyCodeMap['8'] = KeyCode::Keypad8;
                s_keyCodeMap['9'] = KeyCode::Keypad9;

                s_keyCodeMap['A'] = KeyCode::A;
                s_keyCodeMap['B'] = KeyCode::B;
                s_keyCodeMap['C'] = KeyCode::C;
                s_keyCodeMap['D'] = KeyCode::D;
                s_keyCodeMap['E'] = KeyCode::E;
                s_keyCodeMap['G'] = KeyCode::G;
                s_keyCodeMap['H'] = KeyCode::H;
                s_keyCodeMap['I'] = KeyCode::I;
                s_keyCodeMap['J'] = KeyCode::J;
                s_keyCodeMap['K'] = KeyCode::K;
                s_keyCodeMap['L'] = KeyCode::L;
                s_keyCodeMap['M'] = KeyCode::M;
                s_keyCodeMap['O'] = KeyCode::O;
                s_keyCodeMap['P'] = KeyCode::P;
                s_keyCodeMap['R'] = KeyCode::R;
                s_keyCodeMap['S'] = KeyCode::S;
                s_keyCodeMap['T'] = KeyCode::T;
                s_keyCodeMap['U'] = KeyCode::U;
                s_keyCodeMap['V'] = KeyCode::V;
                s_keyCodeMap['W'] = KeyCode::W;
                s_keyCodeMap['X'] = KeyCode::X;
                s_keyCodeMap['Y'] = KeyCode::Y;
                s_keyCodeMap['Z'] = KeyCode::Z;

                s_keyCodeMap['a'] = KeyCode::A;
                s_keyCodeMap['b'] = KeyCode::B;
                s_keyCodeMap['c'] = KeyCode::C;
                s_keyCodeMap['d'] = KeyCode::D;
                s_keyCodeMap['e'] = KeyCode::E;
                s_keyCodeMap['g'] = KeyCode::G;
                s_keyCodeMap['h'] = KeyCode::H;
                s_keyCodeMap['i'] = KeyCode::I;
                s_keyCodeMap['j'] = KeyCode::J;
                s_keyCodeMap['k'] = KeyCode::K;
                s_keyCodeMap['l'] = KeyCode::L;
                s_keyCodeMap['m'] = KeyCode::M;
                s_keyCodeMap['o'] = KeyCode::O;
                s_keyCodeMap['p'] = KeyCode::P;
                s_keyCodeMap['r'] = KeyCode::R;
                s_keyCodeMap['s'] = KeyCode::S;
                s_keyCodeMap['t'] = KeyCode::T;
                s_keyCodeMap['u'] = KeyCode::U;
                s_keyCodeMap['v'] = KeyCode::V;
                s_keyCodeMap['w'] = KeyCode::W;
                s_keyCodeMap['x'] = KeyCode::X;
                s_keyCodeMap['y'] = KeyCode::Y;
                s_keyCodeMap['z'] = KeyCode::Z;

                //s_keyCodeMap[','] = KeyCode::Comma;
                //s_keyCodeMap[':'] = KeyCode::Colon;
                s_keyCodeMap[';'] = KeyCode::SemiColon;
                //s_keyCodeMap['*'] = KeyCode::Aterisk;
                //s_keyCodeMap['&'] = KeyCode::Ampersand;
                s_keyCodeMap['`'] = KeyCode::BackQuote;
                //s_keyCodeMap['~'] = KeyCode::Tilde;
                //s_keyCodeMap['%'] = KeyCode::Percent;
                //s_keyCodeMap['$'] = KeyCode::Dolla;
                //s_keyCodeMap['#'] = KeyCode::Hash;
                //s_keyCodeMap['@'] = KeyCode::At;
                //s_keyCodeMap['!'] = KeyCode::Exclaim;
                //s_keyCodeMap['?'] = KeyCode::Question;
                //s_keyCodeMap['^'] = KeyCode::Caret;
                //s_keyCodeMap['<'] = KeyCode::Less;
                //s_keyCodeMap['='] = KeyCode::Equal;
                //s_keyCodeMap['>'] = KeyCode::Greater;

                s_keyCodeMap['('] = KeyCode::LeftParen;
                s_keyCodeMap[')'] = KeyCode::RightParen;
                s_keyCodeMap['['] = KeyCode::LeftBracket;
                s_keyCodeMap[']'] = KeyCode::RightBracket;
                s_keyCodeMap['{'] = KeyCode::LeftCurlyBracket;
                s_keyCodeMap['}'] = KeyCode::RightCurlyBracket;

                //s_keyCodeMap['\''] = KeyCode::Quote;
                //s_keyCodeMap['\"'] = KeyCode::DoubleQuote;

                s_keyCodeMap['\/'] = KeyCode::Slash;
                s_keyCodeMap['\\'] = KeyCode::BackSlash;

                return s_keyCodeMap[nativeKey];
            }
        }

        static LRESULT CALLBACK Window_Proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        {
            // Donot handle event from other window
            if (hwnd != _mainWindow)
            {
                return DefWindowProcA(hwnd, msg, wparam, lparam);;
            }

            switch (msg)
            {
            case WM_LBUTTONUP:
                Input::UpdateMouse(MouseButton::Left, false);
                return 0;

            case WM_LBUTTONDOWN:
                Input::UpdateMouse(MouseButton::Left, true);
                return 0;

            case WM_RBUTTONUP:
                Input::UpdateMouse(MouseButton::Right, false);
                return 0;

            case WM_RBUTTONDOWN:
                Input::UpdateMouse(MouseButton::Right, true);
                return 0;

            case WM_MBUTTONUP:
                Input::UpdateMouse(MouseButton::Middle, false);
                return 0;
                
            case WM_MBUTTONDOWN:
                Input::UpdateMouse(MouseButton::Middle, true);
                return 0;

            case WM_XBUTTONUP:
                Input::UpdateMouse(MouseButton::XButton1, false);
                return 0;

            case WM_XBUTTONDOWN:
                Input::UpdateMouse(MouseButton::XButton1, true);
                return 0;

            case WM_MOUSEMOVE:
                Input::UpdateMouseMove((float)GET_X_LPARAM(lparam), (float)GET_Y_LPARAM(lparam));
                return 0;

            case WM_MOUSEWHEEL:
                Input::UpdateMouseWheel(0, (float)GET_WHEEL_DELTA_WPARAM(wparam));
                return 0;

            case WM_MOUSEHWHEEL:
                Input::UpdateMouseWheel((float)GET_WHEEL_DELTA_WPARAM(wparam), 0);
                return 0;

            case WM_KEYUP:
                Input::UpdateKey(ConvertKeyCode((int)wparam), false);
                return 0;

            case WM_KEYDOWN:
                Input::UpdateKey(ConvertKeyCode((int)wparam), true);
                return 0;

            case WM_DESTROY:
                ::PostQuitMessage(0);
                return 0;

            case WM_CHAR:
            case WM_UNICHAR:
                Input::UpdateCharInput((int)wparam);
                return 0;

            case WM_QUIT:
                return 0;

            case WM_SIZE:
                GL::Viewport(0, 0, Window::GetWidth(), Window::GetHeight());
                return 0;
            }

            return DefWindowProcA(hwnd, msg, wparam, lparam);
        }

        static bool RegisterWindowClass(void)
        {
            static bool registered = false;

            if (!registered)
            {
                WNDCLASSA wc        = {};
                wc.lpfnWndProc      = Window_Proc;
                wc.hInstance        = ::GetModuleHandleA(nullptr);
                wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
                wc.lpszClassName    = WINDOW_CLASS;

                registered = ::RegisterClassA(&wc);
            }

            return registered;
        }

        static bool InitBaseContext(void)
        {
            static bool init = false;
            if (init)
            {
                return true;
            }

            HINSTANCE hInstance = ::GetModuleHandleA(nullptr);
            HWND hwnd = ::CreateWindowA(
                WINDOW_CLASS, DUMMY_WINDOW,
                0,
                0, 0, 100, 100,
                nullptr, nullptr, hInstance, nullptr);
            if (!hwnd)
            {
                return false;
            }

            HDC hdc = GetDC(hwnd);
            if (!hdc)
            {
                return false;
            }

            int format;
            PIXELFORMATDESCRIPTOR pfd = {
                sizeof(PIXELFORMATDESCRIPTOR),
                1,
                PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
                PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
                32,                   // Colordepth of the framebuffer.
                0, 0, 0, 0, 0, 0,
                0,
                0,
                0,
                0, 0, 0, 0,
                24,                   // Number of bits for the depthbuffer
                8,                    // Number of bits for the stencilbuffer
                0,                    // Number of Aux buffers in the framebuffer.
                PFD_MAIN_PLANE,
                0,
                0, 0, 0
            };
            if ((format = ChoosePixelFormat(hdc, &pfd)) == 0)
            {
                return false;
            }

            if (!SetPixelFormat(hdc, format, &pfd))
            {
                return false;
            }

            HGLRC context = wglCreateContext(hdc);
            if (!context)
            {
                return false;
            }
            wglMakeCurrent(hdc, context);

            glewExperimental = false;
            GLenum glewState = glewInit();
            if (glewState != GLEW_OK)
            {
                return false;
            }

            ::wglDeleteContext(context);
            ::DestroyWindow(hwnd);

            return (init = true);
        }
    }

    namespace Window
    {
        bool Setup(const char* title, int width, int height)
        {
            if (!RegisterWindowClass())
            {
                return false;
            }

            if (!InitBaseContext())
            {
                return false;
            }
            
            HWND hwnd = CreateWindowA(
                WINDOW_CLASS, title,
                WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                -1, -1, width, height,
                nullptr, nullptr, GetModuleHandleA(nullptr), nullptr);
            if (!hwnd)
            {
                return false;
            }

            _mainWindow  = hwnd;
            _mainDevice  = ::GetDC(hwnd);
            return true;
        }

        void Shutdown(void)
        {
            ::DestroyWindow(_mainWindow);

            _mainDevice  = NULL;
            _mainWindow  = NULL;
        }

        bool PollEvents(void)
        {
            Input::NewFrame();

            MSG msg;
            while (::PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    return false;
                }
                else
                {
                    ::TranslateMessage(&msg);
                    ::DispatchMessageA(&msg);
                }
            }

            Input::EndFrame();
            return true;
        }

        bool WaitAndPollEvents(void)
        {
            Input::NewFrame();

            MSG msg;
            if (::GetMessage(&msg, NULL, 0, 0))
            {
                if (msg.message == WM_QUIT)
                {
                    return false;
                }
                else
                {
                    ::TranslateMessage(&msg);
                    ::DispatchMessageA(&msg);
                }

                while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                {
                    if (msg.message == WM_QUIT)
                    {
                        return false;
                    }
                    else
                    {
                        ::TranslateMessage(&msg);
                        ::DispatchMessageA(&msg);
                    }
                }
            }

            Input::EndFrame();
            return true;
        }

        float GetWidth(void)
        {
            RECT rect;
            ::GetClientRect((HWND)_mainWindow, &rect);
            return (float)(rect.right - rect.left);
        }
        
        float GetHeight(void)
        {
            RECT rect;
            ::GetClientRect((HWND)_mainWindow, &rect);
            return (float)(rect.bottom - rect.top);
        }
    }

    namespace GL
    {
        static bool IsSettingsValid(const GraphicsSettings& settings)
        {
            if (settings.redBits < 1)
            {
                return false;
            }

            if (settings.greenBits < 1)
            {
                return false;
            }

            if (settings.blueBits < 1)
            {
                return false;
            }

            if (settings.alphaBits < 1)
            {
                return false;
            }

            if (settings.depthBits < 1)
            {
                return false;
            }

            if (settings.stencilBits < 1)
            {
                return false;
            }

            if (settings.multisamples < 1)
            {
                return false;
            }

            return true;
        }

        bool Setup(const GraphicsSettings& settings)
        {
            // Settings affect image result
            // It should be check
            if (!IsSettingsValid(settings))
            {
                return false;
            }

            // Modern api require differences extension to create context
            if (!wglewIsSupported("WGL_ARB_create_context"))
            {
                return false;
            }

            HDC hdc = _mainDevice;
            if (!hdc)
            {
                return false;
            }

            int format;
            int numFormats;
            int formatAttribs[] = {
                WGL_DOUBLE_BUFFER_ARB, 1,
                WGL_SUPPORT_OPENGL_ARB, 1,
                WGL_DRAW_TO_WINDOW_ARB, 1,

                WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,

                WGL_RED_BITS_ARB, settings.redBits,
                WGL_GREEN_BITS_ARB, settings.greenBits,
                WGL_BLUE_BITS_ARB, settings.blueBits,
                WGL_ALPHA_BITS_ARB, settings.alphaBits,
                
                WGL_DEPTH_BITS_ARB, settings.depthBits,
                WGL_STENCIL_BITS_ARB, settings.stencilBits,

                WGL_SAMPLE_BUFFERS_ARB, WGL_ARB_multisample && settings.multisamples > 1,
                WGL_SAMPLES_ARB, WGL_ARB_multisample ? settings.multisamples : 1,

                0
            };
            if (!wglChoosePixelFormatARB(hdc, formatAttribs, NULL, 1, &format, (UINT*)&numFormats))
            {
                return false;
            }

            if (!SetPixelFormat(hdc, format, NULL))
            {
                return false;
            }

            int contextAttribs[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 4, // Highest current supported version
                WGL_CONTEXT_MINOR_VERSION_ARB, 5, // Highest current supported version
                WGL_CONTEXT_FLAGS_ARB, 0,
                0
            };

            HGLRC context = wglCreateContextAttribsARB(hdc, 0, contextAttribs);
            if (!::wglMakeCurrent(hdc, context))
            {
                return false;
            }

            // Set viewport
            GL::Viewport(0, 0, Window::GetWidth(), Window::GetHeight());

            // Default blend
            GL::Enable(GraphicsMode::Blend);
            GL::SetBlendOp(BlendOp::Add);
            GL::SetBlendFunc(BlendFactor::SrcAlpha, BlendFactor::InvertSrcAlpha);
        }

        void Shutdown(void)
        {
            ::wglDeleteContext(_mainContext);
            _mainContext = NULL;
        }

        void SwapBuffers(void)
        {
            ::SwapBuffers(_mainDevice);
        }

        void ClearBuffer(int flags)
        {
            GLenum clearBits = 0;
            clearBits |= (flags & ClearFlag::Color)   ? GL_COLOR_BUFFER_BIT   : 0;
            clearBits |= (flags & ClearFlag::Depth)   ? GL_DEPTH_BUFFER_BIT   : 0;
            clearBits |= (flags & ClearFlag::Stencil) ? GL_STENCIL_BUFFER_BIT : 0;
            ::glClear(clearBits);
        }
    }
}