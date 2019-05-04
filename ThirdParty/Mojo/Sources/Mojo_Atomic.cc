#include <Mojo/Atomic.h>

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

inline namespace Mojo
{
    AtomicI32& AtomicI32::operator=(int value)
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

    AtomicI32& operator+=(AtomicI32& atomic, int value)
    {
        _InlineInterlockedAdd((volatile LONG*)&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI32& operator-=(AtomicI32& atomic, int value)
    {
        _InlineInterlockedAdd((volatile LONG*)&atomic.value, (LONG)-value);

        return atomic;
    }

    AtomicI32& operator^=(AtomicI32& atomic, int value)
    {
        _InterlockedXor((volatile LONG*)&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI32& operator|=(AtomicI32& atomic, int value)
    {
        _InterlockedOr((volatile LONG*)&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI32& operator&=(AtomicI32& atomic, int value)
    {
        _InterlockedAnd((volatile LONG*)&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI64& AtomicI64::operator=(int value)
    {
        _InlineInterlockedExchange64(&this->value, value);
        return *this;
    }

    AtomicI64& AtomicI64::operator=(const AtomicI64& other)
    {
        _InlineInterlockedExchange64(&this->value, other.value);
        return *this;
    }

    AtomicI64& operator++(AtomicI64& atomic)
    {
        _InlineInterlockedIncrement64(&atomic.value);

        return atomic;
    }

    AtomicI64& operator--(AtomicI64& atomic)
    {
        _InlineInterlockedDecrement64(&atomic.value);
        return atomic;
    }

    AtomicI64 operator++(AtomicI64& atomic, int)
    {
        AtomicI32 result = atomic;

        _InlineInterlockedIncrement64(&atomic.value);

        return result;
    }

    AtomicI64 operator--(AtomicI64& atomic, int)
    {
        auto result = atomic;
        
        _InlineInterlockedDecrement64(&atomic.value);

        return result;
    }

    AtomicI64& operator+=(AtomicI64& atomic, int value)
    {
        _InlineInterlockedAdd64(&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI64& operator-=(AtomicI64& atomic, int value)
    {
        _InlineInterlockedAdd64(&atomic.value, (LONG)-value);

        return atomic;
    }

    AtomicI64& operator^=(AtomicI64& atomic, int value)
    {
        _InlineInterlockedXor64(&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI64& operator|=(AtomicI64& atomic, int value)
    {
        _InlineInterlockedOr64(&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI64& operator&=(AtomicI64& atomic, int value)
    {
        _InlineInterlockedAnd64(&atomic.value, (LONG)value);

        return atomic;
    }
}