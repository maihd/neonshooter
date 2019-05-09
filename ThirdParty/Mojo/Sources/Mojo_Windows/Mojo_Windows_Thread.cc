#include <Mojo/Thread.h>

inline namespace Mojo
{
    bool Thread::Run(ThreadEntry entry, void* args, Thread* outThread)
    {
        Thread thread;
        thread.handle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)entry, args, 0, (DWORD*)&thread.id);

        if (outThread)
        {
            *outThread = thread;
        }

        return thread.handle != NULL;
    }
    
    void Thread::Join(Thread& thread)
    {
        ::WaitForSingleObject(thread.handle, INFINITE);
    }

    Thread Thread::GetCurrent(void)
    {
        Thread thread;
        thread.id     = ::GetCurrentThreadId();
        thread.handle = ::GetCurrentThread();
        return thread;
    }
}