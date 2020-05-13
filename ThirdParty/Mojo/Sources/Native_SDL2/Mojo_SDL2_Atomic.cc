#include <Mojo/Core/Atomic.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#if _WIN32_WINNT == 0x0a00
#define WIN10_SDK 1
#else
#define WIN10_SDK 0
#endif

namespace Mojo
{
    AtomicI32& AtomicI32::operator=(I32 value)
    {
        _InterlockedExchange((long*)&this->value, value);
        return *this;
    }

    AtomicI32& AtomicI32::operator=(const AtomicI32& other)
    {
        _InterlockedExchange((long*)&this->value, other.value);
        return *this;
    }

    AtomicI32& operator++(AtomicI32& atomic)
    {
        _InterlockedIncrement((volatile long*)&atomic.value);

        return atomic;
    }

    AtomicI32& operator--(AtomicI32& atomic)
    {
        _InterlockedDecrement((volatile long*)&atomic.value);
        return atomic;
    }

    AtomicI32 operator++(AtomicI32& atomic, int)
    {
        AtomicI32 result = atomic;

        _InterlockedIncrement((volatile long*)&atomic.value);

        return result;
    }

    AtomicI32 operator--(AtomicI32& atomic, int)
    {
        auto result = atomic;

        _InterlockedDecrement((volatile long*)&atomic.value);

        return result;
    }

    AtomicI32& operator+=(AtomicI32& atomic, I32 value)
    {
#if WIN10_SDK
        _InlineInterlockedAdd((volatile LONG*)&atomic.value, (LONG)value);
#else
        _InterlockedAdd((volatile LONG*)&atomic.value, (LONG)value);
#endif

        return atomic;
    }

    AtomicI32& operator-=(AtomicI32& atomic, I32 value)
    {
#if WIN10_SDK
        _InlineInterlockedAdd((volatile LONG*)&atomic.value, (LONG)-value);
#else
        _InterlockedAdd((volatile LONG*)&atomic.value, (LONG)-value);
#endif

        return atomic;
    }

    AtomicI32& operator^=(AtomicI32& atomic, I32 value)
    {
        _InterlockedXor((volatile LONG*)&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI32& operator|=(AtomicI32& atomic, I32 value)
    {
        _InterlockedOr((volatile LONG*)&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI32& operator&=(AtomicI32& atomic, I32 value)
    {
        _InterlockedAnd((volatile LONG*)&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI64& AtomicI64::operator=(I64 value)
    {
#if WIN10_SDK
        _InlineInterlockedExchange64(&this->value, value);
#else
        _InterlockedExchange64(&this->value, value);
#endif
        return *this;
    }

    AtomicI64& AtomicI64::operator=(const AtomicI64& other)
    {
#if WIN10_SDK
        _InlineInterlockedExchange64(&this->value, other.value);
#else
        _InterlockedExchange64(&this->value, other.value);
#endif
        return *this;
    }

    AtomicI64& operator++(AtomicI64& atomic)
    {
#if WIN10_SDK
        _InlineInterlockedIncrement64(&atomic.value);
#else
        _InterlockedIncrement64(&atomic.value);
#endif

        return atomic;
    }

    AtomicI64& operator--(AtomicI64& atomic)
    {
#if WIN10_SDK
        _InlineInterlockedDecrement64(&atomic.value);
#else
        _InterlockedDecrement64(&atomic.value);
#endif

        return atomic;
    }

    AtomicI64 operator++(AtomicI64& atomic, int)
    {
        AtomicI32 result = atomic;

#if WIN10_SDK
        _InlineInterlockedIncrement64(&atomic.value);
#else
        _InterlockedIncrement64(&atomic.value);
#endif

        return result;
    }

    AtomicI64 operator--(AtomicI64& atomic, int)
    {
        auto result = atomic;

#if WIN10_SDK
        _InlineInterlockedDecrement64(&atomic.value);
#else
        _InterlockedDecrement64(&atomic.value);
#endif

        return result;
    }

    AtomicI64& operator+=(AtomicI64& atomic, I64 value)
    {
#if WIN10_SDK
        _InlineInterlockedAdd64(&atomic.value, value);
#else
        _InterlockedAdd64(&atomic.value, value);
#endif

        return atomic;
    }

    AtomicI64& operator-=(AtomicI64& atomic, I64 value)
    {
#if WIN10_SDK
        _InlineInterlockedAdd64(&atomic.value, -value);
#else
        _InterlockedAdd64(&atomic.value, -value);
#endif

        return atomic;
    }

    AtomicI64& operator^=(AtomicI64& atomic, I64 value)
    {
#if WIN10_SDK
        _InlineInterlockedXor64(&atomic.value, value);
#else
        _InterlockedXor64(&atomic.value, value);
#endif

        return atomic;
    }

    AtomicI64& operator|=(AtomicI64& atomic, I64 value)
    {
#if WIN10_SDK
        _InlineInterlockedOr64(&atomic.value, value);
#else
        _InterlockedOr64(&atomic.value, value);
#endif

        return atomic;
    }

    AtomicI64& operator&=(AtomicI64& atomic, I64 value)
    {
#if WIN10_SDK
        _InlineInterlockedAnd64(&atomic.value, value);
#else
        _InterlockedAnd64(&atomic.value, value);
#endif

        return atomic;
    }
}