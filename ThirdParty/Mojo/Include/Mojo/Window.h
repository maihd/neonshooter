#pragma once

inline namespace Mojo
{
    namespace WindowFlag
    {
        enum
        {
            None       = 0,
            Visible    = 1 << 0,
            Resizable  = 1 << 1,
            Fullscreen = 1 << 2,
            Borderless = 1 << 3,

            Default = Visible,
        };
    }

    namespace Window
    {
        bool Setup(const char* title, int width, int height, int flags = WindowFlag::Default);
        void Shutdown(void);

        // Process all coming events
        // Return true if has quit message, false otherwise
        bool PollEvents(void); 

        // Wait a events and then process all coming events
        // Return true if has quit message, false otherwise
        bool WaitAndPollEvents(void);

        float GetWidth(void);
        float GetHeight(void);

        bool IsBorderless(void);
        bool IsFullscreen(void);
        //bool IsFullscreenSettings(void);

        void SetBorderless(void);
        void SetFullscreen(void);

        bool IsWindowed(void);
        void SetWindowed(void);

        bool IsVisible(void);
        void SetVisible(bool visible);

        void EnableVSync(void);
        void DisableVSync(void);
        bool IsVSyncEnabled(void);
        void SetVSyncEnabled(bool value);

        bool HasInputFocus(void);
        bool HasMouseFocus(void);
    }
}