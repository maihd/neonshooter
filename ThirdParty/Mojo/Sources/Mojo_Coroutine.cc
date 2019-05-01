#include <Mojo/Coroutine.h>

#include <stdlib.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#undef Yield

inline namespace Mojo
{
    thread_local void*      s_threadFiber;
    thread_local Coroutine* s_currentCoroutine;

    static void WINAPI Coroutine_FiberProc(void* params) 
    {
        Coroutine* coro = (Coroutine*)params;
        if (coro->_function)
        {
            coro->_function(coro->_userdata);
        }

        coro->_handle = NULL;
        coro->Yield();
    }

    bool Coroutine::Start(void(*entry)(void*), void* args)
    {
        if (!_handle)
        {
            if (!s_threadFiber)
            {
                s_threadFiber = ::ConvertThreadToFiber(NULL);
                if (!s_threadFiber)
                {
                    return false;
                }
            }

            _handle = ::CreateFiber(0, Coroutine_FiberProc, this);
            _userdata = args;
            _function = entry;
        }

        return _handle != NULL;
    }

    void Coroutine::Release(void)
    {
        if (_handle)
        {
            ::DeleteFiber(_handle);
            _handle = NULL;
        }
    }

    bool Coroutine::Resume(void)
    {
        if (_handle)
        {
            s_currentCoroutine = this;
            ::SwitchToFiber(_handle);
            return true;
        }
        else
        {
            return false;
        }
    }
    
    bool Coroutine::Status(void)
    {
        return _handle != NULL;
    }
    
    void Coroutine::Yield(void)
    {
        if (s_threadFiber)
        {
            s_currentCoroutine = NULL;
            ::SwitchToFiber(s_threadFiber);
        }
    }
}