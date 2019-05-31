#include <Mojo/ImGui.h>
#include <Mojo/Native/Window.h>
#include "Mojo_ImGui_Impl.h"

// SDL
// (the multi-viewports feature requires SDL features supported from SDL 2.0.4+. SDL 2.0.5+ is highly recommended)
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#if defined(__APPLE__)
#include "TargetConditionals.h"
#endif

struct SDL_Window;
typedef union SDL_Event SDL_Event;

bool ImGui_ImplSDL2_InitForOpenGL(SDL_Window* window, void* sdl_gl_context);
bool ImGui_ImplSDL2_InitForVulkan(SDL_Window* window);
void ImGui_ImplSDL2_Shutdown(void);
void ImGui_ImplSDL2_NewFrame(SDL_Window* window);
bool ImGui_ImplSDL2_ProcessEvent(const SDL_Event* event);

bool ImGui_ImplOpenGL3_Init(const char* glsl_version = NULL);
void ImGui_ImplOpenGL3_Shutdown(void);
void ImGui_ImplOpenGL3_NewFrame(void);
void ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);

// Called by Init/NewFrame/Shutdown
bool ImGui_ImplOpenGL3_CreateFontsTexture(void);
void ImGui_ImplOpenGL3_DestroyFontsTexture(void);
bool ImGui_ImplOpenGL3_CreateDeviceObjects(void);
void ImGui_ImplOpenGL3_DestroyDeviceObjects(void);

#define SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE    SDL_VERSION_ATLEAST(2,0,4)
#define SDL_HAS_WINDOW_ALPHA                SDL_VERSION_ATLEAST(2,0,5)
#define SDL_HAS_ALWAYS_ON_TOP               SDL_VERSION_ATLEAST(2,0,5)
#define SDL_HAS_USABLE_DISPLAY_BOUNDS       SDL_VERSION_ATLEAST(2,0,5)
#define SDL_HAS_PER_MONITOR_DPI             SDL_VERSION_ATLEAST(2,0,4)
#define SDL_HAS_VULKAN                      SDL_VERSION_ATLEAST(2,0,6)
#define SDL_HAS_MOUSE_FOCUS_CLICKTHROUGH    SDL_VERSION_ATLEAST(2,0,5)
#if !SDL_HAS_VULKAN
static const Uint32 SDL_WINDOW_VULKAN = 0x10000000;
#endif
// Data
static SDL_Window*  g_Window = NULL;
static Uint64       g_Time = 0;
static bool         g_MousePressed[3] = { false, false, false };
static SDL_Cursor*  g_MouseCursors[ImGuiMouseCursor_COUNT] = { 0 };
static char*        g_ClipboardTextData = NULL;

// Forward Declarations
static void ImGui_ImplSDL2_InitPlatformInterface(SDL_Window* window, void* sdl_gl_context);
static void ImGui_ImplSDL2_ShutdownPlatformInterface();

static const char* ImGui_ImplSDL2_GetClipboardText(void*)
{
    if (g_ClipboardTextData)
        SDL_free(g_ClipboardTextData);
    g_ClipboardTextData = SDL_GetClipboardText();
    return g_ClipboardTextData;
}

static void ImGui_ImplSDL2_SetClipboardText(void*, const char* text)
{
    SDL_SetClipboardText(text);
}

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
// If you have multiple SDL events and some of them are not meant to be used by dear imgui, you may need to filter events based on their windowID field.
bool ImGui_ImplSDL2_ProcessEvent(const SDL_Event* event)
{
    ImGuiIO& io = ImGui::GetIO();
    switch (event->type)
    {
    case SDL_MOUSEWHEEL:
    {
        if (event->wheel.x > 0) io.MouseWheelH += 1;
        if (event->wheel.x < 0) io.MouseWheelH -= 1;
        if (event->wheel.y > 0) io.MouseWheel += 1;
        if (event->wheel.y < 0) io.MouseWheel -= 1;
        return true;
    }
    case SDL_MOUSEBUTTONDOWN:
    {
        if (event->button.button == SDL_BUTTON_LEFT) g_MousePressed[0] = true;
        if (event->button.button == SDL_BUTTON_RIGHT) g_MousePressed[1] = true;
        if (event->button.button == SDL_BUTTON_MIDDLE) g_MousePressed[2] = true;
        return true;
    }
    case SDL_TEXTINPUT:
    {
        io.AddInputCharactersUTF8(event->text.text);
        return true;
    }
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    {
        int key = event->key.keysym.scancode;
        IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
        io.KeysDown[key] = (event->type == SDL_KEYDOWN);
        io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
        io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
        io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
        io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
        return true;
    }
    // Multi-viewport support
    case SDL_WINDOWEVENT:
        Uint8 window_event = event->window.event;
        if (window_event == SDL_WINDOWEVENT_CLOSE || window_event == SDL_WINDOWEVENT_MOVED || window_event == SDL_WINDOWEVENT_RESIZED)
            if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle((void*)SDL_GetWindowFromID(event->window.windowID)))
            {
                if (window_event == SDL_WINDOWEVENT_CLOSE)
                    viewport->PlatformRequestClose = true;
                if (window_event == SDL_WINDOWEVENT_MOVED)
                    viewport->PlatformRequestMove = true;
                if (window_event == SDL_WINDOWEVENT_RESIZED)
                    viewport->PlatformRequestResize = true;
                return true;
            }
        break;
    }
    return false;
}

static bool ImGui_ImplSDL2_Init(SDL_Window* window, void* sdl_gl_context)
{
    g_Window = window;

    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
#if SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE
    io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;  // We can create multi-viewports on the Platform side (optional)
#endif
    io.BackendPlatformName = "imgui_impl_sdl";

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
    io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
    io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
    io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
    io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
    io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
    io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
    io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
    io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
    io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

    io.SetClipboardTextFn = ImGui_ImplSDL2_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplSDL2_GetClipboardText;
    io.ClipboardUserData = NULL;

    g_MouseCursors[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    g_MouseCursors[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    g_MouseCursors[ImGuiMouseCursor_ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
    g_MouseCursors[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
    g_MouseCursors[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
    g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
    g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
    g_MouseCursors[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    // Our mouse update function expect PlatformHandle to be filled for the main viewport
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    main_viewport->PlatformHandle = (void*)window;

    // We need SDL_CaptureMouse(), SDL_GetGlobalMouseState() from SDL 2.0.4+ to support multiple viewports.
    // We left the call to ImGui_ImplSDL2_InitPlatformInterface() outside of #ifdef to avoid unused-function warnings.
    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) && (io.BackendFlags & ImGuiBackendFlags_PlatformHasViewports))
        ImGui_ImplSDL2_InitPlatformInterface(window, sdl_gl_context);

    return true;
}

bool ImGui_ImplSDL2_InitForOpenGL(SDL_Window* window, void* sdl_gl_context)
{
    (void)sdl_gl_context; // Viewport branch will need this.
    return ImGui_ImplSDL2_Init(window, sdl_gl_context);
}

bool ImGui_ImplSDL2_InitForVulkan(SDL_Window* window)
{
#if !SDL_HAS_VULKAN
    IM_ASSERT(0 && "Unsupported");
#endif
    return ImGui_ImplSDL2_Init(window, NULL);
}

void ImGui_ImplSDL2_Shutdown()
{
    ImGui_ImplSDL2_ShutdownPlatformInterface();
    g_Window = NULL;

    // Destroy last known clipboard data
    if (g_ClipboardTextData)
        SDL_free(g_ClipboardTextData);
    g_ClipboardTextData = NULL;

    // Destroy SDL mouse cursors
    for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
        SDL_FreeCursor(g_MouseCursors[cursor_n]);
    memset(g_MouseCursors, 0, sizeof(g_MouseCursors));
}

// This code is incredibly messy because some of the functions we need for full viewport support are not available in SDL < 2.0.4. 
static void ImGui_ImplSDL2_UpdateMousePosAndButtons()
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseHoveredViewport = 0;

    // [1]
    // Only when requested by io.WantSetMousePos: set OS mouse pos from Dear ImGui mouse pos.
    // (rarely used, mostly when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
    if (io.WantSetMousePos)
    {
#if SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            SDL_WarpMouseGlobal((int)io.MousePos.x, (int)io.MousePos.y);
        else
#endif
            SDL_WarpMouseInWindow(g_Window, (int)io.MousePos.x, (int)io.MousePos.y);
    }
    else
    {
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    }

    // [2]
    // Set Dear ImGui mouse pos from OS mouse pos + get buttons. (this is the common behavior)
    int mouse_x_local, mouse_y_local;
    Uint32 mouse_buttons = SDL_GetMouseState(&mouse_x_local, &mouse_y_local);
    io.MouseDown[0] = g_MousePressed[0] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;      // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
    io.MouseDown[1] = g_MousePressed[1] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    io.MouseDown[2] = g_MousePressed[2] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
    g_MousePressed[0] = g_MousePressed[1] = g_MousePressed[2] = false;

#if SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE && !defined(__EMSCRIPTEN__) && !defined(__ANDROID__) && !(defined(__APPLE__) && TARGET_OS_IOS)

    // SDL 2.0.4 and later has SDL_GetGlobalMouseState() and SDL_CaptureMouse()
    int mouse_x_global, mouse_y_global;
    SDL_GetGlobalMouseState(&mouse_x_global, &mouse_y_global);

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        // Multi-viewport mode: mouse position in OS absolute coordinates (io.MousePos is (0,0) when the mouse is on the upper-left of the primary monitor)
        if (SDL_Window* focused_window = SDL_GetKeyboardFocus())
            if (ImGui::FindViewportByPlatformHandle((void*)focused_window) != NULL)
                io.MousePos = ImVec2((float)mouse_x_global, (float)mouse_y_global);
    }
    else
    {
        // Single-viewport mode: mouse position in client window coordinatesio.MousePos is (0,0) when the mouse is on the upper-left corner of the app window)
        if (SDL_GetWindowFlags(g_Window) & SDL_WINDOW_INPUT_FOCUS)
        {
            int window_x, window_y;
            SDL_GetWindowPosition(g_Window, &window_x, &window_y);
            io.MousePos = ImVec2((float)(mouse_x_global - window_x), (float)(mouse_y_global - window_y));
        }
    }

    // SDL_CaptureMouse() let the OS know e.g. that our imgui drag outside the SDL window boundaries shouldn't e.g. trigger the OS window resize cursor.
    // The function is only supported from SDL 2.0.4 (released Jan 2016)
    bool any_mouse_button_down = ImGui::IsAnyMouseDown();
    SDL_CaptureMouse(any_mouse_button_down ? SDL_TRUE : SDL_FALSE);
#else
    // SDL 2.0.3 and before: single-viewport only
    if (SDL_GetWindowFlags(g_Window) & SDL_WINDOW_INPUT_FOCUS)
        io.MousePos = ImVec2((float)mouse_x_local, (float)mouse_y_local);
#endif
}

static void ImGui_ImplSDL2_UpdateMouseCursor()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        SDL_ShowCursor(SDL_FALSE);
    }
    else
    {
        // Show OS mouse cursor
        SDL_SetCursor(g_MouseCursors[imgui_cursor] ? g_MouseCursors[imgui_cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
        SDL_ShowCursor(SDL_TRUE);
    }
}

static void ImGui_ImplSDL2_UpdateGamepads()
{
    ImGuiIO& io = ImGui::GetIO();
    memset(io.NavInputs, 0, sizeof(io.NavInputs));
    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)
        return;

    // Get gamepad
    SDL_GameController* game_controller = SDL_GameControllerOpen(0);
    if (!game_controller)
    {
        io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
        return;
    }

    // Update gamepad inputs
#define MAP_BUTTON(NAV_NO, BUTTON_NO)       { io.NavInputs[NAV_NO] = (SDL_GameControllerGetButton(game_controller, BUTTON_NO) != 0) ? 1.0f : 0.0f; }
#define MAP_ANALOG(NAV_NO, AXIS_NO, V0, V1) { float vn = (float)(SDL_GameControllerGetAxis(game_controller, AXIS_NO) - V0) / (float)(V1 - V0); if (vn > 1.0f) vn = 1.0f; if (vn > 0.0f && io.NavInputs[NAV_NO] < vn) io.NavInputs[NAV_NO] = vn; }
    const int thumb_dead_zone = 8000;           // SDL_gamecontroller.h suggests using this value.
    MAP_BUTTON(ImGuiNavInput_Activate, SDL_CONTROLLER_BUTTON_A);               // Cross / A
    MAP_BUTTON(ImGuiNavInput_Cancel, SDL_CONTROLLER_BUTTON_B);               // Circle / B
    MAP_BUTTON(ImGuiNavInput_Menu, SDL_CONTROLLER_BUTTON_X);               // Square / X
    MAP_BUTTON(ImGuiNavInput_Input, SDL_CONTROLLER_BUTTON_Y);               // Triangle / Y
    MAP_BUTTON(ImGuiNavInput_DpadLeft, SDL_CONTROLLER_BUTTON_DPAD_LEFT);       // D-Pad Left
    MAP_BUTTON(ImGuiNavInput_DpadRight, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);      // D-Pad Right
    MAP_BUTTON(ImGuiNavInput_DpadUp, SDL_CONTROLLER_BUTTON_DPAD_UP);         // D-Pad Up
    MAP_BUTTON(ImGuiNavInput_DpadDown, SDL_CONTROLLER_BUTTON_DPAD_DOWN);       // D-Pad Down
    MAP_BUTTON(ImGuiNavInput_FocusPrev, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
    MAP_BUTTON(ImGuiNavInput_FocusNext, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
    MAP_BUTTON(ImGuiNavInput_TweakSlow, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
    MAP_BUTTON(ImGuiNavInput_TweakFast, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
    MAP_ANALOG(ImGuiNavInput_LStickLeft, SDL_CONTROLLER_AXIS_LEFTX, -thumb_dead_zone, -32768);
    MAP_ANALOG(ImGuiNavInput_LStickRight, SDL_CONTROLLER_AXIS_LEFTX, +thumb_dead_zone, +32767);
    MAP_ANALOG(ImGuiNavInput_LStickUp, SDL_CONTROLLER_AXIS_LEFTY, -thumb_dead_zone, -32767);
    MAP_ANALOG(ImGuiNavInput_LStickDown, SDL_CONTROLLER_AXIS_LEFTY, +thumb_dead_zone, +32767);

    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
#undef MAP_BUTTON
#undef MAP_ANALOG
}

void ImGui_ImplSDL2_NewFrame(SDL_Window* window)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

    // Setup display size (every frame to accommodate for window resizing)
    int w, h;
    int display_w, display_h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_GL_GetDrawableSize(window, &display_w, &display_h);
    io.DisplaySize = ImVec2((float)w, (float)h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

    // Setup time step (we don't use SDL_GetTicks() because it is using millisecond resolution)
    static Uint64 frequency = SDL_GetPerformanceFrequency();
    Uint64 current_time = SDL_GetPerformanceCounter();
    io.DeltaTime = g_Time > 0 ? (float)((double)(current_time - g_Time) / frequency) : (float)(1.0f / 60.0f);
    g_Time = current_time;

    ImGui_ImplSDL2_UpdateMousePosAndButtons();
    ImGui_ImplSDL2_UpdateMouseCursor();

    // Update game controllers (if enabled and available)
    ImGui_ImplSDL2_UpdateGamepads();
}

//--------------------------------------------------------------------------------------------------------
// MULTI-VIEWPORT / PLATFORM INTERFACE SUPPORT
// This is an _advanced_ and _optional_ feature, allowing the back-end to create and handle multiple viewports simultaneously.
// If you are new to dear imgui or creating a new binding for dear imgui, it is recommended that you completely ignore this section first..
//--------------------------------------------------------------------------------------------------------

struct ImGuiViewportDataSDL2
{
    SDL_Window*     Window;
    Uint32          WindowID;
    bool            WindowOwned;
    SDL_GLContext   GLContext;

    ImGuiViewportDataSDL2() { Window = NULL; WindowID = 0; WindowOwned = false; GLContext = NULL; }
    ~ImGuiViewportDataSDL2() { IM_ASSERT(Window == NULL && GLContext == NULL); }
};

static void ImGui_ImplSDL2_CreateWindow(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = IM_NEW(ImGuiViewportDataSDL2)();
    viewport->PlatformUserData = data;

    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGuiViewportDataSDL2* main_viewport_data = (ImGuiViewportDataSDL2*)main_viewport->PlatformUserData;

    // Share GL resources with main context
    bool use_opengl = (main_viewport_data->GLContext != NULL);
    SDL_GLContext backup_context = NULL;
    if (use_opengl)
    {
        backup_context = SDL_GL_GetCurrentContext();
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
        SDL_GL_MakeCurrent(main_viewport_data->Window, main_viewport_data->GLContext);
    }

    Uint32 sdl_flags = 0;
    sdl_flags |= use_opengl ? SDL_WINDOW_OPENGL : SDL_WINDOW_VULKAN;
    sdl_flags |= SDL_GetWindowFlags(g_Window) & SDL_WINDOW_ALLOW_HIGHDPI;
    sdl_flags |= SDL_WINDOW_HIDDEN;
    sdl_flags |= (viewport->Flags & ImGuiViewportFlags_NoDecoration) ? SDL_WINDOW_BORDERLESS : 0;
    sdl_flags |= (viewport->Flags & ImGuiViewportFlags_NoDecoration) ? 0 : SDL_WINDOW_RESIZABLE;
#if SDL_HAS_ALWAYS_ON_TOP
    sdl_flags |= (viewport->Flags & ImGuiViewportFlags_TopMost) ? SDL_WINDOW_ALWAYS_ON_TOP : 0;
#endif
    data->Window = SDL_CreateWindow("No Title Yet", (int)viewport->Pos.x, (int)viewport->Pos.y, (int)viewport->Size.x, (int)viewport->Size.y, sdl_flags);
    data->WindowOwned = true;
    if (use_opengl)
    {
        data->GLContext = SDL_GL_CreateContext(data->Window);
        SDL_GL_SetSwapInterval(0);
    }
    if (use_opengl && backup_context)
        SDL_GL_MakeCurrent(data->Window, backup_context);
    viewport->PlatformHandle = (void*)data->Window;
}

static void ImGui_ImplSDL2_DestroyWindow(ImGuiViewport* viewport)
{
    if (ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData)
    {
        if (data->GLContext && data->WindowOwned)
            SDL_GL_DeleteContext(data->GLContext);
        if (data->Window && data->WindowOwned)
            SDL_DestroyWindow(data->Window);
        data->GLContext = NULL;
        data->Window = NULL;
        IM_DELETE(data);
    }
    viewport->PlatformUserData = viewport->PlatformHandle = NULL;
}

static void ImGui_ImplSDL2_ShowWindow(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
#if defined(_WIN32)
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWindowWMInfo(data->Window, &info))
    {
        HWND hwnd = info.info.win.window;

        // SDL hack: Hide icon from task bar
        // Note: SDL 2.0.6+ has a SDL_WINDOW_SKIP_TASKBAR flag which is supported under Windows but the way it create the window breaks our seamless transition.
        if (viewport->Flags & ImGuiViewportFlags_NoTaskBarIcon)
        {
            LONG ex_style = ::GetWindowLong(hwnd, GWL_EXSTYLE);
            ex_style &= ~WS_EX_APPWINDOW;
            ex_style |= WS_EX_TOOLWINDOW;
            ::SetWindowLong(hwnd, GWL_EXSTYLE, ex_style);
        }

        // SDL hack: SDL always activate/focus windows :/
        if (viewport->Flags & ImGuiViewportFlags_NoFocusOnAppearing)
        {
            ::ShowWindow(hwnd, SW_SHOWNA);
            return;
        }
    }
#endif

    SDL_ShowWindow(data->Window);
}

static ImVec2 ImGui_ImplSDL2_GetWindowPos(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    int x = 0, y = 0;
    SDL_GetWindowPosition(data->Window, &x, &y);
    return ImVec2((float)x, (float)y);
}

static void ImGui_ImplSDL2_SetWindowPos(ImGuiViewport* viewport, ImVec2 pos)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    SDL_SetWindowPosition(data->Window, (int)pos.x, (int)pos.y);
}

static ImVec2 ImGui_ImplSDL2_GetWindowSize(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    int w = 0, h = 0;
    SDL_GetWindowSize(data->Window, &w, &h);
    return ImVec2((float)w, (float)h);
}

static void ImGui_ImplSDL2_SetWindowSize(ImGuiViewport* viewport, ImVec2 size)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    SDL_SetWindowSize(data->Window, (int)size.x, (int)size.y);
}

static void ImGui_ImplSDL2_SetWindowTitle(ImGuiViewport* viewport, const char* title)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    SDL_SetWindowTitle(data->Window, title);
}

#if SDL_HAS_WINDOW_ALPHA
static void ImGui_ImplSDL2_SetWindowAlpha(ImGuiViewport* viewport, float alpha)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    SDL_SetWindowOpacity(data->Window, alpha);
}
#endif

static void ImGui_ImplSDL2_SetWindowFocus(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    SDL_RaiseWindow(data->Window);
}

static bool ImGui_ImplSDL2_GetWindowFocus(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    return (SDL_GetWindowFlags(data->Window) & SDL_WINDOW_INPUT_FOCUS) != 0;
}

static bool ImGui_ImplSDL2_GetWindowMinimized(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    return (SDL_GetWindowFlags(data->Window) & SDL_WINDOW_MINIMIZED) != 0;
}

static void ImGui_ImplSDL2_RenderWindow(ImGuiViewport* viewport, void*)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    if (data->GLContext)
        SDL_GL_MakeCurrent(data->Window, data->GLContext);
}

static void ImGui_ImplSDL2_SwapBuffers(ImGuiViewport* viewport, void*)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    if (data->GLContext)
    {
        SDL_GL_MakeCurrent(data->Window, data->GLContext);
        SDL_GL_SwapWindow(data->Window);
    }
}

// Vulkan support (the Vulkan renderer needs to call a platform-side support function to create the surface)
// SDL is graceful enough to _not_ need <vulkan/vulkan.h> so we can safely include this.
#if SDL_HAS_VULKAN
#include <SDL2/SDL_vulkan.h>
static int ImGui_ImplSDL2_CreateVkSurface(ImGuiViewport* viewport, ImU64 vk_instance, const void* vk_allocator, ImU64* out_vk_surface)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    (void)vk_allocator;
    SDL_bool ret = SDL_Vulkan_CreateSurface(data->Window, (VkInstance)vk_instance, (VkSurfaceKHR*)out_vk_surface);
    return ret ? 0 : 1; // ret ? VK_SUCCESS : VK_NOT_READY 
}
#endif // SDL_HAS_VULKAN

// FIXME-PLATFORM: SDL doesn't have an event to notify the application of display/monitor changes
static void ImGui_ImplSDL2_UpdateMonitors()
{
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    platform_io.Monitors.resize(0);
    int display_count = SDL_GetNumVideoDisplays();
    for (int n = 0; n < display_count; n++)
    {
        // Warning: the validity of monitor DPI information on Windows depends on the application DPI awareness settings, which generally needs to be set in the manifest or at runtime.
        ImGuiPlatformMonitor monitor;
        SDL_Rect r;
        SDL_GetDisplayBounds(n, &r);
        monitor.MainPos = monitor.WorkPos = ImVec2((float)r.x, (float)r.y);
        monitor.MainSize = monitor.WorkSize = ImVec2((float)r.w, (float)r.h);
#if SDL_HAS_USABLE_DISPLAY_BOUNDS
        SDL_GetDisplayUsableBounds(n, &r);
        monitor.WorkPos = ImVec2((float)r.x, (float)r.y);
        monitor.WorkSize = ImVec2((float)r.w, (float)r.h);
#endif
#if SDL_HAS_PER_MONITOR_DPI
        float dpi = 0.0f;
        if (!SDL_GetDisplayDPI(n, &dpi, NULL, NULL))
            monitor.DpiScale = dpi / 96.0f;
#endif
        platform_io.Monitors.push_back(monitor);
    }
}

static void ImGui_ImplSDL2_InitPlatformInterface(SDL_Window* window, void* sdl_gl_context)
{
    // Register platform interface (will be coupled with a renderer interface)
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    platform_io.Platform_CreateWindow = ImGui_ImplSDL2_CreateWindow;
    platform_io.Platform_DestroyWindow = ImGui_ImplSDL2_DestroyWindow;
    platform_io.Platform_ShowWindow = ImGui_ImplSDL2_ShowWindow;
    platform_io.Platform_SetWindowPos = ImGui_ImplSDL2_SetWindowPos;
    platform_io.Platform_GetWindowPos = ImGui_ImplSDL2_GetWindowPos;
    platform_io.Platform_SetWindowSize = ImGui_ImplSDL2_SetWindowSize;
    platform_io.Platform_GetWindowSize = ImGui_ImplSDL2_GetWindowSize;
    platform_io.Platform_SetWindowFocus = ImGui_ImplSDL2_SetWindowFocus;
    platform_io.Platform_GetWindowFocus = ImGui_ImplSDL2_GetWindowFocus;
    platform_io.Platform_GetWindowMinimized = ImGui_ImplSDL2_GetWindowMinimized;
    platform_io.Platform_SetWindowTitle = ImGui_ImplSDL2_SetWindowTitle;
    platform_io.Platform_RenderWindow = ImGui_ImplSDL2_RenderWindow;
    platform_io.Platform_SwapBuffers = ImGui_ImplSDL2_SwapBuffers;
#if SDL_HAS_WINDOW_ALPHA
    platform_io.Platform_SetWindowAlpha = ImGui_ImplSDL2_SetWindowAlpha;
#endif
#if SDL_HAS_VULKAN
    platform_io.Platform_CreateVkSurface = ImGui_ImplSDL2_CreateVkSurface;
#endif

    // SDL2 by default doesn't pass mouse clicks to the application when the click focused a window. This is getting in the way of our interactions and we disable that behavior.
#if SDL_HAS_MOUSE_FOCUS_CLICKTHROUGH
    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
#endif

    ImGui_ImplSDL2_UpdateMonitors();

    // Register main window handle (which is owned by the main application, not by us)
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGuiViewportDataSDL2* data = IM_NEW(ImGuiViewportDataSDL2)();
    data->Window = window;
    data->WindowID = SDL_GetWindowID(window);
    data->WindowOwned = false;
    data->GLContext = sdl_gl_context;
    main_viewport->PlatformUserData = data;
    main_viewport->PlatformHandle = data->Window;
}

static void ImGui_ImplSDL2_ShutdownPlatformInterface()
{
}

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>     // intptr_t
#else
#include <stdint.h>     // intptr_t
#endif
#if defined(__APPLE__)
#include "TargetConditionals.h"
#endif

// Auto-detect GL version
#if !defined(IMGUI_IMPL_OPENGL_ES2) && !defined(IMGUI_IMPL_OPENGL_ES3)
#if (defined(__APPLE__) && TARGET_OS_IOS) || (defined(__ANDROID__))
#define IMGUI_IMPL_OPENGL_ES3       // iOS, Android  -> GL ES 3, "#version 300 es"
#elif defined(__EMSCRIPTEN__)
#define IMGUI_IMPL_OPENGL_ES2       // Emscripten    -> GL ES 2, "#version 100"
#endif
#endif

//#if defined(IMGUI_IMPL_OPENGL_ES2)
//#include <GLES2/gl2.h>
//#elif defined(IMGUI_IMPL_OPENGL_ES3)
//#include <GLES3/gl3.h>  // Use GL ES 3
//#else

#include <GL/glew.h>    // Needs to be initialized with glewInit() in user's code

// Desktop GL has glDrawElementsBaseVertex() which GL ES and WebGL don't have.
#if defined(IMGUI_IMPL_OPENGL_ES2) || defined(IMGUI_IMPL_OPENGL_ES3)
#define IMGUI_IMPL_OPENGL_HAS_DRAW_WITH_BASE_VERTEX     0
#else
#define IMGUI_IMPL_OPENGL_HAS_DRAW_WITH_BASE_VERTEX     1
#endif

// OpenGL Data
static char         g_GlslVersionString[32] = "";
static GLuint       g_FontTexture = 0;
static GLuint       g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
static int          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;                                // Uniforms location
static int          g_AttribLocationVtxPos = 0, g_AttribLocationVtxUV = 0, g_AttribLocationVtxColor = 0; // Vertex attributes location
static unsigned int g_VboHandle = 0, g_ElementsHandle = 0;

// Forward Declarations
static void ImGui_ImplOpenGL3_InitPlatformInterface();
static void ImGui_ImplOpenGL3_ShutdownPlatformInterface();

// Functions
bool    ImGui_ImplOpenGL3_Init(const char* glsl_version)
{
    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;    // We can create multi-viewports on the Renderer side (optional)
    io.BackendRendererName = "imgui_impl_opengl3";
#if IMGUI_IMPL_OPENGL_HAS_DRAW_WITH_BASE_VERTEX
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
#endif

    // Store GLSL version string so we can refer to it later in case we recreate shaders. Note: GLSL version is NOT the same as GL version. Leave this to NULL if unsure.
#if defined(IMGUI_IMPL_OPENGL_ES2)
    if (glsl_version == NULL)
        glsl_version = "#version 100";
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    if (glsl_version == NULL)
        glsl_version = "#version 300 es";
#else
    if (glsl_version == NULL)
        glsl_version = "#version 130";
#endif
    IM_ASSERT((int)strlen(glsl_version) + 2 < IM_ARRAYSIZE(g_GlslVersionString));
    strcpy(g_GlslVersionString, glsl_version);
    strcat(g_GlslVersionString, "\n");

    // Make a dummy GL call (we don't actually need the result)
    // IF YOU GET A CRASH HERE: it probably means that you haven't initialized the OpenGL function loader used by this code.
    // Desktop OpenGL 3/4 need a function loader. See the IMGUI_IMPL_OPENGL_LOADER_xxx explanation above.
    GLint current_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_texture);

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        ImGui_ImplOpenGL3_InitPlatformInterface();

    return true;
}

void    ImGui_ImplOpenGL3_Shutdown()
{
    ImGui_ImplOpenGL3_ShutdownPlatformInterface();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
}

void    ImGui_ImplOpenGL3_NewFrame()
{
    if (!g_FontTexture)
        ImGui_ImplOpenGL3_CreateDeviceObjects();
}

static void ImGui_ImplOpenGL3_SetupRenderState(ImDrawData* draw_data, int fb_width, int fb_height, GLuint vertex_array_object)
{
    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

    // Setup viewport, orthographic projection matrix
    // Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is (0,0) for single viewport apps.
    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
    const float ortho_projection[4][4] =
    {
        { 2.0f / (R - L),   0.0f,         0.0f,   0.0f },
        { 0.0f,         2.0f / (T - B),   0.0f,   0.0f },
        { 0.0f,         0.0f,        -1.0f,   0.0f },
        { (R + L) / (L - R),  (T + B) / (B - T),  0.0f,   1.0f },
    };
    glUseProgram(g_ShaderHandle);
    glUniform1i(g_AttribLocationTex, 0);
    glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
#ifdef GL_SAMPLER_BINDING
    glBindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.
#endif

    (void)vertex_array_object;
#ifndef IMGUI_IMPL_OPENGL_ES2
    glBindVertexArray(vertex_array_object);
#endif

    // Bind vertex/index buffers and setup attributes for ImDrawVert
    glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
    glEnableVertexAttribArray(g_AttribLocationVtxPos);
    glEnableVertexAttribArray(g_AttribLocationVtxUV);
    glEnableVertexAttribArray(g_AttribLocationVtxColor);
    glVertexAttribPointer(g_AttribLocationVtxPos, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(g_AttribLocationVtxUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(g_AttribLocationVtxColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));
}

// OpenGL3 Render function.
// (this used to be set in io.RenderDrawListsFn and called by ImGui::Render(), but you can now call this directly from your main loop)
// Note that this implementation is little overcomplicated because we are saving/setting up/restoring every OpenGL state explicitly, in order to be able to run within any OpenGL engine that doesn't do so.
void    ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data)
{
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    // Backup GL state
    GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
    glActiveTexture(GL_TEXTURE0);
    GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
#ifdef GL_SAMPLER_BINDING
    GLint last_sampler; glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
#endif
    GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
#ifndef IMGUI_IMPL_OPENGL_ES2
    GLint last_vertex_array_object; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array_object);
#endif
#ifdef GL_POLYGON_MODE
    GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
#endif
    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
    GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
    GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
    GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
    GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
    GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
    bool clip_origin_lower_left = true;
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
    GLenum last_clip_origin = 0; glGetIntegerv(GL_CLIP_ORIGIN, (GLint*)&last_clip_origin); // Support for GL 4.5's glClipControl(GL_UPPER_LEFT)
    if (last_clip_origin == GL_UPPER_LEFT)
        clip_origin_lower_left = false;
#endif

    // Setup desired GL state
    // Recreate the VAO every time (this is to easily allow multiple GL contexts to be rendered to. VAO are not shared among GL contexts)
    // The renderer would actually work without any VAO bound, but then our VertexAttrib calls would overwrite the default one currently bound.
    GLuint vertex_array_object = 0;
#ifndef IMGUI_IMPL_OPENGL_ES2
    glGenVertexArrays(1, &vertex_array_object);
#endif
    ImGui_ImplOpenGL3_SetupRenderState(draw_data, fb_width, fb_height, vertex_array_object);

    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];

        // Upload vertex/index buffers
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != NULL)
            {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    ImGui_ImplOpenGL3_SetupRenderState(draw_data, fb_width, fb_height, vertex_array_object);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                {
                    // Apply scissor/clipping rectangle
                    if (clip_origin_lower_left)
                        glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
                    else
                        glScissor((int)clip_rect.x, (int)clip_rect.y, (int)clip_rect.z, (int)clip_rect.w); // Support for GL 4.5 rarely used glClipControl(GL_UPPER_LEFT)

                    // Bind texture, Draw
                    glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
#if IMGUI_IMPL_OPENGL_HAS_DRAW_WITH_BASE_VERTEX
                    glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)), (GLint)pcmd->VtxOffset);
#else
                    glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)));
#endif
                }
            }
        }
    }

    // Destroy the temporary VAO
#ifndef IMGUI_IMPL_OPENGL_ES2
    glDeleteVertexArrays(1, &vertex_array_object);
#endif

    // Restore modified GL state
    glUseProgram(last_program);
    glBindTexture(GL_TEXTURE_2D, last_texture);
#ifdef GL_SAMPLER_BINDING
    glBindSampler(0, last_sampler);
#endif
    glActiveTexture(last_active_texture);
#ifndef IMGUI_IMPL_OPENGL_ES2
    glBindVertexArray(last_vertex_array_object);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
    if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
    if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
#endif
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
    glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

bool ImGui_ImplOpenGL3_CreateFontsTexture()
{
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef GL_UNPACK_ROW_LENGTH
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Store our identifier
    io.Fonts->TexID = (ImTextureID)(intptr_t)g_FontTexture;

    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);

    return true;
}

void ImGui_ImplOpenGL3_DestroyFontsTexture()
{
    if (g_FontTexture)
    {
        ImGuiIO& io = ImGui::GetIO();
        glDeleteTextures(1, &g_FontTexture);
        io.Fonts->TexID = 0;
        g_FontTexture = 0;
    }
}

// If you get an error please report on github. You may try different GL context version or GLSL version. See GL<>GLSL version table at the top of this file.
static bool CheckShader(GLuint handle, const char* desc)
{
    GLint status = 0, log_length = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);
    if ((GLboolean)status == GL_FALSE)
        fprintf(stderr, "ERROR: ImGui_ImplOpenGL3_CreateDeviceObjects: failed to compile %s!\n", desc);
    if (log_length > 1)
    {
        ImVector<char> buf;
        buf.resize((int)(log_length + 1));
        glGetShaderInfoLog(handle, log_length, NULL, (GLchar*)buf.begin());
        fprintf(stderr, "%s\n", buf.begin());
    }
    return (GLboolean)status == GL_TRUE;
}

// If you get an error please report on GitHub. You may try different GL context version or GLSL version.
static bool CheckProgram(GLuint handle, const char* desc)
{
    GLint status = 0, log_length = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);
    if ((GLboolean)status == GL_FALSE)
        fprintf(stderr, "ERROR: ImGui_ImplOpenGL3_CreateDeviceObjects: failed to link %s! (with GLSL '%s')\n", desc, g_GlslVersionString);
    if (log_length > 1)
    {
        ImVector<char> buf;
        buf.resize((int)(log_length + 1));
        glGetProgramInfoLog(handle, log_length, NULL, (GLchar*)buf.begin());
        fprintf(stderr, "%s\n", buf.begin());
    }
    return (GLboolean)status == GL_TRUE;
}

bool    ImGui_ImplOpenGL3_CreateDeviceObjects()
{
    // Backup GL state
    GLint last_texture, last_array_buffer;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
#ifndef IMGUI_IMPL_OPENGL_ES2
    GLint last_vertex_array;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
#endif

    // Parse GLSL version string
    int glsl_version = 130;
    sscanf(g_GlslVersionString, "#version %d", &glsl_version);

    const GLchar* vertex_shader_glsl_120 =
        "uniform mat4 ProjMtx;\n"
        "attribute vec2 Position;\n"
        "attribute vec2 UV;\n"
        "attribute vec4 Color;\n"
        "varying vec2 Frag_UV;\n"
        "varying vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "    Frag_UV = UV;\n"
        "    Frag_Color = Color;\n"
        "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
        "}\n";

    const GLchar* vertex_shader_glsl_130 =
        "uniform mat4 ProjMtx;\n"
        "in vec2 Position;\n"
        "in vec2 UV;\n"
        "in vec4 Color;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "    Frag_UV = UV;\n"
        "    Frag_Color = Color;\n"
        "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
        "}\n";

    const GLchar* vertex_shader_glsl_300_es =
        "precision mediump float;\n"
        "layout (location = 0) in vec2 Position;\n"
        "layout (location = 1) in vec2 UV;\n"
        "layout (location = 2) in vec4 Color;\n"
        "uniform mat4 ProjMtx;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "    Frag_UV = UV;\n"
        "    Frag_Color = Color;\n"
        "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
        "}\n";

    const GLchar* vertex_shader_glsl_410_core =
        "layout (location = 0) in vec2 Position;\n"
        "layout (location = 1) in vec2 UV;\n"
        "layout (location = 2) in vec4 Color;\n"
        "uniform mat4 ProjMtx;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "    Frag_UV = UV;\n"
        "    Frag_Color = Color;\n"
        "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
        "}\n";

    const GLchar* fragment_shader_glsl_120 =
        "#ifdef GL_ES\n"
        "    precision mediump float;\n"
        "#endif\n"
        "uniform sampler2D Texture;\n"
        "varying vec2 Frag_UV;\n"
        "varying vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);\n"
        "}\n";

    const GLchar* fragment_shader_glsl_130 =
        "uniform sampler2D Texture;\n"
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "out vec4 Out_Color;\n"
        "void main()\n"
        "{\n"
        "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
        "}\n";

    const GLchar* fragment_shader_glsl_300_es =
        "precision mediump float;\n"
        "uniform sampler2D Texture;\n"
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "layout (location = 0) out vec4 Out_Color;\n"
        "void main()\n"
        "{\n"
        "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
        "}\n";

    const GLchar* fragment_shader_glsl_410_core =
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "uniform sampler2D Texture;\n"
        "layout (location = 0) out vec4 Out_Color;\n"
        "void main()\n"
        "{\n"
        "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
        "}\n";

    // Select shaders matching our GLSL versions
    const GLchar* vertex_shader = NULL;
    const GLchar* fragment_shader = NULL;
    if (glsl_version < 130)
    {
        vertex_shader = vertex_shader_glsl_120;
        fragment_shader = fragment_shader_glsl_120;
    }
    else if (glsl_version >= 410)
    {
        vertex_shader = vertex_shader_glsl_410_core;
        fragment_shader = fragment_shader_glsl_410_core;
    }
    else if (glsl_version == 300)
    {
        vertex_shader = vertex_shader_glsl_300_es;
        fragment_shader = fragment_shader_glsl_300_es;
    }
    else
    {
        vertex_shader = vertex_shader_glsl_130;
        fragment_shader = fragment_shader_glsl_130;
    }

    // Create shaders
    const GLchar* vertex_shader_with_version[2] = { g_GlslVersionString, vertex_shader };
    g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(g_VertHandle, 2, vertex_shader_with_version, NULL);
    glCompileShader(g_VertHandle);
    CheckShader(g_VertHandle, "vertex shader");

    const GLchar* fragment_shader_with_version[2] = { g_GlslVersionString, fragment_shader };
    g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(g_FragHandle, 2, fragment_shader_with_version, NULL);
    glCompileShader(g_FragHandle);
    CheckShader(g_FragHandle, "fragment shader");

    g_ShaderHandle = glCreateProgram();
    glAttachShader(g_ShaderHandle, g_VertHandle);
    glAttachShader(g_ShaderHandle, g_FragHandle);
    glLinkProgram(g_ShaderHandle);
    CheckProgram(g_ShaderHandle, "shader program");

    g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
    g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");
    g_AttribLocationVtxPos = glGetAttribLocation(g_ShaderHandle, "Position");
    g_AttribLocationVtxUV = glGetAttribLocation(g_ShaderHandle, "UV");
    g_AttribLocationVtxColor = glGetAttribLocation(g_ShaderHandle, "Color");

    // Create buffers
    glGenBuffers(1, &g_VboHandle);
    glGenBuffers(1, &g_ElementsHandle);

    ImGui_ImplOpenGL3_CreateFontsTexture();

    // Restore modified GL state
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
#ifndef IMGUI_IMPL_OPENGL_ES2
    glBindVertexArray(last_vertex_array);
#endif

    return true;
}

void    ImGui_ImplOpenGL3_DestroyDeviceObjects()
{
    if (g_VboHandle) glDeleteBuffers(1, &g_VboHandle);
    if (g_ElementsHandle) glDeleteBuffers(1, &g_ElementsHandle);
    g_VboHandle = g_ElementsHandle = 0;

    if (g_ShaderHandle && g_VertHandle) glDetachShader(g_ShaderHandle, g_VertHandle);
    if (g_VertHandle) glDeleteShader(g_VertHandle);
    g_VertHandle = 0;

    if (g_ShaderHandle && g_FragHandle) glDetachShader(g_ShaderHandle, g_FragHandle);
    if (g_FragHandle) glDeleteShader(g_FragHandle);
    g_FragHandle = 0;

    if (g_ShaderHandle) glDeleteProgram(g_ShaderHandle);
    g_ShaderHandle = 0;

    ImGui_ImplOpenGL3_DestroyFontsTexture();
}

//--------------------------------------------------------------------------------------------------------
// MULTI-VIEWPORT / PLATFORM INTERFACE SUPPORT
// This is an _advanced_ and _optional_ feature, allowing the back-end to create and handle multiple viewports simultaneously.
// If you are new to dear imgui or creating a new binding for dear imgui, it is recommended that you completely ignore this section first..
//--------------------------------------------------------------------------------------------------------

static void ImGui_ImplOpenGL3_RenderWindow(ImGuiViewport* viewport, void*)
{
    if (!(viewport->Flags & ImGuiViewportFlags_NoRendererClear))
    {
        ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    ImGui_ImplOpenGL3_RenderDrawData(viewport->DrawData);
}

static void ImGui_ImplOpenGL3_InitPlatformInterface()
{
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    platform_io.Renderer_RenderWindow = ImGui_ImplOpenGL3_RenderWindow;
}

static void ImGui_ImplOpenGL3_ShutdownPlatformInterface()
{
    ImGui::DestroyPlatformWindows();
}

namespace Mojo
{
    bool ImGuiImpl_Setup(void)
    {
        return ImGui_ImplSDL2_Init((SDL_Window*)Window::GetSDLHandle(), 0) && ImGui_ImplOpenGL3_Init();
    }

    void ImGuiImpl_Shutdown(void)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
    }

    void ImGuiImpl_NewFrame(void)
    {
        ImGui_ImplSDL2_NewFrame((SDL_Window*)Window::GetSDLHandle());
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiImpl_EndFrame(void)
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    bool ImGuiImpl_ProcessEvent(void)
    {
        return false;
    }
}