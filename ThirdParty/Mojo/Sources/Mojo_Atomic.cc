#include <Mojo/Atomic.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

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
        _InterlockedAdd((volatile LONG*)&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI32& operator-=(AtomicI32& atomic, int value)
    {
        _InterlockedAdd((volatile LONG*)&atomic.value, (LONG)-value);

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
        _InterlockedExchange64(&this->value, value);
        return *this;
    }

    AtomicI64& AtomicI64::operator=(const AtomicI64& other)
    {
        _InterlockedExchange64(&this->value, other.value);
        return *this;
    }

    AtomicI64& operator++(AtomicI64& atomic)
    {
        _InterlockedIncrement64(&atomic.value);

        return atomic;
    }

    AtomicI64& operator--(AtomicI64& atomic)
    {
        _InterlockedDecrement64(&atomic.value);
        return atomic;
    }

    AtomicI64 operator++(AtomicI64& atomic, int)
    {
        AtomicI32 result = atomic;

        _InterlockedIncrement64(&atomic.value);

        return result;
    }

    AtomicI64 operator--(AtomicI64& atomic, int)
    {
        auto result = atomic;

        _InterlockedDecrement64(&atomic.value);

        return result;
    }

    AtomicI64& operator+=(AtomicI64& atomic, int value)
    {
        _InterlockedAdd64(&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI64& operator-=(AtomicI64& atomic, int value)
    {
        _InterlockedAdd64(&atomic.value, (LONG)-value);

        return atomic;
    }

    AtomicI64& operator^=(AtomicI64& atomic, int value)
    {
        _InterlockedXor64(&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI64& operator|=(AtomicI64& atomic, int value)
    {
        _InterlockedOr64(&atomic.value, (LONG)value);

        return atomic;
    }

    AtomicI64& operator&=(AtomicI64& atomic, int value)
    {
        _InterlockedAnd64(&atomic.value, (LONG)value);

        return atomic;
    }
}