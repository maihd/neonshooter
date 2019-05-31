#pragma once

#include <math.h>
#include <Mojo/Core/Types.h>

#if defined(__SSSE3__)
#   define SSE_SUPPORT 1
#endif

#if defined(__SSE__) || defined(__SSE2__) || defined(__SSE3__)
#   undef  SSE_SUPPORT
#   define SSE_SUPPORT 1
#endif

#if defined(__SSE4_1__) || defined(__SSE4_2__) || defined(__SSE_MATH__)
#   undef  SSE_SUPPORT
#   define SSE_SUPPORT 1
#endif

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_IX64))
#   if defined(_M_HYBRID_X86_ARM64)
#       define SSE_SUPPORT 0
#   else
#       define SSE_SUPPORT 1
#   endif
#endif

#if SSE_SUPPORT
#   include <mmintrin.h>
//# include <smmintrin.h>
//# include <xmmintrin.h>
#   include <emmintrin.h>
#endif

//
// @region: Math extensions
//

inline int min(int x, int y)
{
    return x < y ? x : y;
}

inline int max(int x, int y)
{
    return x < y ? x : y;
}

inline int clamp(int x, int min, int max)
{
    return x < min ? min : (x > max ? max : x);
}

// Computes sign of 'x'
inline float signf(float x)
{
    return x < 0.0f ? -1.0f : x == 0.0f ? 0 : 1.0f;
}

/* Get the fractal part of floating point
*/
inline float fracf(float x)
{
    return modff(x, 0);
}

/* Get the smaller value
 */
inline float minf(float x, float y)
{
    return x < y ? x : y;
}

/* Get the larger value
 */
inline float maxf(float x, float y)
{
    return x > y ? x : y;
}

/* Clamps the 'x' value to the [min, max].
 */
inline float clampf(float x, float min, float max)
{
    return x < min ? min : (x > max ? max : x);
}

/* Clamps the specified value within the range of 0 to 1
 */
inline float saturatef(float x)
{
    return clampf(x, 0.0f, 1.0f);
}

/* Compares two values, returning 0 or 1 based on which value is greater.
 */
inline float stepf(float y, float x)
{
    return x >= y;
}

/* Performs a linear interpolation.
 */
inline float lerpf(float x, float y, float s)
{
    return x + (y - x) * s;
}

/* Compute a smooth Hermite interpolation
 * @return: 0 if x <= min
 *          1 if x >= max
 *          (0, 1) otherwise
 */
inline float smoothstepf(float min, float max, float x)
{
    return (clampf(x, min, max) - min) / (max - min);
}

/* Computes inverse square root of 'x'.
 */
inline float rsqrtf(float x)
{
    return 1.0f / sqrtf(x);
}

/* Computes fast inverse square root of 'x'.
 */
inline float frsqrtf(float x)
{
    union
    {
        float f;
        int   i;
    } cvt;

    cvt.f = x;
    cvt.i = 0x5F3759DF - (cvt.i >> 1);
    cvt.f = cvt.f * (1.5f - (0.5f * x * cvt.f * cvt.f));
    return cvt.f;
}

/* Computes fast inverse square root of 'x'.
 */
inline float fsqrtf(float x)
{
    return x == 0.0f ? 0.0f : 1.0f / frsqrtf(x);
}

inline namespace Mojo
{
    struct quat;
    struct float2;
    struct float3;
    struct float4;
    struct float4x4;

    namespace Math
    {
        float4x4 Transform(const float2& position, float rotation, const float2& scale);
        float4x4 Transform(const float3& position, const quat& rotation, const float3& scale);

        float4x4 Scalation(float s);
        float4x4 Scalation(const float2& v);
        float4x4 Scalation(const float3& v);
        float4x4 Scalation(float x, float y, float z = 1.0f);

        float4x4 Translation(const float2& v);
        float4x4 Translation(const float3& v);
        float4x4 Translation(float x, float y, float z = 0.0f);

        float4x4 Rotation(const quat& quaternion);
        float4x4 Rotation(const float3& axis, float angle);
        float4x4 Rotation(float x, float y, float z, float angle);

        float4x4 RotationX(float angle);
        float4x4 RotationY(float angle);
        float4x4 RotationZ(float angle);

        float4x4 Lookat(const float3& eye, const float3& target, const float3& up);

        float4x4 Ortho(float l, float r, float b, float t, float n, float f);
        float4x4 Frustum(float l, float r, float b, float t, float n, float f);
        float4x4 Perspective(float fov, float aspect, float znear, float zfar);

        void Decompose(const float4x4& m, float3* scalation, quat* quaternion, float3* translation);
        void Decompose(const float4x4& m, float3* scalation, float3* axis, float* angle, float3* translation);
    }

    //
    // @region: Constructors
    //

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

    inline quat::quat(float x, float y, float z)
    {
        float r;
        float p;

        r = z * 0.5f;
        p = x * 0.5f;
        y = y * 0.5f; // Now y mean yaw

        const float c1 = cosf(y);
        const float c2 = cosf(p);
        const float c3 = cosf(r);
        const float s1 = sinf(y);
        const float s2 = sinf(p);
        const float s3 = sinf(r);

        this->x = s1 * s2 * c3 + c1 * c2 * s3;
        this->y = s1 * c2 * c3 + c1 * s2 * s3;
        this->z = c1 * s2 * c3 - s1 * c2 * s3;
        this->w = c1 * c2 * c3 - s1 * s2 * s3;
    }

    inline quat::quat(const float3& axis, float angle)
    {
        const float lsqr = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
        if (lsqr == 0.0f)
        {
            this->x = 0.0f;
            this->y = 0.0f;
            this->z = 0.0f;
            this->w = 1.0f;
        }
        else
        {
            float f = 1.0f / lsqr * sinf(angle * 0.5f);

            this->x = axis.x * f;
            this->y = axis.y * f;
            this->z = axis.z * f;
            this->w = cosf(angle * 0.5f);
        }
    }

    //
    // @region: Operators
    //

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

    // Computes sign of 'x'
    inline float2 sign(const float2& v)
    {
        return float2(signf(v.x), signf(v.y));
    }

    /* Computes absolute value
     */
    inline float2 abs(const float2& v)
    {
        return float2(fabsf(v.x), fabsf(v.y));
    }

    /* Computes cosine
     */
    inline float2 cos(const float2& v)
    {
        return float2(cosf(v.x), cosf(v.y));
    }

    /* Computes sine
     */
    inline float2 sin(const float2& v)
    {
        return float2(sinf(v.x), sinf(v.y));
    }

    /* Computes tangent
     */
    inline float2 tan(const float2& v)
    {
        return float2(tanf(v.x), tanf(v.y));
    }

    /* Computes hyperbolic cosine
     */
    inline float2 cosh(const float2& v)
    {
        return float2(coshf(v.x), coshf(v.y));
    }

    /* Computes hyperbolic sine
     */
    inline float2 sinh(const float2& v)
    {
        return float2(sinhf(v.x), sinhf(v.y));
    }

    /* Computes hyperbolic tangent
     */
    inline float2 tanh(const float2& v)
    {
        return float2(tanhf(v.x), tanhf(v.y));
    }

    /* Computes inverse cosine
     */
    inline float2 acos(const float2& v)
    {
        return float2(acosf(v.x), acosf(v.y));
    }

    /* Computes inverse sine
     */
    inline float2 asin(const float2& v)
    {
        return float2(asinf(v.x), asinf(v.y));
    }

    /* Computes inverse tangent
     */
    inline float2 atan(const float2& v)
    {
        return float2(atanf(v.x), atanf(v.y));
    }

    /* Computes inverse tangent with 2 args
     */
    inline float2 atan2(const float2& a, const float2& b)
    {
        return float2(atan2f(a.x, b.x), atan2f(a.y, b.y));
    }

    /* Computes Euler number raised to the power 'x'
     */
    inline float2 exp(const float2& v)
    {
        return float2(expf(v.x), expf(v.y));
    }

    /* Computes 2 raised to the power 'x'
     */
    inline float2 exp2(const float2& v)
    {
        return float2(exp2f(v.x), exp2f(v.y));
    }

    /* Computes the base Euler number logarithm
     */
    inline float2 log(const float2& v)
    {
        return float2(logf(v.x), logf(v.y));
    }

    /* Computes the base 2 logarithm
     */
    inline float2 log2(const float2& v)
    {
        return float2(log2f(v.x), log2f(v.y));
    }

    /* Computes the base 10 logarithm
     */
    inline float2 log10(const float2& v)
    {
        return float2(log10f(v.x), log10f(v.y));
    }

    /* Computes the value of base raised to the power exponent
     */
    inline float2 pow(const float2& a, const float2& b)
    {
        return float2(powf(a.x, b.x), powf(a.y, b.y));
    }

    /* Get the fractal part of floating point
     */
    inline float2 frac(const float2& v)
    {
        return float2(fracf(v.x), fracf(v.y));
    }

    /* Computes the floating-point remainder of the division operation x/y
     */
    inline float2 fmod(const float2& a, const float2& b)
    {
        return float2(fmodf(a.x, b.x), fmodf(a.y, b.y));
    }

    /* Computes the smallest integer value not less than 'x'
     */
    inline float2 ceil(const float2& v)
    {
        return float2(ceilf(v.x), ceilf(v.y));
    }

    /* Computes the largest integer value not greater than 'x'
     */
    inline float2 floor(const float2& v)
    {
        return float2(floorf(v.x), floorf(v.y));
    }

    /* Computes the nearest integer value
     */
    inline float2 round(const float2& v)
    {
        return float2(roundf(v.x), roundf(v.y));
    }

    /* Computes the nearest integer not greater in magnitude than 'x'
     */
    inline float2 trunc(const float2& v)
    {
        return float2(truncf(v.x), truncf(v.y));
    }

    /* Get the smaller value
     */
    inline float2 min(const float2& a, const float2& b)
    {
        return float2(minf(a.x, b.x), minf(a.y, b.y));
    }

    /* Get the larger value
     */
    inline float2 max(const float2& a, const float2& b)
    {
        return float2(maxf(a.x, b.x), maxf(a.y, b.y));
    }

    /* Clamps the 'x' value to the [min, max].
     */
    inline float2 clamp(const float2& v, const float2& min, const float2& max)
    {
        return float2(clampf(v.x, min.x, max.x), clampf(v.y, min.y, max.y));
    }

    /* Clamps the specified value within the range of 0 to 1
     */
    inline float2 saturate(const float2& v)
    {
        return float2(saturatef(v.x), saturatef(v.y));
    }

    /* Compares two values, returning 0 or 1 based on which value is greater.
     */
    inline float2 step(const float2& a, const float2& b)
    {
        return float2(stepf(a.x, b.x), stepf(a.y, b.y));
    }

    /* Performs a linear interpolation.
     */
    inline float2 lerp(const float2& a, const float2& b, const float2& t)
    {
        return float2(lerpf(a.x, b.x, t.x), lerpf(a.y, b.y, t.y));
    }

    /* Performs a linear interpolation.
     */
    inline float2 lerp(const float2& a, const float2& b, float t)
    {
        return float2(lerpf(a.x, b.x, t), lerpf(a.y, b.y, t));
    }

    /* Compute a smooth Hermite interpolation
     */
    inline float2 smoothstep(const float2& a, const float2& b, const float2& t)
    {
        return float2(smoothstepf(a.x, b.x, t.x), smoothstepf(a.y, b.y, t.y));
    }

    /* Computes square root of 'x'.
     */
    inline float2 sqrt(const float2& v)
    {
        return float2(sqrtf(v.x), sqrtf(v.y));
    }

    /* Computes inverse square root of 'x'.
     */
    inline float2 rsqrt(const float2& v)
    {
        return float2(rsqrtf(v.x), rsqrtf(v.y));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline float2 fsqrt(const float2& v)
    {
        return float2(fsqrtf(v.x), fsqrtf(v.y));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline float2 frsqrt(const float2& v)
    {
        return float2(frsqrtf(v.x), frsqrtf(v.y));
    }

    //
    // @region: Graphics funtions
    //

    /* Compute dot product of two vectors
     */
    inline float dot(const float2& a, const float2& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    /* Compute squared length of vector
     */
    inline float lensqr(const float2& v)
    {
        return dot(v, v);
    }

    /* Compute length of vector
     */
    inline float length(const float2& v)
    {
        return sqrtf(lensqr(v));
    }

    /* Compute distance from 'a' to b
     */
    inline float distance(const float2& a, const float2& b)
    {
        return length(a - b);
    }

    /* Compute squared distance from 'a' to b
     */
    inline float distsqr(const float2& a, const float2& b)
    {
        return lensqr(a - b);
    }

    /* Compute normalized vector
     */
    inline float2 normalize(const float2& v)
    {
        const float lsqr = lensqr(v);
        if (lsqr > 0.0f)
        {
            const float f = rsqrtf(lsqr);
            return float2(v.x * f, v.y * f);
        }
        else
        {
            return v;
        }
    }

    /* Compute reflection vector
     */
    inline float2 reflect(const float2& v, const float2& n)
    {
        return v - 2.0f * dot(v, n) * n;
    }

    /* Compute refraction vector
     */
    inline float2 refract(const float2& v, const float2& n, float eta)
    {
        const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
        return k < 0.0f
            ? float2(0.0f)
            : eta * v - (eta * dot(v, n) + sqrtf(k)) * v;
    }

    /* Compute faceforward vector
     */
    inline float2 faceforward(const float2& n, const float2& i, const float2& nref)
    {
        return dot(i, nref) < 0.0f ? n : -n;
    }
    /* Computes sign of 'x'
 */
    inline float3 sign(const float3& v)
    {
        return float3(signf(v.x), signf(v.y), signf(v.z));
    }

    /* Computes absolute value
     */
    inline float3 abs(const float3& v)
    {
        return float3(fabsf(v.x), fabsf(v.y), fabsf(v.z));
    }

    /* Computes cosine
     */
    inline float3 cos(const float3& v)
    {
        return float3(cosf(v.x), cosf(v.y), cosf(v.z));
    }

    /* Computes sine
     */
    inline float3 sin(const float3& v)
    {
        return float3(sinf(v.x), sinf(v.y), sinf(v.z));
    }

    /* Computes tangent
     */
    inline float3 tan(const float3& v)
    {
        return float3(tanf(v.x), tanf(v.y), tanf(v.z));
    }

    /* Computes hyperbolic cosine
     */
    inline float3 cosh(const float3& v)
    {
        return float3(coshf(v.x), coshf(v.y), coshf(v.z));
    }

    /* Computes hyperbolic sine
     */
    inline float3 sinh(const float3& v)
    {
        return float3(sinhf(v.x), sinhf(v.y), sinhf(v.z));
    }

    /* Computes hyperbolic tangent
     */
    inline float3 tanh(const float3& v)
    {
        return float3(tanhf(v.x), tanhf(v.y), tanhf(v.z));
    }

    /* Computes inverse cosine
     */
    inline float3 acos(const float3& v)
    {
        return float3(acosf(v.x), acosf(v.y), acosf(v.z));
    }

    /* Computes inverse sine
     */
    inline float3 asin(const float3& v)
    {
        return float3(asinf(v.x), asinf(v.y), asinf(v.z));
    }

    /* Computes inverse tangent
     */
    inline float3 atan(const float3& v)
    {
        return float3(atanf(v.x),
            atanf(v.y),
            asinf(v.z));
    }

    /* Computes inverse tangent with 2 args
     */
    inline float3 atan2(const float3& a, const float3& b)
    {
        return float3(atan2f(a.x, b.x),
            atan2f(a.y, b.y),
            atan2f(a.z, b.z));
    }

    /* Computes Euler number raised to the power 'x'
     */
    inline float3 exp(const float3& v)
    {
        return float3(expf(v.x),
            expf(v.y),
            expf(v.z));
    }

    /* Computes 2 raised to the power 'x'
     */
    inline float3 exp2(const float3& v)
    {
        return float3(exp2f(v.x),
            exp2f(v.y),
            exp2f(v.z));
    }

    /* Computes the base Euler number logarithm
     */
    inline float3 log(const float3& v)
    {
        return float3(logf(v.x),
            logf(v.y),
            logf(v.z));
    }

    /* Computes the base 2 logarithm
     */
    inline float3 log2(const float3& v)
    {
        return float3(log2f(v.x),
            log2f(v.y),
            log2f(v.z));
    }

    /* Computes the base 10 logarithm
     */
    inline float3 log10(const float3& v)
    {
        return float3(log10f(v.x),
            log10f(v.y),
            log10f(v.z));
    }

    /* Computes the value of base raised to the power exponent
     */
    inline float3 pow(const float3& a, const float3& b)
    {
        return float3(powf(a.x, b.x),
            powf(a.y, b.y),
            powf(a.z, b.z));
    }

    /* Get the fractal part of floating point
     */
    inline float3 frac(const float3& v)
    {
        return float3(fracf(v.x),
            fracf(v.y),
            fracf(v.z));
    }

    /* Computes the floating-point remainder of the division operation x/y
     */
    inline float3 fmod(const float3& a, const float3& b)
    {
        return float3(fmodf(a.x, b.x),
            fmodf(a.y, b.y),
            fmodf(a.z, b.z));
    }

    /* Computes the smallest integer value not less than 'x'
     */
    inline float3 ceil(const float3& v)
    {
        return float3(ceilf(v.x),
            ceilf(v.y),
            ceilf(v.z));
    }

    /* Computes the largest integer value not greater than 'x'
     */
    inline float3 floor(const float3& v)
    {
        return float3(floorf(v.x),
            floorf(v.y),
            floorf(v.z));
    }

    /* Computes the nearest integer value
     */
    inline float3 round(const float3& v)
    {
        return float3(roundf(v.x),
            roundf(v.y),
            roundf(v.z));
    }

    /* Computes the nearest integer not greater in magnitude than 'x'
     */
    inline float3 trunc(const float3& v)
    {
        return float3(truncf(v.x),
            truncf(v.y),
            truncf(v.z));
    }

    /* Get the smaller value
     */
    inline float3 min(const float3& a, const float3& b)
    {
        return float3(minf(a.x, b.x),
            minf(a.y, b.y),
            minf(a.z, b.z));
    }

    /* Get the larger value
     */
    inline float3 max(const float3& a, const float3& b)
    {
        return float3(maxf(a.x, b.x),
            maxf(a.y, b.y),
            maxf(a.z, b.z));
    }

    /* Clamps the 'x' value to the [min, max].
     */
    inline float3 clamp(const float3& v, const float3& min, const float3& max)
    {
        return float3(clampf(v.x, min.x, max.x),
            clampf(v.y, min.y, max.y),
            clampf(v.z, min.z, max.z));
    }

    /* Clamps the specified value within the range of 0 to 1
     */
    inline float3 saturate(const float3& v)
    {
        return float3(saturatef(v.x),
            saturatef(v.y),
            saturatef(v.z));
    }

    /* Compares two values, returning 0 or 1 based on which value is greater.
     */
    inline float3 step(const float3& a, const float3& b)
    {
        return float3(
            stepf(a.x, b.x),
            stepf(a.y, b.y),
            stepf(a.z, b.z)
        );
    }

    /* Performs a linear interpolation.
     */
    inline float3 lerp(const float3& a, const float3& b, const float3& t)
    {
        return float3(lerpf(a.x, b.x, t.x),
            lerpf(a.y, b.y, t.y),
            lerpf(a.z, b.z, t.z));
    }

    /* Performs a linear interpolation.
     */
    inline float3 lerp(const float3& a, const float3& b, float t)
    {
        return float3(lerpf(a.x, b.x, t),
            lerpf(a.y, b.y, t),
            lerpf(a.z, b.z, t));
    }

    /* Compute a smooth Hermite interpolation
     */
    inline float3 smoothstep(const float3& a, const float3& b, const float3& t)
    {
        return float3(smoothstepf(a.x, b.x, t.x),
            smoothstepf(a.y, b.y, t.y),
            smoothstepf(a.z, b.z, t.z));
    }

    /* Computes square root of 'x'.
     */
    inline float3 sqrt(const float3& v)
    {
        return float3(sqrtf(v.x),
            sqrtf(v.y),
            sqrtf(v.z));
    }

    /* Computes inverse square root of 'x'.
     */
    inline float3 rsqrt(const float3& v)
    {
        return float3(rsqrtf(v.x),
            rsqrtf(v.y),
            rsqrtf(v.z));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline float3 fsqrt(const float3& v)
    {
        return float3(fsqrtf(v.x),
            fsqrtf(v.y),
            fsqrtf(v.z));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline float3 frsqrt(const float3& v)
    {
        return float3(frsqrtf(v.x),
            frsqrtf(v.y),
            frsqrtf(v.z));
    }

    //
    // @region: Graphics functions
    //

    /* Compute cross product of two vectors
     */
    inline float3 cross(const float3& a, const float3& b)
    {
        return float3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    /* Compute dot product of two vectors
     */
    inline float dot(const float3& a, const float3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    /* Compute squared length of vector
     */
    inline float lensqr(const float3& v)
    {
        return dot(v, v);
    }

    /* Compute length of vector
     */
    inline float length(const float3& v)
    {
        return sqrtf(lensqr(v));
    }

    /* Compute distance from 'a' to b
     */
    inline float distance(const float3& a, const float3& b)
    {
        return length(a - b);
    }

    /* Compute squared distance from 'a' to b
     */
    inline float distsqr(const float3& a, const float3& b)
    {
        return lensqr(a - b);
    }

    /* Compute normalized vector
     */
    inline float3 normalize(const float3& v)
    {
        const float lsqr = lensqr(v);
        if (lsqr > 0.0f)
        {
            const float f = rsqrtf(lsqr);
            return float3(v.x * f, v.y * f, v.z * f);
        }
        else
        {
            return v;
        }
    }

    /* Compute reflection vector
     */
    inline float3 reflect(const float3& v, const float3& n)
    {
        return v - 2.0f * dot(v, n) * n;
    }

    /* Compute refraction vector
     */
    inline float3 refract(const float3& v, const float3& n, float eta)
    {
        const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
        return k < 0.0f
            ? float3(0.0f)
            : eta * v - (eta * dot(v, n) + sqrtf(k)) * n;
    }

    /* Compute faceforward vector
     */
    inline float3 faceforward(const float3& n, const float3& i, const float3& nref)
    {
        return dot(i, nref) < 0.0f ? n : -n;
    }

    /* Computes sign of 'x'
 */
    inline float4 sign(const float4& v)
    {
        return float4(signf(v.x),
            signf(v.y),
            signf(v.z),
            signf(v.w));
    }

    /* Computes absolute value
     */
    inline float4 abs(const float4& v)
    {
        return float4(fabsf(v.x),
            fabsf(v.y),
            fabsf(v.z),
            fabsf(v.w));
    }

    /* Computes cosine
     */
    inline float4 cos(const float4& v)
    {
        return float4(cosf(v.x),
            cosf(v.y),
            cosf(v.z),
            cosf(v.w));
    }

    /* Computes sine
     */
    inline float4 sin(const float4& v)
    {
        return float4(sinf(v.x),
            sinf(v.y),
            sinf(v.z),
            sinf(v.w));
    }

    /* Computes tangent
     */
    inline float4 tan(const float4& v)
    {
        return float4(tanf(v.x),
            tanf(v.y),
            tanf(v.z),
            tanf(v.w));
    }

    /* Computes hyperbolic cosine
     */
    inline float4 cosh(const float4& v)
    {
        return float4(coshf(v.x),
            coshf(v.y),
            coshf(v.z),
            coshf(v.w));
    }

    /* Computes hyperbolic sine
     */
    inline float4 sinh(const float4& v)
    {
        return float4(sinhf(v.x),
            sinhf(v.y),
            sinhf(v.z),
            sinhf(v.w));
    }

    /* Computes hyperbolic tangent
     */
    inline float4 tanh(const float4& v)
    {
        return float4(tanhf(v.x),
            tanhf(v.y),
            tanhf(v.z),
            tanhf(v.w));
    }

    /* Computes inverse cosine
     */
    inline float4 acos(const float4& v)
    {
        return float4(acosf(v.x),
            acosf(v.y),
            acosf(v.z),
            acosf(v.w));
    }

    /* Computes inverse sine
     */
    inline float4 asin(const float4& v)
    {
        return float4(asinf(v.x),
            asinf(v.y),
            asinf(v.z),
            asinf(v.w));
    }

    /* Computes inverse tangent
     */
    inline float4 atan(const float4& v)
    {
        return float4(atanf(v.x),
            atanf(v.y),
            atanf(v.z),
            atanf(v.w));
    }

    /* Computes inverse tangent with 2 args
     */
    inline float4 atan2(const float4& a, const float4& b)
    {
        return float4(atan2f(a.x, b.x), atan2f(a.y, b.y), atan2f(a.z, b.z), atan2f(a.w, b.w));
    }

    /* Computes Euler number raised to the power 'x'
     */
    inline float4 exp(const float4& v)
    {
        return float4(expf(v.x), expf(v.y), expf(v.z), expf(v.w));
    }

    /* Computes 2 raised to the power 'x'
     */
    inline float4 exp2(const float4& v)
    {
        return float4(exp2f(v.x), exp2f(v.y), exp2f(v.z), exp2f(v.w));
    }

    /* Computes the base Euler number logarithm
     */
    inline float4 log(const float4& v)
    {
        return float4(logf(v.x), logf(v.y), logf(v.z), logf(v.w));
    }

    /* Computes the base 2 logarithm
     */
    inline float4 log2(const float4& v)
    {
        return float4(log2f(v.x), log2f(v.y), log2f(v.z), log2f(v.w));
    }

    /* Computes the base 10 logarithm
     */
    inline float4 log10(const float4& v)
    {
        return float4(log10f(v.x), log10f(v.y), log10f(v.z), log10f(v.w));
    }

    /* Computes the value of base raised to the power exponent
     */
    inline float4 pow(const float4& a, const float4& b)
    {
        return float4(
            powf(a.x, b.x),
            powf(a.y, b.y),
            powf(a.z, b.z),
            powf(a.w, b.w));
    }

    /* Get the fractal part of floating point
     */
    inline float4 frac(const float4& v)
    {
        return float4(
            fracf(v.x),
            fracf(v.y),
            fracf(v.z),
            fracf(v.w));
    }

    /* Computes the floating-point remainder of the division operation x/y
     */
    inline float4 fmod(const float4& a, const float4& b)
    {
        return float4(
            fmodf(a.x, b.x),
            fmodf(a.y, b.y),
            fmodf(a.z, b.z),
            fmodf(a.w, b.w));
    }

    /* Computes the smallest integer value not less than 'x'
     */
    inline float4 ceil(const float4& v)
    {
        return float4(
            ceilf(v.x),
            ceilf(v.y),
            ceilf(v.z),
            ceilf(v.w));
    }

    /* Computes the largest integer value not greater than 'x'
     */
    inline float4 floor(const float4& v)
    {
        return float4(
            floorf(v.x),
            floorf(v.y),
            floorf(v.z),
            floorf(v.w));
    }

    /* Computes the nearest integer value
     */
    inline float4 round(const float4& v)
    {
        return float4(
            roundf(v.x),
            roundf(v.y),
            roundf(v.z),
            roundf(v.w));
    }

    /* Computes the nearest integer not greater in magnitude than 'x'
     */
    inline float4 trunc(const float4& v)
    {
        return float4(
            truncf(v.x),
            truncf(v.y),
            truncf(v.z),
            truncf(v.w));
    }

    /* Get the smaller value
     */
    inline float4 min(const float4& a, const float4& b)
    {
        return float4(
            minf(a.x, b.x),
            minf(a.y, b.y),
            minf(a.z, b.z),
            minf(a.w, b.w));
    }

    /* Get the larger value
     */
    inline float4 max(const float4& a, const float4& b)
    {
        return float4(
            maxf(a.x, b.x),
            maxf(a.y, b.y),
            maxf(a.z, b.z),
            maxf(a.w, b.w));
    }

    /* Clamps the 'x' value to the [min, max].
     */
    inline float4 clamp(const float4& v, const float4& min, const float4& max)
    {
        return float4(
            clampf(v.x, min.x, max.x),
            clampf(v.y, min.y, max.y),
            clampf(v.z, min.z, max.z),
            clampf(v.w, min.w, max.w));
    }

    /* Clamps the specified value within the range of 0 to 1
     */
    inline float4 saturate(const float4& v)
    {
        return float4(
            saturatef(v.x),
            saturatef(v.y),
            saturatef(v.z),
            saturatef(v.w));
    }

    /* Compares two values, returning 0 or 1 based on which value is greater.
     */
    inline float4 step(const float4& a, const float4& b)
    {
        return float4(
            stepf(a.x, b.x),
            stepf(a.y, b.y),
            stepf(a.z, b.z),
            stepf(a.w, b.w));
    }

    /* Performs a linear interpolation.
     */
    inline float4 lerp(const float4& a, const float4& b, const float4& t)
    {
        return float4(
            lerpf(a.x, b.x, t.x),
            lerpf(a.y, b.y, t.y),
            lerpf(a.z, b.z, t.z),
            lerpf(a.w, b.w, t.w));
    }

    /* Performs a linear interpolation.
     */
    inline float4 lerp(const float4& a, const float4& b, float t)
    {
        return float4(
            lerpf(a.x, b.x, t),
            lerpf(a.y, b.y, t),
            lerpf(a.z, b.z, t),
            lerpf(a.w, b.w, t));
    }

    /* Compute a smooth Hermite interpolation
     */
    inline float4 smoothstep(const float4& a, const float4& b, const float4& t)
    {
        return float4(
            smoothstepf(a.x, b.x, t.x),
            smoothstepf(a.y, b.y, t.y),
            smoothstepf(a.z, b.z, t.z),
            smoothstepf(a.w, b.w, t.w));
    }

    /* Computes square root of 'x'.
     */
    inline float4 sqrt(const float4& v)
    {
        return float4(sqrtf(v.x), sqrtf(v.y), sqrtf(v.z), sqrtf(v.w));
    }

    /* Computes inverse square root of 'x'.
     */
    inline float4 rsqrt(const float4& v)
    {
        return float4(rsqrtf(v.x), rsqrtf(v.y), rsqrtf(v.z), rsqrtf(v.w));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline float4 fsqrt(const float4& v)
    {
        return float4(fsqrtf(v.x), fsqrtf(v.y), fsqrtf(v.z), fsqrtf(v.w));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline float4 frsqrt(const float4& v)
    {
        return float4(frsqrtf(v.x), frsqrtf(v.y), frsqrtf(v.z), frsqrtf(v.w));
    }

    //
    // @region: Graphics functions
    //

    /* Compute dot product of two vectors
     */
    inline float dot(const float4& a, const float4& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    /* Compute squared length of vector
     */
    inline float lensqr(const float4& v)
    {
        return dot(v, v);
    }

    /* Compute length of vector
     */
    inline float length(const float4& v)
    {
        return sqrtf(lensqr(v));
    }

    /* Compute distance from 'a' to b
     */
    inline float distance(const float4& a, const float4& b)
    {
        return length(a - b);
    }

    /* Compute squared distance from 'a' to b
     */
    inline float distsqr(const float4& a, const float4& b)
    {
        return lensqr(a - b);
    }

    /* Compute normalized vector
     */
    inline float4 normalize(const float4& v)
    {
        const float lsqr = lensqr(v);
        if (lsqr > 0.0f)
        {
            const float f = rsqrtf(lsqr);
            return float4(v.x * f, v.y * f, v.z * f, v.w * f);
        }
        else
        {
            return v;
        }
    }

    /* Compute reflection vector
     */
    inline float4 reflect(const float4& v, const float4& n)
    {
        return v - 2.0f * dot(v, n) * n;
    }

    /* Compute refraction vector
     */
    inline float4 refract(const float4& v, const float4& n, float eta)
    {
        const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
        return k < 0.0f
            ? float4(0.0f)
            : eta * v - (eta * dot(v, n) + sqrtf(k)) * n;
    }

    /* Compute faceforward vector
     */
    inline float4 faceforward(const float4& n, const float4& i, const float4& nref)
    {
        return dot(i, nref) < 0.0f ? n : -n;
    }

    //
    // @region: Quaternion
    //

    /* Quaternion multiplication
     */
    inline quat mul(const quat& a, const quat& b)
    {
        const float3 a3 = float3(a.x, a.y, a.z);
        const float3 b3 = float3(b.x, b.y, b.z);

        float3 v = a3 * b.w + b3 * a.w + cross(a3, b3);
        float  w = a.w * b.w - dot(a3, b3);
        return quat(v.x, v.y, v.z, w);
    }

    inline quat inverse(const quat& q)
    {
        return quat(q.x, q.y, q.z, -q.w);
    }

    inline quat conj(const quat& q)
    {
        return quat(-q.x, -q.y, -q.z, q.w);
    }

    //
    // @region: Quaternion
    //

    inline float4 mul(const float4x4& a, const float4& b)
    {
        const float4 c0 = float4(a[0][0], a[1][0], a[2][0], a[3][0]);
        const float4 c1 = float4(a[0][1], a[1][1], a[2][1], a[3][1]);
        const float4 c2 = float4(a[0][2], a[1][2], a[2][2], a[3][2]);
        const float4 c3 = float4(a[0][3], a[1][3], a[2][3], a[3][3]);

        return float4(
            dot(c0, b),
            dot(c1, b),
            dot(c2, b),
            dot(c3, b)
        );
    }

    inline float4 mul(const float4& a, const float4x4& b)
    {
        return float4(
            dot(a, b[0]),
            dot(a, b[1]),
            dot(a, b[2]),
            dot(a, b[3])
        );
    }

    inline float3 mul(const float4x4& a, const float3& b)
    {
        const float4 b0 = float4(b.x, b.y, b.z, 1.0f);
        const float4 b1 = mul(a, b0);

        const float iw = 1.0f / b1.w;
        return float3(b1.x * iw, b1.y * iw, b1.z * iw);
    }

    inline float3 mul(const float3& a, const float4x4& b)
    {
        const float4 a0 = float4(a.x, a.y, a.z, 1.0f);
        const float4 a1 = mul(a0, b);

        const float iw = 1.0f / a1.w;
        return float3(a1.x * iw, a1.y * iw, a1.z * iw);
    }

    inline float4x4 mul(const float4x4& a, const float4x4& b)
    {
        float4x4 result;
        result[0] = mul(a, b[0]);
        result[1] = mul(a, b[1]);
        result[2] = mul(a, b[2]);
        result[3] = mul(a, b[3]);
        return result;
    }

    namespace Math
    {
        inline float4 ToAxisAngle(const quat& q)
        {
            float4 c = float4(q);
            if (c.w != 0.0f)
            {
                c = normalize(c);
            }

            float3 axis;
            const float den = sqrtf(1.0f - c.w * c.w);
            if (den > 0.0001f)
            {
                axis = float3(c.x, c.y, c.z) / den;
            }
            else
            {
                axis = float3(1, 0, 0);
            }

            float angle = 2.0f * cosf(c.w);
            return float4(axis, angle);
        }

        inline float4x4 Transform(const float2& position, float rotation, const float2& scale)
        {
            return mul(mul(Math::Translation(position), Math::RotationZ(rotation)), Math::Scalation(scale));
        }

        inline float4x4 Transform(const float3& position, const quat& rotation, const float3& scale)
        {
            return mul(mul(Math::Translation(position), Math::Rotation(rotation)), Math::Scalation(scale));
        }

        inline float4x4 Ortho(float l, float r, float b, float t, float n, float f)
        {
            const float x = 1.0f / (r - l);
            const float y = 1.0f / (t - b);
            const float z = 1.0f / (f - n);

            float4x4 result;
            result[0] = float4(2.0f * x, 0, 0, 0);
            result[1] = float4(0, 2.0f * y, 0, 0);
            result[2] = float4(0, 0, 2.0f * z, 0);
            result[3] = float4(-x * (l + r), -y * (b + t), -z * (n + f), 1.0f);
            return result;
        }

        inline float4x4 Frustum(float l, float r, float b, float t, float n, float f)
        {
            const float x = 1.0f / (r - l);
            const float y = 1.0f / (t - b);
            const float z = 1.0f / (f - n);

            float4x4 result;
            result[0] = float4(2.0f * x, 0, 0, 0);
            result[1] = float4(0, 2.0f * y, 0, 0);
            result[2] = float4(x * (l + r), y * (b + t), z * (n + f), 1.0f);
            result[3] = float4(0, 0, 2.0f * z, 0);
            return result;
        }

        inline float4x4 Perspective(float fov, float aspect, float znear, float zfar)
        {
            const float a = 1.0f / tanf(fov * 0.5f);
            const float b = zfar / (znear - zfar);

            float4x4 result;
            result[0] = float4(a / aspect, 0, 0, 0);
            result[1] = float4(0, a, 0, 0);
            result[2] = float4(0, 0, b, -1);
            result[3] = float4(0, 0, znear * b, 0);
            return result;
        }

        inline float4x4 Lookat(const float3& eye, const float3& target, const float3& up)
        {
            const float3 z = normalize(eye - target);
            const float3 x = normalize(cross(up, z));
            const float3 y = normalize(cross(z, x));

            float4x4 result;
            result[0] = float4(x.x, y.x, z.x, 0);
            result[1] = float4(x.y, y.y, z.y, 0);
            result[2] = float4(x.z, y.z, z.z, 0);
            result[3] = float4(-dot(x, eye), -dot(y, eye), -dot(z, eye), 1.0f);
            return result;
        }

        inline float4x4 Scalation(float s)
        {
            return Scalation(s, s, s);
        }

        inline float4x4 Scalation(const float2& v)
        {
            return Scalation(v.x, v.y);
        }

        inline float4x4 Scalation(const float3& v)
        {
            return Scalation(v.x, v.y, v.z);
        }

        inline float4x4 Scalation(float x, float y, float z)
        {
            return float4x4(
                x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0,
                0, 0, 0, 1
            );
        }

        inline float4x4 Translation(const float2& v)
        {
            return Translation(v.x, v.y);
        }

        inline float4x4 Translation(const float3& v)
        {
            return Translation(v.x, v.y, v.z);
        }

        inline float4x4 Translation(float x, float y, float z)
        {
            return float4x4(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                x, y, z, 1
            );
        }

        inline float4x4 Rotation(const float3& axis, float angle)
        {
            return Rotation(axis.x, axis.y, axis.z, angle);
        }

        inline float4x4 Rotation(float x, float y, float z, float angle)
        {
            const float c = cosf(-angle);
            const float s = sinf(-angle);
            const float t = 1.0f - c;

            float4x4 result;
            /* Row 1 */
            result[0] = float4(t * x * x + c,
                t * x * y - s * z,
                t * x * z + s * y,
                0.0f);

            /* Row 2 */
            result[1] = float4(t * x * y + s * z,
                t * y * y + c,
                t * y * z - s * x,
                0.0f);

            /* Row 3 */
            result[2] = float4(t * x * z - s * y,
                t * y * z + s * x,
                t * z * z + c,
                0.0f);

            /* Row 4 */
            result[3] = float4(0, 0, 0, 1.0f);
            return result;
        }

        inline float4x4 RotationX(float angle)
        {
            const float s = sinf(angle);
            const float c = cosf(angle);

            return float4x4(
                1, 0, 0, 0,
                0, c, s, 0,
                0, -s, c, 0,
                0, 0, 0, 1
            );
        }

        inline float4x4 RotationY(float angle)
        {
            const float s = sinf(angle);
            const float c = cosf(angle);

            return float4x4(
                c, 0, s, 0,
                0, 1, 0, 0,
                -s, 0, c, 0,
                0, 0, 0, 1
            );
        }

        inline float4x4 RotationZ(float angle)
        {
            const float s = sinf(angle);
            const float c = cosf(angle);

            return float4x4(
                c, s, 0, 0,
                -s, c, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            );
        }

        inline float4x4 Rotation(const quat& quaternion)
        {
            float4 axisangle = ToAxisAngle(quaternion);
            return Rotation(axisangle.x, axisangle.y, axisangle.z, axisangle.w);
        }

        inline void Decompose(const float4x4& m, float3* scalation, quat* quaternion, float3* translation)
        {
            if (translation)
            {
                *translation = float3(m[3][0], m[3][1], m[3][2]);
            }

            if (!scalation && !quaternion)
            {
                return;
            }

            float3 xaxis(m[0][0], m[0][1], m[0][2]);
            float3 yaxis(m[1][0], m[1][1], m[1][2]);
            float3 zaxis(m[2][0], m[2][1], m[2][2]);

            float scale_x = length(xaxis);
            float scale_y = length(yaxis);
            float scale_z = length(zaxis);

            const float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
            const float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
            const float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
            const float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

            const float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
            const float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
            const float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
            const float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

            const float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
            if (det < 0) scale_z = -scale_z;

            if (scalation)
            {
                *scalation = float3(scale_x, scale_y, scale_z);
            }

            if (!quaternion)
            {
                return;
            }

            float rn;

            // Factor the scale out of the matrix axes.
            rn = 1.0f / scale_x;
            xaxis.x *= rn;
            xaxis.y *= rn;
            xaxis.z *= rn;

            rn = 1.0f / scale_y;
            yaxis.x *= rn;
            yaxis.y *= rn;
            yaxis.z *= rn;

            rn = 1.0f / scale_z;
            zaxis.x *= rn;
            zaxis.y *= rn;
            zaxis.z *= rn;

            // Now calculate the rotation from the resulting matrix (axes).
            float trace = xaxis.x + yaxis.y + zaxis.z + 1.0f;

            if (trace > 0.0001f)
            {
                float s = 0.5f / sqrtf(trace);
                quaternion->w = 0.25f / s;
                quaternion->x = (yaxis.z - zaxis.y) * s;
                quaternion->y = (zaxis.x - xaxis.z) * s;
                quaternion->z = (xaxis.y - yaxis.x) * s;
            }
            else
            {
                // Note: since xaxis, yaxis, and zaxis are normalized, 
                // we will never divide by zero in the code below.
                if (xaxis.x > yaxis.y && xaxis.x > zaxis.z)
                {
                    float s = 0.5f / sqrtf(1.0f + xaxis.x - yaxis.y - zaxis.z);
                    quaternion->w = (yaxis.z - zaxis.y) * s;
                    quaternion->x = 0.25f / s;
                    quaternion->y = (yaxis.x + xaxis.y) * s;
                    quaternion->z = (zaxis.x + xaxis.z) * s;
                }
                else if (yaxis.y > zaxis.z)
                {
                    float s = 0.5f / sqrtf(1.0f + yaxis.y - xaxis.x - zaxis.z);
                    quaternion->w = (zaxis.x - xaxis.z) * s;
                    quaternion->x = (yaxis.x + xaxis.y) * s;
                    quaternion->y = 0.25f / s;
                    quaternion->z = (zaxis.y + yaxis.z) * s;
                }
                else
                {
                    float s = 0.5f / sqrtf(1.0f + zaxis.z - xaxis.x - yaxis.y);
                    quaternion->w = (xaxis.y - yaxis.x) * s;
                    quaternion->x = (zaxis.x + xaxis.z) * s;
                    quaternion->y = (zaxis.y + yaxis.z) * s;
                    quaternion->z = 0.25f / s;
                }
            }
        }

        inline void Decompose(const float4x4& m, float3* scalation, float3* axis, float* angle, float3* translation)
        {
            if (axis || angle)
            {
                quat quat;
                Decompose(m, scalation, &quat, translation);

                float4 axisangle = ToAxisAngle(quat);
                *axis = float3(axisangle);
                *angle = axisangle.w;
            }
            else
            {
                Decompose(m, scalation, (quat*)0, translation);
            }
        }
    }
}