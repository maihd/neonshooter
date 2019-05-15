#pragma once

#include <string.h>

#ifndef MOJO_DEFINED_ALLOCATOR
#define MOJO_DEFINED_ALLOCATOR
inline namespace Mojo
{
    struct Allocator
    {
        static Allocator* const Default;

        virtual void* Acquire(int size, int align);
        virtual void  Release(void* pointer);
    };
}
#endif

inline namespace Mojo
{
    template <typename T>
    struct RingBuffer
    {

    };
}