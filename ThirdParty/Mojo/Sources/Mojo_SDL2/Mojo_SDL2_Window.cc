#include <Mojo/Window.h>
#include <Mojo/Graphics.h>

#include <stdio.h>
#include <limits.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "Mojo_SDL2_Input.h"

#pragma comment(lib, "OpenGL32.lib")

inline namespace Mojo
{
    namespace
    {
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

                s_keyCodeMap[SDL_SCANCODE_SPACE] = KeyCode::Space;
                s_keyCodeMap[SDL_SCANCODE_BACKSPACE] = KeyCode::Backspace;
                s_keyCodeMap[SDL_SCANCODE_RETURN] = KeyCode::Return;
                s_keyCodeMap[SDL_SCANCODE_TAB] = KeyCode::Tab;
                
                s_keyCodeMap[SDL_SCANCODE_UP] = KeyCode::UpArrow;
                s_keyCodeMap[SDL_SCANCODE_DOWN] = KeyCode::DownArrow;
                s_keyCodeMap[SDL_SCANCODE_LEFT] = KeyCode::LeftArrow;
                s_keyCodeMap[SDL_SCANCODE_RIGHT] = KeyCode::RightArrow;
                
                s_keyCodeMap[SDL_SCANCODE_HOME] = KeyCode::Home;
                s_keyCodeMap[SDL_SCANCODE_END] = KeyCode::End;
                s_keyCodeMap[SDL_SCANCODE_PAGEUP] = KeyCode::PageUp;
                s_keyCodeMap[SDL_SCANCODE_PAGEDOWN] = KeyCode::PageDown;
                s_keyCodeMap[SDL_SCANCODE_INSERT] = KeyCode::Insert;
                s_keyCodeMap[SDL_SCANCODE_DELETE] = KeyCode::Delete;
                
                s_keyCodeMap[SDL_SCANCODE_LCTRL] = KeyCode::LeftControl;
                s_keyCodeMap[SDL_SCANCODE_RCTRL] = KeyCode::RightControl;
                
                s_keyCodeMap[SDL_SCANCODE_LSHIFT] = KeyCode::LeftShift;
                s_keyCodeMap[SDL_SCANCODE_RSHIFT] = KeyCode::RightShift;
                
                s_keyCodeMap[SDL_SCANCODE_KP_MULTIPLY] = KeyCode::NumpadMultiply;
                s_keyCodeMap[SDL_SCANCODE_KP_DIVIDE] = KeyCode::NumpadDivide;
                s_keyCodeMap[SDL_SCANCODE_KP_PLUS] = KeyCode::NumpadAdd;
                s_keyCodeMap[SDL_SCANCODE_KP_MINUS] = KeyCode::NumpadSubtract;
                s_keyCodeMap[SDL_SCANCODE_KP_0] = KeyCode::Numpad0;
                s_keyCodeMap[SDL_SCANCODE_KP_1] = KeyCode::Numpad1;
                s_keyCodeMap[SDL_SCANCODE_KP_2] = KeyCode::Numpad2;
                s_keyCodeMap[SDL_SCANCODE_KP_3] = KeyCode::Numpad3;
                s_keyCodeMap[SDL_SCANCODE_KP_4] = KeyCode::Numpad4;
                s_keyCodeMap[SDL_SCANCODE_KP_5] = KeyCode::Numpad5;
                s_keyCodeMap[SDL_SCANCODE_KP_6] = KeyCode::Numpad6;
                s_keyCodeMap[SDL_SCANCODE_KP_7] = KeyCode::Numpad7;
                s_keyCodeMap[SDL_SCANCODE_KP_8] = KeyCode::Numpad8;
                s_keyCodeMap[SDL_SCANCODE_KP_9] = KeyCode::Numpad9;
                
                //s_keyCodeMap[SDL_SCANCODE_] = KeyCode::Plus;
                //s_keyCodeMap[VK_OEM_MINUS] = KeyCode::Minus;

                s_keyCodeMap[SDL_SCANCODE_0] = KeyCode::Keypad0;
                s_keyCodeMap[SDL_SCANCODE_1] = KeyCode::Keypad1;
                s_keyCodeMap[SDL_SCANCODE_2] = KeyCode::Keypad2;
                s_keyCodeMap[SDL_SCANCODE_3] = KeyCode::Keypad3;
                s_keyCodeMap[SDL_SCANCODE_4] = KeyCode::Keypad4;
                s_keyCodeMap[SDL_SCANCODE_5] = KeyCode::Keypad5;
                s_keyCodeMap[SDL_SCANCODE_6] = KeyCode::Keypad6;
                s_keyCodeMap[SDL_SCANCODE_7] = KeyCode::Keypad7;
                s_keyCodeMap[SDL_SCANCODE_8] = KeyCode::Keypad8;
                s_keyCodeMap[SDL_SCANCODE_9] = KeyCode::Keypad9;

                s_keyCodeMap[SDL_SCANCODE_A] = KeyCode::A;
                s_keyCodeMap[SDL_SCANCODE_B] = KeyCode::B;
                s_keyCodeMap[SDL_SCANCODE_C] = KeyCode::C;
                s_keyCodeMap[SDL_SCANCODE_D] = KeyCode::D;
                s_keyCodeMap[SDL_SCANCODE_E] = KeyCode::E;
                s_keyCodeMap[SDL_SCANCODE_G] = KeyCode::G;
                s_keyCodeMap[SDL_SCANCODE_H] = KeyCode::H;
                s_keyCodeMap[SDL_SCANCODE_I] = KeyCode::I;
                s_keyCodeMap[SDL_SCANCODE_J] = KeyCode::J;
                s_keyCodeMap[SDL_SCANCODE_K] = KeyCode::K;
                s_keyCodeMap[SDL_SCANCODE_L] = KeyCode::L;
                s_keyCodeMap[SDL_SCANCODE_M] = KeyCode::M;
                s_keyCodeMap[SDL_SCANCODE_O] = KeyCode::O;
                s_keyCodeMap[SDL_SCANCODE_P] = KeyCode::P;
                s_keyCodeMap[SDL_SCANCODE_R] = KeyCode::R;
                s_keyCodeMap[SDL_SCANCODE_S] = KeyCode::S;
                s_keyCodeMap[SDL_SCANCODE_T] = KeyCode::T;
                s_keyCodeMap[SDL_SCANCODE_U] = KeyCode::U;
                s_keyCodeMap[SDL_SCANCODE_V] = KeyCode::V;
                s_keyCodeMap[SDL_SCANCODE_W] = KeyCode::W;
                s_keyCodeMap[SDL_SCANCODE_X] = KeyCode::X;
                s_keyCodeMap[SDL_SCANCODE_Y] = KeyCode::Y;
                s_keyCodeMap[SDL_SCANCODE_Z] = KeyCode::Z;

                //s_keyCodeMap[SDL_SCANCODE_a] = KeyCode::A;
                //s_keyCodeMap[SDL_SCANCODE_b] = KeyCode::B;
                //s_keyCodeMap[SDL_SCANCODE_c] = KeyCode::C;
                //s_keyCodeMap[SDL_SCANCODE_d] = KeyCode::D;
                //s_keyCodeMap[SDL_SCANCODE_e] = KeyCode::E;
                //s_keyCodeMap[SDL_SCANCODE_g] = KeyCode::G;
                //s_keyCodeMap[SDL_SCANCODE_h] = KeyCode::H;
                //s_keyCodeMap[SDL_SCANCODE_i] = KeyCode::I;
                //s_keyCodeMap[SDL_SCANCODE_j] = KeyCode::J;
                //s_keyCodeMap[SDL_SCANCODE_k] = KeyCode::K;
                //s_keyCodeMap[SDL_SCANCODE_l] = KeyCode::L;
                //s_keyCodeMap[SDL_SCANCODE_m] = KeyCode::M;
                //s_keyCodeMap[SDL_SCANCODE_o] = KeyCode::O;
                //s_keyCodeMap[SDL_SCANCODE_p] = KeyCode::P;
                //s_keyCodeMap[SDL_SCANCODE_r] = KeyCode::R;
                //s_keyCodeMap[SDL_SCANCODE_s] = KeyCode::S;
                //s_keyCodeMap[SDL_SCANCODE_t] = KeyCode::T;
                //s_keyCodeMap[SDL_SCANCODE_u] = KeyCode::U;
                //s_keyCodeMap[SDL_SCANCODE_v] = KeyCode::V;
                //s_keyCodeMap[SDL_SCANCODE_w] = KeyCode::W;
                //s_keyCodeMap[SDL_SCANCODE_x] = KeyCode::X;
                //s_keyCodeMap[SDL_SCANCODE_y] = KeyCode::Y;
                //s_keyCodeMap[SDL_SCANCODE_z] = KeyCode::Z;

                s_keyCodeMap[SDL_SCANCODE_F1 ] = KeyCode::F1 ;
                s_keyCodeMap[SDL_SCANCODE_F2 ] = KeyCode::F2 ;
                s_keyCodeMap[SDL_SCANCODE_F3 ] = KeyCode::F3 ;
                s_keyCodeMap[SDL_SCANCODE_F4 ] = KeyCode::F4 ;
                s_keyCodeMap[SDL_SCANCODE_F5 ] = KeyCode::F5 ;
                s_keyCodeMap[SDL_SCANCODE_F6 ] = KeyCode::F6 ;
                s_keyCodeMap[SDL_SCANCODE_F7 ] = KeyCode::F7 ;
                s_keyCodeMap[SDL_SCANCODE_F8 ] = KeyCode::F8 ;
                s_keyCodeMap[SDL_SCANCODE_F9 ] = KeyCode::F9 ;
                s_keyCodeMap[SDL_SCANCODE_F10] = KeyCode::F10;
                s_keyCodeMap[SDL_SCANCODE_F11] = KeyCode::F11;
                s_keyCodeMap[SDL_SCANCODE_F12] = KeyCode::F12;
                s_keyCodeMap[SDL_SCANCODE_F13] = KeyCode::F13;
                s_keyCodeMap[SDL_SCANCODE_F14] = KeyCode::F14;
                s_keyCodeMap[SDL_SCANCODE_F15] = KeyCode::F15;
                s_keyCodeMap[SDL_SCANCODE_F16] = KeyCode::F16;
                s_keyCodeMap[SDL_SCANCODE_F17] = KeyCode::F17;
                s_keyCodeMap[SDL_SCANCODE_F18] = KeyCode::F18;
                s_keyCodeMap[SDL_SCANCODE_F19] = KeyCode::F19;
                s_keyCodeMap[SDL_SCANCODE_F20] = KeyCode::F20;
                s_keyCodeMap[SDL_SCANCODE_F21] = KeyCode::F21;
                s_keyCodeMap[SDL_SCANCODE_F22] = KeyCode::F22;
                s_keyCodeMap[SDL_SCANCODE_F23] = KeyCode::F23;
                s_keyCodeMap[SDL_SCANCODE_F24] = KeyCode::F24;
                
                s_keyCodeMap[SDL_SCANCODE_PERIOD] = KeyCode::Period;
                s_keyCodeMap[SDL_SCANCODE_COMMA] = KeyCode::Comma;
                //s_keyCodeMap[SDL_SCANCODE_COLON] = KeyCode::Colon;
                s_keyCodeMap[SDL_SCANCODE_SEMICOLON] = KeyCode::SemiColon;
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
                s_keyCodeMap[SDL_SCANCODE_EQUALS] = KeyCode::Equal;
                //s_keyCodeMap['>'] = KeyCode::Greater;

                s_keyCodeMap['('] = KeyCode::LeftParen;
                s_keyCodeMap[')'] = KeyCode::RightParen;
                s_keyCodeMap[SDL_SCANCODE_LEFTBRACKET] = KeyCode::LeftBracket;
                s_keyCodeMap[SDL_SCANCODE_RIGHTBRACKET] = KeyCode::RightBracket;
                s_keyCodeMap['{'] = KeyCode::LeftCurlyBracket;
                s_keyCodeMap['}'] = KeyCode::RightCurlyBracket;

                //s_keyCodeMap[SDL_SCANCODE_Q] = KeyCode::Quote;
                //s_keyCodeMap['\"'] = KeyCode::DoubleQuote;

                s_keyCodeMap[SDL_SCANCODE_SLASH] = KeyCode::Slash;
                s_keyCodeMap[SDL_SCANCODE_BACKSLASH] = KeyCode::BackSlash;

                return s_keyCodeMap[nativeKey];
            }
        }

        void HandleEvent(const SDL_Event& event)
        {
            constexpr float axisFactor = 1 / (float)SHRT_MAX;

            switch (event.type)
            {
            case SDL_MOUSEBUTTONUP:
                Input::UpdateMouse((MouseButton)(1 << (event.button.button - 1)), false);
                break;

            case SDL_MOUSEBUTTONDOWN:
                Input::UpdateMouse((MouseButton)(1 << (event.button.button - 1)), true);
                break;

            case SDL_MOUSEMOTION:
                Input::UpdateMouseMove((float)event.motion.x, (float)event.motion.y);
                break;

            case SDL_MOUSEWHEEL:
                Input::UpdateMouseWheel((float)event.wheel.x, (float)event.wheel.y);
                break;

            case SDL_KEYUP:
                Input::UpdateKey(ConvertKeyCode((int)event.key.keysym.scancode), false);
                break;

            case SDL_KEYDOWN:
                Input::UpdateKey(ConvertKeyCode((int)event.key.keysym.scancode), true);
                break;

            case SDL_TEXTINPUT:
                Input::UpdateCharInput(event.text.text);
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    Graphics::Viewport(0, 0, Window::GetWidth(), Window::GetHeight());
                    break;
                }
                break;
            }
        }
    }

    namespace Window
    {
        SDL_Window*   _mainWindow;
        SDL_GLContext _mainContext;

        int   _windowFlags;
        int   _windowedWidth;
        int   _windowedHeight;

        bool Setup(const char* title, int width, int height, int flags)
        {
            if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
            {
                fprintf(stderr, "[Error] SDL_Init: %s\n", SDL_GetError());
                //goto error;
                return false;
            }

            if (_mainWindow)
            {
                return true;
            }

            Uint32 sdlFlags = SDL_WINDOW_OPENGL;
            if (flags & WindowFlag::Visible)
            {
                sdlFlags |= SDL_WINDOW_SHOWN;
            }
            if (flags & WindowFlag::Resizable)
            {
                sdlFlags |= SDL_WINDOW_RESIZABLE;
            }

            switch (flags & (WindowFlag::Borderless | WindowFlag::Fullscreen))
            {
            case WindowFlag::Borderless:
            case WindowFlag::Borderless | WindowFlag::Fullscreen:
                sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
                sdlFlags &= ~SDL_WINDOW_RESIZABLE;
                break;

            case WindowFlag::Fullscreen:
                sdlFlags |= SDL_WINDOW_FULLSCREEN;
                sdlFlags &= ~SDL_WINDOW_RESIZABLE;
                break;

            default:
                break;
            }

            SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, sdlFlags);
            if (!window)
            {
                return false;
            }

            _mainWindow = window;
            _windowFlags = flags;

            Input::Setup();
            return true;
        }

        void Shutdown(void)
        {
            Input::Shutdown();

            SDL_DestroyWindow(_mainWindow);
            SDL_QuitSubSystem(SDL_INIT_VIDEO);

            _mainWindow  = NULL;
            _windowFlags = 0;
        }

        bool PollEvents(void)
        {
            Input::NewFrame();

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    return false;
                }
                else
                {
                    HandleEvent(event);
                }
            }

            Input::EndFrame();
            return true;
        }

        bool WaitAndPollEvents(void)
        {
            Input::NewFrame();

            SDL_Event event;
            if (SDL_WaitEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    return false;
                }
                else
                {
                    HandleEvent(event);
                }

                while (SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                    {
                        return false;
                    }
                    else
                    {
                        HandleEvent(event);
                    }
                }
            }

            Input::EndFrame();
            return true;
        }

        float GetWidth(void)
        {
            int w;
            int h;
            SDL_GetWindowSize(_mainWindow, &w, &h);
            return (float)w;
        }
        
        float GetHeight(void)
        {
            int w;
            int h;
            SDL_GetWindowSize(_mainWindow, &w, &h);
            return (float)h;
        }

        void SetBorderless(void)
        {
            if (_mainWindow)
            {
                SDL_SetWindowFullscreen(_mainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
        }

        void SetFullscreen(void)
        {
            if (_mainWindow)
            {
                SDL_SetWindowFullscreen(_mainWindow, SDL_WINDOW_FULLSCREEN);
            }
        }

        bool IsBorderless(void)
        {
            return _windowFlags & WindowFlag::Borderless;
        }

        bool IsFullscreen(void)
        {
            return _windowFlags & WindowFlag::Fullscreen;
        }

        bool IsWindowed(void)
        {
            return !IsBorderless() || !IsFullscreen();
        }

        void SetWindowed(void)
        {
            if (_mainWindow)
            {
                //RECT clientRect;
                //RECT windowRect;
                //::GetClientRect(_mainWindow, &clientRect);
                //::GetWindowRect(_mainWindow, &windowRect);
                //
                //int moreWidth = (int)((windowRect.right - windowRect.left) - (clientRect.right - clientRect.left));
                //int moreHeight = (int)((windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top));
                //
                //int width  = _windowedWidth + moreWidth;
                //int height = _windowedHeight + moreHeight;
                //
                //int x, y;
                //DWORD flags = 0;
                //if (!IsWindowed())
                //{
                //    x = (::GetSystemMetrics(SM_CXSCREEN) - width) / 2;
                //    y = (::GetSystemMetrics(SM_CYSCREEN) - height) / 2;
                //    flags = SWP_NOZORDER;
                //}
                //else
                //{
                //    x = 0;
                //    y = 0;
                //    flags = SWP_NOMOVE | SWP_NOZORDER;
                //}
                //
                //LONG_PTR styles = WS_VISIBLE | WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;
                //::SetWindowLongPtr(_mainWindow, GWL_STYLE, styles);
                //::SetWindowPos(_mainWindow, HWND_TOP, x, y, width, height, flags);
            }
        }

        bool IsVisible(void)
        {
            return SDL_GetWindowFlags(_mainWindow) & SDL_WINDOW_SHOWN;
        }

        void SetVisible(bool visible)
        {
            if (visible)
            {
                SDL_ShowWindow(_mainWindow);
            }
            else
            {
                SDL_HideWindow(_mainWindow);
            }
        }

        void SetCenter(void)
        {
            if (_mainWindow && IsWindowed())
            {
                //int width = _windowedWidth + ::GetSystemMetrics(SM_CXBORDER);
                //int height = _windowedHeight + ::GetSystemMetrics(SM_CYBORDER);
                //
                //int x = (::GetSystemMetrics(SM_CXSCREEN) - width) / 2;
                //int y = (::GetSystemMetrics(SM_CYSCREEN) - height) / 2;
                //
                //::SetWindowPos(_mainWindow, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                SDL_SetWindowPosition(_mainWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
            }
        }

        void EnableVSync(void)
        {
            SetVSyncEnabled(true);
        }

        void DisableVSync(void)
        {
            SetVSyncEnabled(false);
        }

        bool IsVSyncEnabled(void)
        {
            return SDL_GL_GetSwapInterval() == 1;
        }

        void SetVSyncEnabled(bool vsync)
        {
            SDL_GL_SetSwapInterval(vsync);
        }

        bool HasInputFocus(void)
        {
            return _mainWindow && (_mainWindow == SDL_GetMouseFocus() || _mainWindow == SDL_GetKeyboardFocus());
        }

        bool HasMouseFocus(void)
        {
            return _mainWindow && (_mainWindow == SDL_GetMouseFocus());
        }
    }

    namespace Graphics
    {
        void ApplyDefaultSettings(void);
        void CreateDefaultObjects(void);

        static bool IsSettingsValid(const GraphicsSettings& settings)
        {
            if (settings.multisamples < 1)
            {
                return false;
            }

            return true;
        }

        bool Setup(const GraphicsSettings& settings)
        {
            //int formatAttribs[] = {
            //    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            //    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            //    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            //
            //    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            //    //WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            //
            //    WGL_ACCUM_RED_BITS_ARB, 8,
            //    WGL_ACCUM_GREEN_BITS_ARB, 8,
            //    WGL_ACCUM_BLUE_BITS_ARB, 8,
            //    WGL_ACCUM_ALPHA_BITS_ARB, 8,
            //
            //    WGL_RED_BITS_ARB, 8,
            //    WGL_GREEN_BITS_ARB, 8,
            //    WGL_BLUE_BITS_ARB, 8,
            //    WGL_ALPHA_BITS_ARB, 8,
            //
            //    WGL_RED_SHIFT_ARB, 0,
            //    WGL_GREEN_SHIFT_ARB, 8,
            //    WGL_BLUE_SHIFT_ARB, 16,
            //    WGL_ALPHA_SHIFT_ARB, 24,
            //
            //    WGL_ACCUM_BITS_ARB, 32,
            //    WGL_COLOR_BITS_ARB, 32,
            //    WGL_DEPTH_BITS_ARB, 24,
            //    WGL_STENCIL_BITS_ARB, 8,
            //
            //    WGL_ARB_multisample && settings.multisamples > 1 ? WGL_SAMPLE_BUFFERS_ARB : 0, 1,
            //    WGL_ARB_multisample && settings.multisamples > 1 ? WGL_SAMPLES_ARB : 0, settings.multisamples,
            //
            //    0
            //};

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GLContext context = SDL_GL_CreateContext(Window::_mainWindow);
            if (!context)
            {
                return false;
            }

            if (SDL_GL_MakeCurrent(Window::_mainWindow, context) != 0)
            {
                SDL_GL_DeleteContext(context);
                return false;
            }
            Window::_mainContext = context;

            // Initialize glew
            //glewExperimental = false;
            GLenum glewState = glewInit();
            if (glewState != GLEW_OK)
            {
                return false;
            }

            // By default, vsync is enable
            Window::SetVSyncEnabled(true);

            // Set viewport
            Graphics::Viewport(0, 0, Window::GetWidth(), Window::GetHeight());

            // Default settings
            ApplyDefaultSettings();
            CreateDefaultObjects();

            return true;
        }

        void Shutdown(void)
        {
            SDL_GL_DeleteContext(Window::_mainContext);
            Window::_mainContext = NULL;
        }

        void SwapBuffers(void)
        {
            SDL_GL_SwapWindow(Window::_mainWindow);
        }

        void ClearBuffer(void)
        {
            //GLbitfield flags = GL_COLOR_BUFFER_BIT;// | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
            GLbitfield flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
            glClear(flags);
        }
    }
}