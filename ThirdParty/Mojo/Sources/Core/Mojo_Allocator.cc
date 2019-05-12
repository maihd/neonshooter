#include <Mojo/Core/Allocator.h>

#include <stdlib.h>

namespace Mojo
{
    static Allocator DefaultAllocator;
    Allocator* const Allocator::Default = &DefaultAllocator;
    
    void* Allocator::Acquire(int size, int)
    {
        return ::malloc(size);
    }

    void  Allocator::Release(void* pointer)
    {
        return ::free(pointer);
    }
}