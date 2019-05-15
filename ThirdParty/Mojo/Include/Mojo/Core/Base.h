#pragma once

/* This struct should not have vtable */
#define NOVTABLE __declspec(novtable)

/* Swap bytes in 16-bit value.  */
#define BITSWAP16(x) (((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))

/* Swap bytes in 32-bit value.  */
#define BITSWAP32(x)        					                \
    ((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >> 8)	\
    | (((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24))

/* Swap bytes in 64-bit value.  */
#define BITSWAP64(x)			                \
    ((((x) & 0xff00000000000000ull) >> 56)	    \
    | (((x) & 0x00ff000000000000ull) >> 40)	    \
    | (((x) & 0x0000ff0000000000ull) >> 24)	    \
    | (((x) & 0x000000ff00000000ull) >> 8)	    \
    | (((x) & 0x00000000ff000000ull) << 8)	    \
    | (((x) & 0x0000000000ff0000ull) << 24)	    \
    | (((x) & 0x000000000000ff00ull) << 40)	    \
    | (((x) & 0x00000000000000ffull) << 56))

#if defined(_WIN32) || defined(_ANDROID)
#   define LE_TO_NATIVE_16(x) (x)
#   define BE_TO_NATIVE_16(x) BITSWAP16(x)
#   define LE_TO_NATIVE_32(x) (x)
#   define BE_TO_NATIVE_32(x) BITSWAP32(x)
#   define LE_TO_NATIVE_64(x) (x)
#   define BE_TO_NATIVE_64(x) BITSWAP64(x)

#   define LE_TO_NATIVE_FLOAT(x) (x)
#   define BE_TO_NATIVE_FLOAT(x) SwapFloat(x)
#   define LE_TO_NATIVE_DOUBLE(x) (x)
#   define BE_TO_NATIVE_DOUBLE(x) SwapDouble(x)
#else
#   define LE_TO_NATIVE_16(x) BITSWAP16(x)
#   define BE_TO_NATIVE_16(x) (x)
#   define LE_TO_NATIVE_32(x) BITSWAP32(x)
#   define BE_TO_NATIVE_32(x) (x)
#   define LE_TO_NATIVE_64(x) BITSWAP64(x)
#   define BE_TO_NATIVE_64(x) (x)

#   define LE_TO_NATIVE_FLOAT(x) SwapFloat(x)
#   define BE_TO_NATIVE_FLOAT(x) (x)
#   define LE_TO_NATIVE_DOUBLE(x) SwapDouble(x)
#   define BE_TO_NATIVE_DOUBLE(x) (x)
#endif

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

    using byte      = uint8;
    using sbyte     = int8;

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

        explicit quat(float s);
        explicit quat(const float4& v);
        explicit quat(const float3& v, float z);
        explicit quat(const float2& v, float z, float w);

        static quat euler(float x, float y, float z);

        static float4 toaxis(const quat& q);
        static quat axisangle(const float3& axis, float angle);
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

    inline float2::float2(float s)
        : x(s)
        , y(s)
    {
    }

    inline float2::float2(const float3& v)
        : x(v.x)
        , y(v.y)
    {
    }

    inline float2::float2(const float4& v)
        : x(v.x)
        , y(v.y)
    {
    }

    inline float2::float2(const quat& q)
        : x(q.x)
        , y(q.y)
    {
    }

    inline float3::float3(float s)
        : x(s)
        , y(s)
        , z(s)
    {
    }

    inline float3::float3(const float2& v, float z)
        : x(v.x)
        , y(v.y)
        , z(z)
    {
    }

    inline float3::float3(const float4& v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
    {
    }

    inline float3::float3(const quat& q)
        : x(q.x)
        , y(q.y)
        , z(q.z)
    {
    }

    inline float4::float4(float s)
        : x(s)
        , y(s)
        , z(s)
        , w(s)
    {
    }

    inline float4::float4(const quat& q)
        : x(q.x)
        , y(q.y)
        , z(q.z)
        , w(q.w)
    {
    }

    inline float4::float4(const float3& v, float w)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(w)
    {
    }

    inline float4::float4(const float2& v, float z, float w)
        : x(v.x)
        , y(v.y)
        , z(z)
        , w(w)
    {
    }

    inline quat::quat(float s)
        : x(s)
        , y(s)
        , z(s)
        , w(s)
    {
    }

    inline quat::quat(const float4& v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(v.w)
    {
    }

    inline quat::quat(const float3& v, float w)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(w)
    {
    }

    inline quat::quat(const float2& v, float z, float w)
        : x(v.x)
        , y(v.y)
        , z(z)
        , w(w)
    {
    }

    inline float SwapFloat(float x)
    {
        union
        {
            float   f;
            uint32  u;
        } swapper;
        swapper.f = x;
        swapper.u = BITSWAP32(swapper.u);
        return swapper.f;
    }

    inline double SwapDouble(double x)
    {
        union
        {
            double  d;
            uint64  u;
        } swapper;
        swapper.d = x;
        swapper.u = BITSWAP64(swapper.u);
        return swapper.d;
    }

    inline float2 operator-(const float2& v)
    {
        return float2(-v.x, -v.y);
    }

    inline const float2& operator+(const float2& v)
    {
        return v;
    }

    inline float2& operator--(float2& v)
    {
        --v.x;
        --v.y;
        return v;
    }

    inline float2& operator++(float2& v)
    {
        ++v.x;
        ++v.y;
        return v;
    }

    inline float2 operator--(float2& v, int)
    {
        const float2 result = v;

        v.x--;
        v.y--;

        return result;
    }

    inline float2 operator++(float2& v, int)
    {
        const float2 result = v;

        v.x++;
        v.y++;

        return result;
    }

    inline float2 operator+(const float2& a, const float2& b)
    {
#if MATH_ENABLE_NEON   
        return float2(vadd_f32(a, b));
#else
        return float2(a.x + b.x, a.y + b.y);
#endif
    }

    inline float2 operator-(const float2& a, const float2& b)
    {
#if MATH_ENABLE_NEON   
        return float2(vsub_f32(a, b));
#else
        return float2(a.x - b.x, a.y - b.y);
#endif
    }

    inline float2 operator*(const float2& a, const float2& b)
    {
#if MATH_ENABLE_NEON   
        return float2(vmul_f32(a, b));
#else
        return float2(a.x * b.x, a.y * b.y);
#endif
    }

    inline float2 operator/(const float2& a, const float2& b)
    {
#if MATH_ENABLE_NEON && 0 // experimental
        float2 res;
        __asm volatile(
        "vcvt.f32.u32  q0, q0     \n\t"
            "vrecpe.f32    q0, q0     \n\t"
            "vmul.f32      q0, q0, q1 \n\t"
            "vcvt.u32.f32  q0, q0     \n\t"
            :
        : "r"(dst), "r"()
            :
            );
#else
        return float2(a.x / b.x, a.y / b.y);
#endif
    }

    inline float2 operator+(const float2& a, float b)
    {
        return a + float2(b);
    }

    inline float2 operator-(const float2& a, float b)
    {
        return a - float2(b);
    }

    inline float2 operator*(const float2& a, float b)
    {
        return a * float2(b);
    }

    inline float2 operator/(const float2& a, float b)
    {
        return a / float2(b);
    }

    inline float2 operator+(float a, const float2& b)
    {
        return float2(a) + b;
    }

    inline float2 operator-(float a, const float2& b)
    {
        return float2(a) - b;
    }

    inline float2 operator*(float a, const float2& b)
    {
        return float2(a) * b;
    }

    inline float2 operator/(float a, const float2& b)
    {
        return float2(a) / b;
    }

    inline float2& operator+=(float2& a, const float2& b)
    {
        return (a = a + b);
    }

    inline float2& operator+=(float2& a, float b)
    {
        return (a = a + b);
    }

    inline float2& operator-=(float2& a, const float2& b)
    {
        return (a = a - b);
    }

    inline float2& operator-=(float2& a, float b)
    {
        return (a = a - b);
    }

    inline float2& operator*=(float2& a, const float2& b)
    {
        return (a = a * b);
    }

    inline float2& operator*=(float2& a, float b)
    {
        return (a = a * b);
    }

    inline float2& operator/=(float2& a, const float2& b)
    {
        return (a = a / b);
    }

    inline float2& operator/=(float2& a, float b)
    {
        return (a = a + b);
    }

    inline bool operator==(const float2& a, const float2& b)
    {
        return a.x == b.x && a.y == b.y;
    }

    inline bool operator!=(const float2& a, const float2& b)
    {
        return a.x != b.x || a.y != b.y;
    }

    inline float3 operator-(const float3& v)
    {
        return float3(-v.x, -v.y, -v.z);
    }

    inline const float3& operator+(const float3& v)
    {
        return v;
    }

    inline float3& operator--(float3& v)
    {
        --v.x;
        --v.y;
        --v.z;
        return v;
    }

    inline float3& operator++(float3& v)
    {
        ++v.x;
        ++v.y;
        ++v.z;
        return v;
    }

    inline float3 operator--(float3& v, int)
    {
        const float3 result = v;

        v.x--;
        v.y--;
        v.z--;

        return result;
    }

    inline float3 operator++(float3& v, int)
    {
        const float3 result = v;

        v.x++;
        v.y++;
        v.z++;

        return result;
    }

    inline float3 operator+(const float3& a, const float3& b)
    {
        return float3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    inline float3 operator-(const float3& a, const float3& b)
    {
        return float3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    inline float3 operator*(const float3& a, const float3& b)
    {
        return float3(a.x * b.x, a.y * b.y, a.z * b.z);
    }

    inline float3 operator/(const float3& a, const float3& b)
    {
        return float3(a.x / b.x, a.y / b.y, a.z / b.z);
    }

    inline float3 operator+(const float3& a, float b)
    {
        return float3(a.x + b, a.y + b, a.z + b);
    }

    inline float3 operator-(const float3& a, float b)
    {
        return float3(a.x - b, a.y - b, a.z - b);
    }

    inline float3 operator*(const float3& a, float b)
    {
        return float3(a.x * b, a.y * b, a.z * b);
    }

    inline float3 operator/(const float3& a, float b)
    {
        return float3(a.x / b, a.y / b, a.z / b);
    }

    inline float3 operator+(float a, const float3& b)
    {
        return float3(a + b.x, a + b.y, a + b.z);
    }

    inline float3 operator-(float a, const float3& b)
    {
        return float3(a - b.x, a - b.y, a - b.z);
    }

    inline float3 operator*(float a, const float3& b)
    {
        return float3(a * b.x, a * b.y, a * b.z);
    }

    inline float3 operator/(float a, const float3& b)
    {
        return float3(a / b.x, a / b.y, a / b.z);
    }

    inline float3& operator+=(float3& a, const float3& b)
    {
        return (a = a + b);
    }

    inline float3& operator+=(float3& a, float b)
    {
        return (a = a + b);
    }

    inline float3& operator-=(float3& a, const float3& b)
    {
        return (a = a - b);
    }

    inline float3& operator-=(float3& a, float b)
    {
        return (a = a - b);
    }

    inline float3& operator*=(float3& a, const float3& b)
    {
        return (a = a * b);
    }

    inline float3& operator*=(float3& a, float b)
    {
        return (a = a * b);
    }

    inline float3& operator/=(float3& a, const float3& b)
    {
        return (a = a / b);
    }

    inline float3& operator/=(float3& a, float b)
    {
        return (a = a + b);
    }

    inline bool operator==(const float3& a, const float3& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }

    inline bool operator!=(const float3& a, const float3& b)
    {
        return a.x != b.x || a.y != b.y || a.z != b.z;
    }

    inline float4 operator-(const float4& v)
    {
        return float4(-v.x, -v.y, -v.z, -v.w);
    }

    inline const float4& operator+(const float4& v)
    {
        return v;
    }

    inline float4& operator--(float4& v)
    {
        --v.x;
        --v.y;
        --v.z;
        --v.w;
        return v;
    }

    inline float4& operator++(float4& v)
    {
        ++v.x;
        ++v.y;
        ++v.z;
        ++v.w;
        return v;
    }

    inline float4 operator--(float4& v, int)
    {
        const float4 result = v;

        v.x--;
        v.y--;
        v.z--;
        v.w--;

        return result;
    }

    inline float4 operator++(float4& v, int)
    {
        const float4 result = v;

        v.x++;
        v.y++;
        v.z++;
        v.w++;

        return result;
    }

    inline float4 operator+(const float4& a, const float4& b)
    {
        return float4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    inline float4 operator-(const float4& a, const float4& b)
    {
        return float4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }

    inline float4 operator*(const float4& a, const float4& b)
    {
        return float4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
    }

    inline float4 operator/(const float4& a, const float4& b)
    {
        return float4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
    }

    inline float4 operator+(const float4& a, float b)
    {
        return float4(a.x + b, a.y + b, a.z + b, a.w + b);
    }

    inline float4 operator-(const float4& a, float b)
    {
        return float4(a.x - b, a.y - b, a.z - b, a.w - b);
    }

    inline float4 operator*(const float4& a, float b)
    {
        return float4(a.x * b, a.y * b, a.z * b, a.w * b);
    }

    inline float4 operator/(const float4& a, float b)
    {
        return float4(a.x / b, a.y / b, a.z / b, a.w / b);
    }

    inline float4 operator+(float a, const float4& b)
    {
        return float4(a + b.x, a + b.y, a + b.z, a + b.w);
    }

    inline float4 operator-(float a, const float4& b)
    {
        return float4(a - b.x, a - b.y, a - b.z, a - b.w);
    }

    inline float4 operator*(float a, const float4& b)
    {
        return float4(a * b.x, a * b.y, a * b.z, a * b.w);
    }

    inline float4 operator/(float a, const float4& b)
    {
        return float4(a / b.x, a / b.y, a / b.z, a / b.w);
    }

    inline float4& operator+=(float4& a, const float4& b)
    {
        return (a = a + b);
    }

    inline float4& operator+=(float4& a, float b)
    {
        return (a = a + b);
    }

    inline float4& operator-=(float4& a, const float4& b)
    {
        return (a = a - b);
    }

    inline float4& operator-=(float4& a, float b)
    {
        return (a = a - b);
    }

    inline float4& operator*=(float4& a, const float4& b)
    {
        return (a = a * b);
    }

    inline float4& operator*=(float4& a, float b)
    {
        return (a = a * b);
    }

    inline float4& operator/=(float4& a, const float4& b)
    {
        return (a = a / b);
    }

    inline float4& operator/=(float4& a, float b)
    {
        return (a = a + b);
    }

    inline bool operator==(const float4& a, const float4& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }

    inline bool operator!=(const float4& a, const float4& b)
    {
        return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
    }
    inline quat operator-(const quat& v)
    {
        return quat(-v.x, -v.y, -v.z, -v.w);
    }

    inline const quat& operator+(const quat& v)
    {
        return v;
    }

    inline quat& operator--(quat& v)
    {
        --v.x;
        --v.y;
        --v.z;
        --v.w;
        return v;
    }

    inline quat& operator++(quat& v)
    {
        ++v.x;
        ++v.y;
        ++v.z;
        ++v.w;
        return v;
    }

    inline quat operator--(quat& v, int)
    {
        const quat result = v;

        v.x--;
        v.y--;
        v.z--;
        v.w--;

        return result;
    }

    inline quat operator++(quat& v, int)
    {
        const quat result = v;

        v.x++;
        v.y++;
        v.z++;
        v.w++;

        return result;
    }

    inline quat operator+(const quat& a, const quat& b)
    {
        return quat(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    inline quat operator-(const quat& a, const quat& b)
    {
        return quat(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }

    inline quat operator*(const quat& a, const quat& b)
    {
        return quat(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
    }

    inline quat operator/(const quat& a, const quat& b)
    {
        return quat(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
    }

    inline quat operator+(const quat& a, float b)
    {
        return quat(a.x + b, a.y + b, a.z + b, a.w + b);
    }

    inline quat operator-(const quat& a, float b)
    {
        return quat(a.x - b, a.y - b, a.z - b, a.w - b);
    }

    inline quat operator*(const quat& a, float b)
    {
        return quat(a.x * b, a.y * b, a.z * b, a.w * b);
    }

    inline quat operator/(const quat& a, float b)
    {
        return quat(a.x / b, a.y / b, a.z / b, a.w / b);
    }

    inline quat operator+(float a, const quat& b)
    {
        return quat(a + b.x, a + b.y, a + b.z, a + b.w);
    }

    inline quat operator-(float a, const quat& b)
    {
        return quat(a - b.x, a - b.y, a - b.z, a - b.w);
    }

    inline quat operator*(float a, const quat& b)
    {
        return quat(a * b.x, a * b.y, a * b.z, a * b.w);
    }

    inline quat operator/(float a, const quat& b)
    {
        return quat(a / b.x, a / b.y, a / b.z, a / b.w);
    }

    inline quat& operator+=(quat& a, const quat& b)
    {
        return (a = a + b);
    }

    inline quat& operator+=(quat& a, float b)
    {
        return (a = a + b);
    }

    inline quat& operator-=(quat& a, const quat& b)
    {
        return (a = a - b);
    }

    inline quat& operator-=(quat& a, float b)
    {
        return (a = a - b);
    }

    inline quat& operator*=(quat& a, const quat& b)
    {
        return (a = a * b);
    }

    inline quat& operator*=(quat& a, float b)
    {
        return (a = a * b);
    }

    inline quat& operator/=(quat& a, const quat& b)
    {
        return (a = a / b);
    }

    inline quat& operator/=(quat& a, float b)
    {
        return (a = a + b);
    }

    inline bool operator==(const quat& a, const quat& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }

    inline bool operator!=(const quat& a, const quat& b)
    {
        return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
    }

    inline bool operator==(const float4x4& a, const float4x4& b)
    {
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
    }

    inline bool operator!=(const float4x4& a, const float4x4& b)
    {
        return a[0] == b[0] || a[1] == b[1] || a[2] == b[2] || a[3] == b[3];
    }

    inline float4x4 operator-(const float4x4& m)
    {
        float4x4 result;
        result[0] = -m[0];
        result[1] = -m[1];
        result[2] = -m[2];
        result[3] = -m[3];
        return result;
    }

    inline const float4x4& operator+(const float4x4& m)
    {
        return m;
    }

    inline float4x4& operator--(float4x4& m)
    {
        --m[0];
        --m[1];
        --m[2];
        --m[3];
        return m;
    }

    inline float4x4& operator++(float4x4& m)
    {
        ++m[0];
        ++m[1];
        ++m[2];
        ++m[3];
        return m;
    }

    inline const float4x4& operator--(float4x4& m, int)
    {
        m[0]--;
        m[1]--;
        m[2]--;
        m[3]--;
        return m;
    }

    inline const float4x4& operator++(float4x4& m, int)
    {
        m[0]++;
        m[1]++;
        m[2]++;
        m[3]++;
        return m;
    }

    inline float4x4 operator+(const float4x4& a, const float4x4& b)
    {
        float4x4 result;
        result[0] = a[0] + b[0];
        result[1] = a[1] + b[1];
        result[2] = a[2] + b[2];
        result[3] = a[3] + b[3];
        return result;
    }

    inline float4x4 operator+(const float4x4& a, float b)
    {
        float4x4 result;
        result[0] = a[0] + b;
        result[1] = a[1] + b;
        result[2] = a[2] + b;
        result[3] = a[3] + b;
        return result;
    }

    inline float4x4 operator+(float a, const float4x4& b)
    {
        float4x4 result;
        result[0] = a + b[0];
        result[1] = a + b[1];
        result[2] = a + b[2];
        result[3] = a + b[3];
        return result;
    }

    inline float4x4 operator-(const float4x4& a, const float4x4& b)
    {
        float4x4 result;
        result[0] = a[0] - b[0];
        result[1] = a[1] - b[1];
        result[2] = a[2] - b[2];
        result[3] = a[3] - b[3];
        return result;
    }

    inline float4x4 operator-(const float4x4& a, float b)
    {
        float4x4 result;
        result[0] = a[0] - b;
        result[1] = a[1] - b;
        result[2] = a[2] - b;
        result[3] = a[3] - b;
        return result;
    }

    inline float4x4 operator-(float a, const float4x4& b)
    {
        float4x4 result;
        result[0] = a - b[0];
        result[1] = a - b[1];
        result[2] = a - b[2];
        result[3] = a - b[3];
        return result;
    }

    inline float4x4 operator*(const float4x4& a, float b)
    {
        float4x4 result;
        result[0] = a[0] * b;
        result[1] = a[1] * b;
        result[2] = a[2] * b;
        result[3] = a[3] * b;
        return result;
    }

    inline float4x4 operator*(float a, const float4x4& b)
    {
        float4x4 result;
        result[0] = a * b[0];
        result[1] = a * b[1];
        result[2] = a * b[2];
        result[3] = a * b[3];
        return result;
    }

    inline float4x4 operator*(const float4x4& a, const float4x4& b)
    {
        float4x4 result;
        result[0] = a[0] * b[0];
        result[1] = a[1] * b[1];
        result[2] = a[2] * b[2];
        result[3] = a[3] * b[3];
        return result;
    }

    inline float4x4 operator/(const float4x4& a, const float4x4& b)
    {
        float4x4 result;
        result[0] = a[0] / b[0];
        result[1] = a[1] / b[1];
        result[2] = a[2] / b[2];
        result[3] = a[3] / b[3];
        return result;
    }

    inline float4x4 operator/(const float4x4& a, float b)
    {
        float4x4 result;
        result[0] = a[0] / b;
        result[1] = a[1] / b;
        result[2] = a[2] / b;
        result[3] = a[3] / b;
        return result;
    }

    inline float4x4 operator/(float a, const float4x4& b)
    {
        float4x4 result;
        result[0] = a / b[0];
        result[1] = a / b[1];
        result[2] = a / b[2];
        result[3] = a / b[3];
        return result;
    }

    inline float4x4& operator+=(float4x4& a, const float4x4& b)
    {
        return (a = a + b);
    }

    inline float4x4& operator+=(float4x4& a, float b)
    {
        return (a = a + b);
    }

    inline float4x4& operator-=(float4x4& a, const float4x4& b)
    {
        return (a = a - b);
    }

    inline float4x4& operator-=(float4x4& a, float b)
    {
        return (a = a - b);
    }

    inline float4x4& operator*=(float4x4& a, const float4x4& b)
    {
        return (a = a * b);
    }

    inline float4x4& operator*=(float4x4& a, float b)
    {
        return (a = a * b);
    }

    inline float4x4& operator/=(float4x4& a, const float4x4& b)
    {
        return (a = a / b);
    }

    inline float4x4& operator/=(float4x4& a, float b)
    {
        return (a = a + b);
    }
}