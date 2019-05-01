#pragma once

inline namespace Mojo
{
    struct AsyncOperation;
    using  AsyncOperationHandler = void (*)(const AsyncOperation*);

    struct AsyncOperation
    {
        bool const isDone;
        bool const success;

        int   const priority;
        float const progress;

        const void* const buffer;
        const int   const length;

        void*                 args; 
        AsyncOperationHandler handler;
    };
}