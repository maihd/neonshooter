#include <Mojo/Core/Thread.h>

#include <SDL2/SDL.h>

inline namespace Mojo
{
    bool Thread::Run(ThreadEntry entry, void* args, Thread* outThread)
    {
        Thread thread;
        thread.handle = SDL_CreateThread(entry, NULL, args);
        thread.id     = SDL_GetThreadID((SDL_Thread*)thread.handle);

        if (outThread)
        {
            *outThread = thread;
        }

        return thread.handle != NULL;
    }

    void Thread::Join(Thread& thread)
    {
        SDL_WaitThread((SDL_Thread*)thread.handle, NULL);
    }

    Thread Thread::GetCurrent(void)
    {
        Thread thread;
        thread.id     = SDL_ThreadID();
        thread.handle = NULL;
        return thread;
    }
}