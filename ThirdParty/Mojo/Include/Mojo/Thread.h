#pragma once

inline namespace Mojo
{
    using ThreadEntry = int (*)(void*);

    struct Thread
    {
        int   id;
        void* handle;

        Thread()
            : id(-1)
            , handle(0)
        {}

        static bool   Run(ThreadEntry entry, void* args, Thread* thread = 0);
        static void   Join(Thread& thread);
        static Thread GetCurrent(void);
    };

    struct ThreadMutex
    {
#ifdef _WIN64
        char _buffer[40];
#elif _WIN32
        char _buffer[24];
#else
        char _buffer[sizeof(int)];
#endif

        ThreadMutex(void);

        void Lock(void);
        void Unlock(void);
        bool TryLock(void);
    };

    struct ThreadSignal
    {
#if _WIN32
        char _buffer[sizeof(void*)];
#else
        char _buffer[sizeof(int)];
#endif

        ThreadSignal(void);

        void Wait(ThreadMutex& mutex);
        void Signal(void);
        void Broadcast(void);
    };

    struct ThreadSemaphore
    {
        ThreadMutex  _mutex;
        ThreadSignal _signal;
        int          _count;

        ThreadSemaphore(int value);

        void Post(void);
        void Wait(void);
        bool TryWait(void);
    };
}