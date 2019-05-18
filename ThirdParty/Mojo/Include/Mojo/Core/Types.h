#pragma once

inline namespace Mojo
{
#if _WIN32
    using int8      = char;
    using uint8     = unsigned char;
    using int16     = short;
    using uint16    = unsigned short;
    using int32     = long;
    using uint32    = unsigned long;
    using int64     = long long;
    using uint64    = unsigned long long;
#else
    using int8      = char;
    using uint8     = unsigned char;
    using int16     = short;
    using uint16    = unsigned short;
    using int32     = int;
    using uint32    = unsigned int;
    using int64     = long;
    using uint64    = unsigned long;
#endif

    using sbyte     = int8;
    using byte      = uint8;
    using word      = uint16;
    using dword     = uint32;

#if _WIN64
    using intptr    = int64;
    using uintptr   = uint64;
#else
    using intptr    = int32;
    using uintptr   = uint32;
#endif

    //using NullPtr   = decltype(nullptr);

    struct quat;
    struct float2;
    struct float3;
    struct float4;
    struct float4x4;

    struct float2
    {
        float x, y;

        constexpr float2(void)
            : x(0)
            , y(0)
        {
        }

        inline float2(float x, float y)
            : x(x)
            , y(y)
        {
        }

        float2(float s);
        float2(const float3& v);
        float2(const float4& v);
        float2(const quat& q);
    };

    struct float3
    {
        float x, y, z;

        constexpr float3(void)
            : x(0)
            , y(0)
            , z(0)
        {
        }

        inline float3(float x, float y, float z = 0.0f)
            : x(x)
            , y(y)
            , z(z)
        {
        }

        float3(float s);
        float3(const float4& v);
        float3(const quat& v);
        float3(const float2& v, float z = 0.0f);
    };

    struct float4
    {
        float x, y, z, w;

        constexpr float4(void)
            : x(0)
            , y(0)
            , z(0)
            , w(0)
        {
        }

        inline float4(float x, float y, float z, float w)
            : x(x)
            , y(y)
            , z(z)
            , w(w)
        {
        }

        float4(float s);
        float4(const quat& v);
        float4(const float3& v, float w);
        float4(const float2& v, float z, float w);

        float& operator[](int index)
        {
            return ((float*)this)[index];
        }

        float  operator[](int index) const
        {
            return ((float*)this)[index];
        }
    };

    struct quat
    {
        float x, y, z, w; 

        constexpr quat(void)
            : x(0)
            , y(0)
            , z(0)
            , w(0)
        {
        }

        inline quat(float x, float y, float z, float w)
            : x(x)
            , y(y)
            , z(z)
            , w(w)
        {
        }

        quat(float s);
        quat(const float4& v);
        quat(float x, float y, float z);
        quat(const float3& axis, float angle);
    };

    struct float4x4
    {
        float4 rows[4];

        constexpr float4x4(void) {}

        inline float4x4(float s)
        {
            rows[0] = float4(s, 0, 0, 0);
            rows[1] = float4(0, s, 0, 0);
            rows[2] = float4(0, 0, s, 0);
            rows[3] = float4(0, 0, 0, s);
        }

        inline float4x4(const float4& m0, const float4& m1, const float4& m2, const float4& m3)
        {
            rows[0] = m0;
            rows[1] = m1;
            rows[2] = m2;
            rows[3] = m3;
        }

        inline float4x4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33)
        {
            rows[0] = float4(m00, m01, m02, m03);
            rows[1] = float4(m10, m11, m12, m13);
            rows[2] = float4(m20, m21, m22, m23);
            rows[3] = float4(m30, m31, m32, m33);
        }

        float4& operator[](int index)
        {
            return rows[index];
        }

        const float4& operator[](int index) const
        {
            return rows[index];
        }
    };
}