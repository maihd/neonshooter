#pragma once

inline namespace Mojo
{
    struct Coroutine
    {
        void* _handle;
        void* _userdata;
        void(*_function)(void*);

        bool Start(void(*entry)(void*), void* args);
        void Release(void);

        bool Resume(void);
        bool Status(void);

        static void Yield(void);
    };
}