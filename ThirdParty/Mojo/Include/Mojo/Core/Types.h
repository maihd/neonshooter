#pragma once

inline namespace Mojo
{
#if _WIN32
    using I8    = char;
    using U8    = unsigned char;
    using I16   = short;
    using U16   = unsigned short;
    using I32   = long;
    using U32   = unsigned long;
    using I64   = long long;
    using U64   = unsigned long long;
#else
    using I8    = char;
    using U8    = unsigned char;
    using I16   = short;
    using U16   = unsigned short;
    using I32   = int;
    using U32   = unsigned int;
    using I64   = long;
    using U64   = unsigned long;
#endif

#if _WIN64
    using IPtr  = I64;
    using UPtr  = U64;
#else
    using IPtr  = I32;
    using UPtr  = U32;
#endif

    using Handle    = U32;
    using NullPtr   = decltype(nullptr);

    //----------------------------------
    // Math types
    //----------------------------------

    struct Vector2;
    struct Vector3;
    struct Vector4;
    struct Matrix4;
    struct Quaterion;

    struct Vector2
    {
        float x, y;

        constexpr Vector2(void)
            : x(0)
            , y(0)
        {
        }

        inline Vector2(float x, float y)
            : x(x)
            , y(y)
        {
        }

        Vector2(float s);
        Vector2(const Vector3& v);
        Vector2(const Vector4& v);
        Vector2(const Quaterion& q);
    };

    struct Vector3
    {
        float x, y, z;

        constexpr Vector3(void)
            : x(0)
            , y(0)
            , z(0)
        {
        }

        inline Vector3(float x, float y, float z = 0.0f)
            : x(x)
            , y(y)
            , z(z)
        {
        }

        Vector3(float s);
        Vector3(const Vector4& v);
        Vector3(const Quaterion& v);
        Vector3(const Vector2& v, float z = 0.0f);
    };

    struct Vector4
    {
        float x, y, z, w;

        constexpr Vector4(void)
            : x(0)
            , y(0)
            , z(0)
            , w(0)
        {
        }

        inline Vector4(float x, float y, float z, float w)
            : x(x)
            , y(y)
            , z(z)
            , w(w)
        {
        }

        Vector4(float s);
        Vector4(const Quaterion& v);
        Vector4(const Vector3& v, float w);
        Vector4(const Vector2& v, float z, float w);

        float& operator[](int index)
        {
            return ((float*)this)[index];
        }

        float  operator[](int index) const
        {
            return ((float*)this)[index];
        }
    };

    struct Quaterion
    {
        float x, y, z, w; 

        constexpr Quaterion(void)
            : x(0)
            , y(0)
            , z(0)
            , w(0)
        {
        }

        inline Quaterion(float x, float y, float z, float w)
            : x(x)
            , y(y)
            , z(z)
            , w(w)
        {
        }

        Quaterion(float s);
        Quaterion(const Vector4& v);
        Quaterion(float x, float y, float z);
        Quaterion(const Vector3& axis, float angle);
    };

    struct Matrix4
    {
        Vector4 rows[4];

        constexpr Matrix4(void) {}

        inline Matrix4(float s)
        {
            rows[0] = Vector4(s, 0, 0, 0);
            rows[1] = Vector4(0, s, 0, 0);
            rows[2] = Vector4(0, 0, s, 0);
            rows[3] = Vector4(0, 0, 0, s);
        }

        inline Matrix4(const Vector4& m0, const Vector4& m1, const Vector4& m2, const Vector4& m3)
        {
            rows[0] = m0;
            rows[1] = m1;
            rows[2] = m2;
            rows[3] = m3;
        }

        inline Matrix4(
            float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33
        )
        {
            rows[0] = Vector4(m00, m01, m02, m03);
            rows[1] = Vector4(m10, m11, m12, m13);
            rows[2] = Vector4(m20, m21, m22, m23);
            rows[3] = Vector4(m30, m31, m32, m33);
        }

        Vector4& operator[](int index)
        {
            return rows[index];
        }

        const Vector4& operator[](int index) const
        {
            return rows[index];
        }
    };

    //----------------------------
    // Graphics types
    //----------------------------
    struct Color
    {
        U8 r, g, b, a;
    };

    //----------------------------
    // Atomic types
    //----------------------------

    struct AtomicI32
    {
        volatile I32 value;

        AtomicI32(I32 value = 0)
            : value(value)
        {
        }

        operator I32(void) const
        {
            return value;
        }

        AtomicI32& operator=(I32 value);
        AtomicI32& operator=(const AtomicI32& other);
    };

    struct AtomicI64
    {
        volatile I64 value;

        AtomicI64(I64 value = 0)
            : value(value)
        {
        }

        operator I64(void) const
        {
            return value;
        }

        AtomicI64& operator=(I64 value);
        AtomicI64& operator=(const AtomicI64& other);
    };

    AtomicI32& operator++(AtomicI32& atomic);
    AtomicI32& operator--(AtomicI32& atomic);
    AtomicI32  operator++(AtomicI32& atomic, int);
    AtomicI32  operator--(AtomicI32& atomic, int);
    AtomicI32& operator+=(AtomicI32& atomic, I32 value);
    AtomicI32& operator-=(AtomicI32& atomic, I32 value);
    AtomicI32& operator^=(AtomicI32& atomic, I32 value);
    AtomicI32& operator|=(AtomicI32& atomic, I32 value);
    AtomicI32& operator&=(AtomicI32& atomic, I32 value);

    AtomicI64& operator++(AtomicI64& atomic);
    AtomicI64& operator--(AtomicI64& atomic);
    AtomicI64  operator++(AtomicI64& atomic, int);
    AtomicI64  operator--(AtomicI64& atomic, int);
    AtomicI64& operator+=(AtomicI64& atomic, I64 value);
    AtomicI64& operator-=(AtomicI64& atomic, I64 value);
    AtomicI64& operator^=(AtomicI64& atomic, I64 value);
    AtomicI64& operator|=(AtomicI64& atomic, I64 value);
    AtomicI64& operator&=(AtomicI64& atomic, I64 value);
}
