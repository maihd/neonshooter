#pragma once

inline namespace Mojo
{
    struct AtomicI32
    {
        volatile int value;

        AtomicI32(int value = 0)
            : value(value)
        {
        }

        operator int(void) const
        {
            return value;
        }

        AtomicI32& operator=(int value);
        AtomicI32& operator=(const AtomicI32& other);
    };

    struct AtomicI64
    {
        volatile long long value;

        AtomicI64(long long value = 0)
            : value(value)
        {
        }

        operator long long(void) const
        {
            return value;
        }

        AtomicI64& operator=(int value);
        AtomicI64& operator=(const AtomicI64& other);
    };

    AtomicI32& operator++(AtomicI32& atomic);
    AtomicI32& operator--(AtomicI32& atomic);
    AtomicI32  operator++(AtomicI32& atomic, int);
    AtomicI32  operator--(AtomicI32& atomic, int);
    AtomicI32& operator+=(AtomicI32& atomic, int value);
    AtomicI32& operator-=(AtomicI32& atomic, int value);
    AtomicI32& operator^=(AtomicI32& atomic, int value);
    AtomicI32& operator|=(AtomicI32& atomic, int value);
    AtomicI32& operator&=(AtomicI32& atomic, int value);

    AtomicI64& operator++(AtomicI64& atomic);
    AtomicI64& operator--(AtomicI64& atomic);
    AtomicI64  operator++(AtomicI64& atomic, int);
    AtomicI64  operator--(AtomicI64& atomic, int);
    AtomicI64& operator+=(AtomicI64& atomic, long long value);
    AtomicI64& operator-=(AtomicI64& atomic, long long value);
    AtomicI64& operator^=(AtomicI64& atomic, long long value);
    AtomicI64& operator|=(AtomicI64& atomic, long long value);
    AtomicI64& operator&=(AtomicI64& atomic, long long value);
}