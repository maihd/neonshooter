#pragma once

inline namespace Mojo
{
    namespace Window 
    {
        bool Setup(const char* title, int width, int height);
        void Shutdown(void);

        // Process all coming events
        // Return true if has quit message, false otherwise
        bool PollEvents(void); 

        // Wait a events and then process all coming events
        // Return true if has quit message, false otherwise
        bool WaitAndPollEvents(void);

        float GetWidth(void);
        float GetHeight(void);
    }
}