#include <Mojo/Core/Coroutine.h>

#include <stdlib.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#undef Yield

namespace Mojo
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

        coro->handle = NULL;
        coro->Yield();
    }

    bool Coroutine::Start(void(*entry)(void*), void* args)
    {
        if (!handle)
        {
            if (!s_threadFiber)
            {
                s_threadFiber = ::ConvertThreadToFiber(NULL);
                if (!s_threadFiber)
                {
                    return false;
                }
            }

            handle = ::CreateFiber(0, Coroutine_FiberProc, this);
            _userdata = args;
            _function = entry;
        }

        return handle != NULL;
    }

    void Coroutine::Release(void)
    {
        if (handle)
        {
            ::DeleteFiber(handle);
            handle = NULL;
        }
    }

    bool Coroutine::Resume(void)
    {
        if (handle)
        {
            s_currentCoroutine = this;
            ::SwitchToFiber(handle);
            return true;
        }
        else
        {
            return false;
        }
    }
    
    bool Coroutine::Status(void)
    {
        return handle != NULL;
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