#pragma once

#include <Mojo/Core/Atomic.h>
#include <Mojo/Core/Thread.h>

inline namespace Mojo
{
    struct JobCounter
    {
        AtomicI32    _count;
        ThreadMutex  _mutex;
        ThreadSignal _signal;

        JobCounter(int value)
            : _count(value)
            , _mutex()
            , _signal()
        {
        }
    };
}