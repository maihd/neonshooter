#pragma once 

inline namespace Mojo
{
    namespace System
    {
        int         CpuCores(void);

        void        Sleep(int miliseconds);
        void        MicroSleep(long long microseconds);

        long long   PerformanceCounter(void);
        long long   PerformanceFrequency(void);
    }
}