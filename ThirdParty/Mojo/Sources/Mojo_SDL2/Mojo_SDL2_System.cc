#include <Mojo/System.h>

#include <SDL2/SDL.h>

inline namespace Mojo
{
    namespace System
    {
        int CpuCount(void)
        {
            return SDL_GetCPUCount();
        }

        void Sleep(int milliseconds)
        {
            SDL_Delay((Uint32)milliseconds);
        }

        long long PerformanceCounter(void)
        {
            return (long long)SDL_GetPerformanceCounter();
        }

        long long PerformanceFrequency(void)
        {
            static long long savedValue;
            if (savedValue > 0)
            {
                return savedValue;
            }

            return (savedValue = (long long)SDL_GetPerformanceFrequency());
        }
    }
}