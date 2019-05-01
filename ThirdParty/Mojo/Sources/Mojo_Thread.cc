#include <Mojo/Thread.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

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

    ThreadMutex::ThreadMutex(void)
    {
        ::InitializeCriticalSection((LPCRITICAL_SECTION)this);
    }

    void ThreadMutex::Lock(void)
    {
        ::EnterCriticalSection((LPCRITICAL_SECTION)this);
    }

    void ThreadMutex::Unlock(void)
    {
        ::LeaveCriticalSection((LPCRITICAL_SECTION)this);
    }

    bool ThreadMutex::TryLock(void)
    {
        return ::TryEnterCriticalSection((LPCRITICAL_SECTION)this);
    }

    ThreadSignal::ThreadSignal(void)
    {
        ::InitializeConditionVariable((CONDITION_VARIABLE*)this);
    }

    void ThreadSignal::Wait(ThreadMutex& mutex)
    {
        if (::SleepConditionVariableCS((CONDITION_VARIABLE*)this, (CRITICAL_SECTION*)&mutex, INFINITE))
        {
            //process::abort();
        }
    }

    void ThreadSignal::Signal(void)
    {
        ::WakeConditionVariable((CONDITION_VARIABLE*)this);
    }

    void ThreadSignal::Broadcast(void)
    {
        ::WakeAllConditionVariable((CONDITION_VARIABLE*)this);
    }

    ThreadSemaphore::ThreadSemaphore(int count)
        : _mutex()
        , _signal()
        , _count(count)
    {
    }

    void ThreadSemaphore::Post(void)
    {
        _mutex.Lock();

        while (_count == 1)
        {
            _signal.Wait(_mutex);
        }
        _count++;

        _mutex.Unlock();
    }

    void ThreadSemaphore::Wait(void)
    {
        _mutex.Lock();

        while (_count <= 0)
        {
            _signal.Wait(_mutex);
        }
        _count--;

        _mutex.Unlock();
    }

    bool ThreadSemaphore::TryWait(void)
    {
        if (!_mutex.TryLock())
            return false;

        if (_count == 0) 
        {
            _mutex.Unlock();
            return false;
        }

        _count--;
        _mutex.Unlock();

        return true;
    }
}