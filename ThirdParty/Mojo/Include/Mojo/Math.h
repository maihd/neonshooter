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
    struct Quaterion;
    struct Vector2;
    struct Vector3;
    struct Vector4;
    struct Matrix4;

    namespace Math
    {
        Matrix4 Transform(const Vector2& position, float rotation, const Vector2& scale);
        Matrix4 Transform(const Vector3& position, const Quaterion& rotation, const Vector3& scale);

        Matrix4 Scalation(float s);
        Matrix4 Scalation(const Vector2& v);
        Matrix4 Scalation(const Vector3& v);
        Matrix4 Scalation(float x, float y, float z = 1.0f);

        Matrix4 Translation(const Vector2& v);
        Matrix4 Translation(const Vector3& v);
        Matrix4 Translation(float x, float y, float z = 0.0f);

        Matrix4 Rotation(const Quaterion& quaternion);
        Matrix4 Rotation(const Vector3& axis, float angle);
        Matrix4 Rotation(float x, float y, float z, float angle);

        Matrix4 RotationX(float angle);
        Matrix4 RotationY(float angle);
        Matrix4 RotationZ(float angle);

        Matrix4 Lookat(const Vector3& eye, const Vector3& target, const Vector3& up);

        Matrix4 Ortho(float l, float r, float b, float t, float n, float f);
        Matrix4 Frustum(float l, float r, float b, float t, float n, float f);
        Matrix4 Perspective(float fov, float aspect, float znear, float zfar);

        void Decompose(const Matrix4& m, Vector3* scalation, Quaterion* quaternion, Vector3* translation);
        void Decompose(const Matrix4& m, Vector3* scalation, Vector3* axis, float* angle, Vector3* translation);
    }

    //
    // @region: Constructors
    //

    inline Vector2::Vector2(float s)
        : x(s)
        , y(s)
    {
    }

    inline Vector2::Vector2(const Vector3& v)
        : x(v.x)
        , y(v.y)
    {
    }

    inline Vector2::Vector2(const Vector4& v)
        : x(v.x)
        , y(v.y)
    {
    }

    inline Vector2::Vector2(const Quaterion& q)
        : x(q.x)
        , y(q.y)
    {
    }

    inline Vector3::Vector3(float s)
        : x(s)
        , y(s)
        , z(s)
    {
    }

    inline Vector3::Vector3(const Vector2& v, float z)
        : x(v.x)
        , y(v.y)
        , z(z)
    {
    }

    inline Vector3::Vector3(const Vector4& v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
    {
    }

    inline Vector3::Vector3(const Quaterion& q)
        : x(q.x)
        , y(q.y)
        , z(q.z)
    {
    }

    inline Vector4::Vector4(float s)
        : x(s)
        , y(s)
        , z(s)
        , w(s)
    {
    }

    inline Vector4::Vector4(const Quaterion& q)
        : x(q.x)
        , y(q.y)
        , z(q.z)
        , w(q.w)
    {
    }

    inline Vector4::Vector4(const Vector3& v, float w)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(w)
    {
    }

    inline Vector4::Vector4(const Vector2& v, float z, float w)
        : x(v.x)
        , y(v.y)
        , z(z)
        , w(w)
    {
    }

    inline Quaterion::Quaterion(float s)
        : x(s)
        , y(s)
        , z(s)
        , w(s)
    {
    }

    inline Quaterion::Quaterion(const Vector4& v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(v.w)
    {
    }

    inline Quaterion::Quaterion(float x, float y, float z)
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

    inline Quaterion::Quaterion(const Vector3& axis, float angle)
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

    inline Vector2 operator-(const Vector2& v)
    {
        return Vector2(-v.x, -v.y);
    }

    inline const Vector2& operator+(const Vector2& v)
    {
        return v;
    }

    inline Vector2& operator--(Vector2& v)
    {
        --v.x;
        --v.y;
        return v;
    }

    inline Vector2& operator++(Vector2& v)
    {
        ++v.x;
        ++v.y;
        return v;
    }

    inline Vector2 operator--(Vector2& v, int)
    {
        const Vector2 result = v;

        v.x--;
        v.y--;

        return result;
    }

    inline Vector2 operator++(Vector2& v, int)
    {
        const Vector2 result = v;

        v.x++;
        v.y++;

        return result;
    }

    inline Vector2 operator+(const Vector2& a, const Vector2& b)
    {
#if MATH_ENABLE_NEON   
        return Vector2(vadd_f32(a, b));
#else
        return Vector2(a.x + b.x, a.y + b.y);
#endif
    }

    inline Vector2 operator-(const Vector2& a, const Vector2& b)
    {
#if MATH_ENABLE_NEON   
        return Vector2(vsub_f32(a, b));
#else
        return Vector2(a.x - b.x, a.y - b.y);
#endif
    }

    inline Vector2 operator*(const Vector2& a, const Vector2& b)
    {
#if MATH_ENABLE_NEON   
        return Vector2(vmul_f32(a, b));
#else
        return Vector2(a.x * b.x, a.y * b.y);
#endif
    }

    inline Vector2 operator/(const Vector2& a, const Vector2& b)
    {
#if MATH_ENABLE_NEON && 0 // experimental
        Vector2 res;
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
        return Vector2(a.x / b.x, a.y / b.y);
#endif
    }

    inline Vector2 operator+(const Vector2& a, float b)
    {
        return a + Vector2(b);
    }

    inline Vector2 operator-(const Vector2& a, float b)
    {
        return a - Vector2(b);
    }

    inline Vector2 operator*(const Vector2& a, float b)
    {
        return a * Vector2(b);
    }

    inline Vector2 operator/(const Vector2& a, float b)
    {
        return a / Vector2(b);
    }

    inline Vector2 operator+(float a, const Vector2& b)
    {
        return Vector2(a) + b;
    }

    inline Vector2 operator-(float a, const Vector2& b)
    {
        return Vector2(a) - b;
    }

    inline Vector2 operator*(float a, const Vector2& b)
    {
        return Vector2(a) * b;
    }

    inline Vector2 operator/(float a, const Vector2& b)
    {
        return Vector2(a) / b;
    }

    inline Vector2& operator+=(Vector2& a, const Vector2& b)
    {
        return (a = a + b);
    }

    inline Vector2& operator+=(Vector2& a, float b)
    {
        return (a = a + b);
    }

    inline Vector2& operator-=(Vector2& a, const Vector2& b)
    {
        return (a = a - b);
    }

    inline Vector2& operator-=(Vector2& a, float b)
    {
        return (a = a - b);
    }

    inline Vector2& operator*=(Vector2& a, const Vector2& b)
    {
        return (a = a * b);
    }

    inline Vector2& operator*=(Vector2& a, float b)
    {
        return (a = a * b);
    }

    inline Vector2& operator/=(Vector2& a, const Vector2& b)
    {
        return (a = a / b);
    }

    inline Vector2& operator/=(Vector2& a, float b)
    {
        return (a = a + b);
    }

    inline bool operator==(const Vector2& a, const Vector2& b)
    {
        return a.x == b.x && a.y == b.y;
    }

    inline bool operator!=(const Vector2& a, const Vector2& b)
    {
        return a.x != b.x || a.y != b.y;
    }

    inline Vector3 operator-(const Vector3& v)
    {
        return Vector3(-v.x, -v.y, -v.z);
    }

    inline const Vector3& operator+(const Vector3& v)
    {
        return v;
    }

    inline Vector3& operator--(Vector3& v)
    {
        --v.x;
        --v.y;
        --v.z;
        return v;
    }

    inline Vector3& operator++(Vector3& v)
    {
        ++v.x;
        ++v.y;
        ++v.z;
        return v;
    }

    inline Vector3 operator--(Vector3& v, int)
    {
        const Vector3 result = v;

        v.x--;
        v.y--;
        v.z--;

        return result;
    }

    inline Vector3 operator++(Vector3& v, int)
    {
        const Vector3 result = v;

        v.x++;
        v.y++;
        v.z++;

        return result;
    }

    inline Vector3 operator+(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    inline Vector3 operator-(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    inline Vector3 operator*(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
    }

    inline Vector3 operator/(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);
    }

    inline Vector3 operator+(const Vector3& a, float b)
    {
        return Vector3(a.x + b, a.y + b, a.z + b);
    }

    inline Vector3 operator-(const Vector3& a, float b)
    {
        return Vector3(a.x - b, a.y - b, a.z - b);
    }

    inline Vector3 operator*(const Vector3& a, float b)
    {
        return Vector3(a.x * b, a.y * b, a.z * b);
    }

    inline Vector3 operator/(const Vector3& a, float b)
    {
        return Vector3(a.x / b, a.y / b, a.z / b);
    }

    inline Vector3 operator+(float a, const Vector3& b)
    {
        return Vector3(a + b.x, a + b.y, a + b.z);
    }

    inline Vector3 operator-(float a, const Vector3& b)
    {
        return Vector3(a - b.x, a - b.y, a - b.z);
    }

    inline Vector3 operator*(float a, const Vector3& b)
    {
        return Vector3(a * b.x, a * b.y, a * b.z);
    }

    inline Vector3 operator/(float a, const Vector3& b)
    {
        return Vector3(a / b.x, a / b.y, a / b.z);
    }

    inline Vector3& operator+=(Vector3& a, const Vector3& b)
    {
        return (a = a + b);
    }

    inline Vector3& operator+=(Vector3& a, float b)
    {
        return (a = a + b);
    }

    inline Vector3& operator-=(Vector3& a, const Vector3& b)
    {
        return (a = a - b);
    }

    inline Vector3& operator-=(Vector3& a, float b)
    {
        return (a = a - b);
    }

    inline Vector3& operator*=(Vector3& a, const Vector3& b)
    {
        return (a = a * b);
    }

    inline Vector3& operator*=(Vector3& a, float b)
    {
        return (a = a * b);
    }

    inline Vector3& operator/=(Vector3& a, const Vector3& b)
    {
        return (a = a / b);
    }

    inline Vector3& operator/=(Vector3& a, float b)
    {
        return (a = a + b);
    }

    inline bool operator==(const Vector3& a, const Vector3& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }

    inline bool operator!=(const Vector3& a, const Vector3& b)
    {
        return a.x != b.x || a.y != b.y || a.z != b.z;
    }

    inline Vector4 operator-(const Vector4& v)
    {
        return Vector4(-v.x, -v.y, -v.z, -v.w);
    }

    inline const Vector4& operator+(const Vector4& v)
    {
        return v;
    }

    inline Vector4& operator--(Vector4& v)
    {
        --v.x;
        --v.y;
        --v.z;
        --v.w;
        return v;
    }

    inline Vector4& operator++(Vector4& v)
    {
        ++v.x;
        ++v.y;
        ++v.z;
        ++v.w;
        return v;
    }

    inline Vector4 operator--(Vector4& v, int)
    {
        const Vector4 result = v;

        v.x--;
        v.y--;
        v.z--;
        v.w--;

        return result;
    }

    inline Vector4 operator++(Vector4& v, int)
    {
        const Vector4 result = v;

        v.x++;
        v.y++;
        v.z++;
        v.w++;

        return result;
    }

    inline Vector4 operator+(const Vector4& a, const Vector4& b)
    {
        return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    inline Vector4 operator-(const Vector4& a, const Vector4& b)
    {
        return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }

    inline Vector4 operator*(const Vector4& a, const Vector4& b)
    {
        return Vector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
    }

    inline Vector4 operator/(const Vector4& a, const Vector4& b)
    {
        return Vector4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
    }

    inline Vector4 operator+(const Vector4& a, float b)
    {
        return Vector4(a.x + b, a.y + b, a.z + b, a.w + b);
    }

    inline Vector4 operator-(const Vector4& a, float b)
    {
        return Vector4(a.x - b, a.y - b, a.z - b, a.w - b);
    }

    inline Vector4 operator*(const Vector4& a, float b)
    {
        return Vector4(a.x * b, a.y * b, a.z * b, a.w * b);
    }

    inline Vector4 operator/(const Vector4& a, float b)
    {
        return Vector4(a.x / b, a.y / b, a.z / b, a.w / b);
    }

    inline Vector4 operator+(float a, const Vector4& b)
    {
        return Vector4(a + b.x, a + b.y, a + b.z, a + b.w);
    }

    inline Vector4 operator-(float a, const Vector4& b)
    {
        return Vector4(a - b.x, a - b.y, a - b.z, a - b.w);
    }

    inline Vector4 operator*(float a, const Vector4& b)
    {
        return Vector4(a * b.x, a * b.y, a * b.z, a * b.w);
    }

    inline Vector4 operator/(float a, const Vector4& b)
    {
        return Vector4(a / b.x, a / b.y, a / b.z, a / b.w);
    }

    inline Vector4& operator+=(Vector4& a, const Vector4& b)
    {
        return (a = a + b);
    }

    inline Vector4& operator+=(Vector4& a, float b)
    {
        return (a = a + b);
    }

    inline Vector4& operator-=(Vector4& a, const Vector4& b)
    {
        return (a = a - b);
    }

    inline Vector4& operator-=(Vector4& a, float b)
    {
        return (a = a - b);
    }

    inline Vector4& operator*=(Vector4& a, const Vector4& b)
    {
        return (a = a * b);
    }

    inline Vector4& operator*=(Vector4& a, float b)
    {
        return (a = a * b);
    }

    inline Vector4& operator/=(Vector4& a, const Vector4& b)
    {
        return (a = a / b);
    }

    inline Vector4& operator/=(Vector4& a, float b)
    {
        return (a = a + b);
    }

    inline bool operator==(const Vector4& a, const Vector4& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }

    inline bool operator!=(const Vector4& a, const Vector4& b)
    {
        return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
    }
    inline Quaterion operator-(const Quaterion& v)
    {
        return Quaterion(-v.x, -v.y, -v.z, -v.w);
    }

    inline const Quaterion& operator+(const Quaterion& v)
    {
        return v;
    }

    inline Quaterion& operator--(Quaterion& v)
    {
        --v.x;
        --v.y;
        --v.z;
        --v.w;
        return v;
    }

    inline Quaterion& operator++(Quaterion& v)
    {
        ++v.x;
        ++v.y;
        ++v.z;
        ++v.w;
        return v;
    }

    inline Quaterion operator--(Quaterion& v, int)
    {
        const Quaterion result = v;

        v.x--;
        v.y--;
        v.z--;
        v.w--;

        return result;
    }

    inline Quaterion operator++(Quaterion& v, int)
    {
        const Quaterion result = v;

        v.x++;
        v.y++;
        v.z++;
        v.w++;

        return result;
    }

    inline Quaterion operator+(const Quaterion& a, const Quaterion& b)
    {
        return Quaterion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    inline Quaterion operator-(const Quaterion& a, const Quaterion& b)
    {
        return Quaterion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }

    inline Quaterion operator*(const Quaterion& a, const Quaterion& b)
    {
        return Quaterion(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
    }

    inline Quaterion operator/(const Quaterion& a, const Quaterion& b)
    {
        return Quaterion(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
    }

    inline Quaterion operator+(const Quaterion& a, float b)
    {
        return Quaterion(a.x + b, a.y + b, a.z + b, a.w + b);
    }

    inline Quaterion operator-(const Quaterion& a, float b)
    {
        return Quaterion(a.x - b, a.y - b, a.z - b, a.w - b);
    }

    inline Quaterion operator*(const Quaterion& a, float b)
    {
        return Quaterion(a.x * b, a.y * b, a.z * b, a.w * b);
    }

    inline Quaterion operator/(const Quaterion& a, float b)
    {
        return Quaterion(a.x / b, a.y / b, a.z / b, a.w / b);
    }

    inline Quaterion operator+(float a, const Quaterion& b)
    {
        return Quaterion(a + b.x, a + b.y, a + b.z, a + b.w);
    }

    inline Quaterion operator-(float a, const Quaterion& b)
    {
        return Quaterion(a - b.x, a - b.y, a - b.z, a - b.w);
    }

    inline Quaterion operator*(float a, const Quaterion& b)
    {
        return Quaterion(a * b.x, a * b.y, a * b.z, a * b.w);
    }

    inline Quaterion operator/(float a, const Quaterion& b)
    {
        return Quaterion(a / b.x, a / b.y, a / b.z, a / b.w);
    }

    inline Quaterion& operator+=(Quaterion& a, const Quaterion& b)
    {
        return (a = a + b);
    }

    inline Quaterion& operator+=(Quaterion& a, float b)
    {
        return (a = a + b);
    }

    inline Quaterion& operator-=(Quaterion& a, const Quaterion& b)
    {
        return (a = a - b);
    }

    inline Quaterion& operator-=(Quaterion& a, float b)
    {
        return (a = a - b);
    }

    inline Quaterion& operator*=(Quaterion& a, const Quaterion& b)
    {
        return (a = a * b);
    }

    inline Quaterion& operator*=(Quaterion& a, float b)
    {
        return (a = a * b);
    }

    inline Quaterion& operator/=(Quaterion& a, const Quaterion& b)
    {
        return (a = a / b);
    }

    inline Quaterion& operator/=(Quaterion& a, float b)
    {
        return (a = a + b);
    }

    inline bool operator==(const Quaterion& a, const Quaterion& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }

    inline bool operator!=(const Quaterion& a, const Quaterion& b)
    {
        return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
    }

    inline bool operator==(const Matrix4& a, const Matrix4& b)
    {
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
    }

    inline bool operator!=(const Matrix4& a, const Matrix4& b)
    {
        return a[0] == b[0] || a[1] == b[1] || a[2] == b[2] || a[3] == b[3];
    }

    inline Matrix4 operator-(const Matrix4& m)
    {
        Matrix4 result;
        result[0] = -m[0];
        result[1] = -m[1];
        result[2] = -m[2];
        result[3] = -m[3];
        return result;
    }

    inline const Matrix4& operator+(const Matrix4& m)
    {
        return m;
    }

    inline Matrix4& operator--(Matrix4& m)
    {
        --m[0];
        --m[1];
        --m[2];
        --m[3];
        return m;
    }

    inline Matrix4& operator++(Matrix4& m)
    {
        ++m[0];
        ++m[1];
        ++m[2];
        ++m[3];
        return m;
    }

    inline const Matrix4& operator--(Matrix4& m, int)
    {
        m[0]--;
        m[1]--;
        m[2]--;
        m[3]--;
        return m;
    }

    inline const Matrix4& operator++(Matrix4& m, int)
    {
        m[0]++;
        m[1]++;
        m[2]++;
        m[3]++;
        return m;
    }

    inline Matrix4 operator+(const Matrix4& a, const Matrix4& b)
    {
        Matrix4 result;
        result[0] = a[0] + b[0];
        result[1] = a[1] + b[1];
        result[2] = a[2] + b[2];
        result[3] = a[3] + b[3];
        return result;
    }

    inline Matrix4 operator+(const Matrix4& a, float b)
    {
        Matrix4 result;
        result[0] = a[0] + b;
        result[1] = a[1] + b;
        result[2] = a[2] + b;
        result[3] = a[3] + b;
        return result;
    }

    inline Matrix4 operator+(float a, const Matrix4& b)
    {
        Matrix4 result;
        result[0] = a + b[0];
        result[1] = a + b[1];
        result[2] = a + b[2];
        result[3] = a + b[3];
        return result;
    }

    inline Matrix4 operator-(const Matrix4& a, const Matrix4& b)
    {
        Matrix4 result;
        result[0] = a[0] - b[0];
        result[1] = a[1] - b[1];
        result[2] = a[2] - b[2];
        result[3] = a[3] - b[3];
        return result;
    }

    inline Matrix4 operator-(const Matrix4& a, float b)
    {
        Matrix4 result;
        result[0] = a[0] - b;
        result[1] = a[1] - b;
        result[2] = a[2] - b;
        result[3] = a[3] - b;
        return result;
    }

    inline Matrix4 operator-(float a, const Matrix4& b)
    {
        Matrix4 result;
        result[0] = a - b[0];
        result[1] = a - b[1];
        result[2] = a - b[2];
        result[3] = a - b[3];
        return result;
    }

    inline Matrix4 operator*(const Matrix4& a, float b)
    {
        Matrix4 result;
        result[0] = a[0] * b;
        result[1] = a[1] * b;
        result[2] = a[2] * b;
        result[3] = a[3] * b;
        return result;
    }

    inline Matrix4 operator*(float a, const Matrix4& b)
    {
        Matrix4 result;
        result[0] = a * b[0];
        result[1] = a * b[1];
        result[2] = a * b[2];
        result[3] = a * b[3];
        return result;
    }

    inline Matrix4 operator*(const Matrix4& a, const Matrix4& b)
    {
        Matrix4 result;
        result[0] = a[0] * b[0];
        result[1] = a[1] * b[1];
        result[2] = a[2] * b[2];
        result[3] = a[3] * b[3];
        return result;
    }

    inline Matrix4 operator/(const Matrix4& a, const Matrix4& b)
    {
        Matrix4 result;
        result[0] = a[0] / b[0];
        result[1] = a[1] / b[1];
        result[2] = a[2] / b[2];
        result[3] = a[3] / b[3];
        return result;
    }

    inline Matrix4 operator/(const Matrix4& a, float b)
    {
        Matrix4 result;
        result[0] = a[0] / b;
        result[1] = a[1] / b;
        result[2] = a[2] / b;
        result[3] = a[3] / b;
        return result;
    }

    inline Matrix4 operator/(float a, const Matrix4& b)
    {
        Matrix4 result;
        result[0] = a / b[0];
        result[1] = a / b[1];
        result[2] = a / b[2];
        result[3] = a / b[3];
        return result;
    }

    inline Matrix4& operator+=(Matrix4& a, const Matrix4& b)
    {
        return (a = a + b);
    }

    inline Matrix4& operator+=(Matrix4& a, float b)
    {
        return (a = a + b);
    }

    inline Matrix4& operator-=(Matrix4& a, const Matrix4& b)
    {
        return (a = a - b);
    }

    inline Matrix4& operator-=(Matrix4& a, float b)
    {
        return (a = a - b);
    }

    inline Matrix4& operator*=(Matrix4& a, const Matrix4& b)
    {
        return (a = a * b);
    }

    inline Matrix4& operator*=(Matrix4& a, float b)
    {
        return (a = a * b);
    }

    inline Matrix4& operator/=(Matrix4& a, const Matrix4& b)
    {
        return (a = a / b);
    }

    inline Matrix4& operator/=(Matrix4& a, float b)
    {
        return (a = a + b);
    }

    // Computes sign of 'x'
    inline Vector2 sign(const Vector2& v)
    {
        return Vector2(signf(v.x), signf(v.y));
    }

    /* Computes absolute value
     */
    inline Vector2 abs(const Vector2& v)
    {
        return Vector2(fabsf(v.x), fabsf(v.y));
    }

    /* Computes cosine
     */
    inline Vector2 cos(const Vector2& v)
    {
        return Vector2(cosf(v.x), cosf(v.y));
    }

    /* Computes sine
     */
    inline Vector2 sin(const Vector2& v)
    {
        return Vector2(sinf(v.x), sinf(v.y));
    }

    /* Computes tangent
     */
    inline Vector2 tan(const Vector2& v)
    {
        return Vector2(tanf(v.x), tanf(v.y));
    }

    /* Computes hyperbolic cosine
     */
    inline Vector2 cosh(const Vector2& v)
    {
        return Vector2(coshf(v.x), coshf(v.y));
    }

    /* Computes hyperbolic sine
     */
    inline Vector2 sinh(const Vector2& v)
    {
        return Vector2(sinhf(v.x), sinhf(v.y));
    }

    /* Computes hyperbolic tangent
     */
    inline Vector2 tanh(const Vector2& v)
    {
        return Vector2(tanhf(v.x), tanhf(v.y));
    }

    /* Computes inverse cosine
     */
    inline Vector2 acos(const Vector2& v)
    {
        return Vector2(acosf(v.x), acosf(v.y));
    }

    /* Computes inverse sine
     */
    inline Vector2 asin(const Vector2& v)
    {
        return Vector2(asinf(v.x), asinf(v.y));
    }

    /* Computes inverse tangent
     */
    inline Vector2 atan(const Vector2& v)
    {
        return Vector2(atanf(v.x), atanf(v.y));
    }

    /* Computes inverse tangent with 2 args
     */
    inline Vector2 atan2(const Vector2& a, const Vector2& b)
    {
        return Vector2(atan2f(a.x, b.x), atan2f(a.y, b.y));
    }

    /* Computes Euler number raised to the power 'x'
     */
    inline Vector2 exp(const Vector2& v)
    {
        return Vector2(expf(v.x), expf(v.y));
    }

    /* Computes 2 raised to the power 'x'
     */
    inline Vector2 exp2(const Vector2& v)
    {
        return Vector2(exp2f(v.x), exp2f(v.y));
    }

    /* Computes the base Euler number logarithm
     */
    inline Vector2 log(const Vector2& v)
    {
        return Vector2(logf(v.x), logf(v.y));
    }

    /* Computes the base 2 logarithm
     */
    inline Vector2 log2(const Vector2& v)
    {
        return Vector2(log2f(v.x), log2f(v.y));
    }

    /* Computes the base 10 logarithm
     */
    inline Vector2 log10(const Vector2& v)
    {
        return Vector2(log10f(v.x), log10f(v.y));
    }

    /* Computes the value of base raised to the power exponent
     */
    inline Vector2 pow(const Vector2& a, const Vector2& b)
    {
        return Vector2(powf(a.x, b.x), powf(a.y, b.y));
    }

    /* Get the fractal part of floating point
     */
    inline Vector2 frac(const Vector2& v)
    {
        return Vector2(fracf(v.x), fracf(v.y));
    }

    /* Computes the floating-point remainder of the division operation x/y
     */
    inline Vector2 fmod(const Vector2& a, const Vector2& b)
    {
        return Vector2(fmodf(a.x, b.x), fmodf(a.y, b.y));
    }

    /* Computes the smallest integer value not less than 'x'
     */
    inline Vector2 ceil(const Vector2& v)
    {
        return Vector2(ceilf(v.x), ceilf(v.y));
    }

    /* Computes the largest integer value not greater than 'x'
     */
    inline Vector2 floor(const Vector2& v)
    {
        return Vector2(floorf(v.x), floorf(v.y));
    }

    /* Computes the nearest integer value
     */
    inline Vector2 round(const Vector2& v)
    {
        return Vector2(roundf(v.x), roundf(v.y));
    }

    /* Computes the nearest integer not greater in magnitude than 'x'
     */
    inline Vector2 trunc(const Vector2& v)
    {
        return Vector2(truncf(v.x), truncf(v.y));
    }

    /* Get the smaller value
     */
    inline Vector2 min(const Vector2& a, const Vector2& b)
    {
        return Vector2(minf(a.x, b.x), minf(a.y, b.y));
    }

    /* Get the larger value
     */
    inline Vector2 max(const Vector2& a, const Vector2& b)
    {
        return Vector2(maxf(a.x, b.x), maxf(a.y, b.y));
    }

    /* Clamps the 'x' value to the [min, max].
     */
    inline Vector2 clamp(const Vector2& v, const Vector2& min, const Vector2& max)
    {
        return Vector2(clampf(v.x, min.x, max.x), clampf(v.y, min.y, max.y));
    }

    /* Clamps the specified value within the range of 0 to 1
     */
    inline Vector2 saturate(const Vector2& v)
    {
        return Vector2(saturatef(v.x), saturatef(v.y));
    }

    /* Compares two values, returning 0 or 1 based on which value is greater.
     */
    inline Vector2 step(const Vector2& a, const Vector2& b)
    {
        return Vector2(stepf(a.x, b.x), stepf(a.y, b.y));
    }

    /* Performs a linear interpolation.
     */
    inline Vector2 lerp(const Vector2& a, const Vector2& b, const Vector2& t)
    {
        return Vector2(lerpf(a.x, b.x, t.x), lerpf(a.y, b.y, t.y));
    }

    /* Performs a linear interpolation.
     */
    inline Vector2 lerp(const Vector2& a, const Vector2& b, float t)
    {
        return Vector2(lerpf(a.x, b.x, t), lerpf(a.y, b.y, t));
    }

    /* Compute a smooth Hermite interpolation
     */
    inline Vector2 smoothstep(const Vector2& a, const Vector2& b, const Vector2& t)
    {
        return Vector2(smoothstepf(a.x, b.x, t.x), smoothstepf(a.y, b.y, t.y));
    }

    /* Computes square root of 'x'.
     */
    inline Vector2 sqrt(const Vector2& v)
    {
        return Vector2(sqrtf(v.x), sqrtf(v.y));
    }

    /* Computes inverse square root of 'x'.
     */
    inline Vector2 rsqrt(const Vector2& v)
    {
        return Vector2(rsqrtf(v.x), rsqrtf(v.y));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline Vector2 fsqrt(const Vector2& v)
    {
        return Vector2(fsqrtf(v.x), fsqrtf(v.y));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline Vector2 frsqrt(const Vector2& v)
    {
        return Vector2(frsqrtf(v.x), frsqrtf(v.y));
    }

    //
    // @region: Graphics funtions
    //

    /* Compute dot product of two vectors
     */
    inline float dot(const Vector2& a, const Vector2& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    /* Compute squared length of vector
     */
    inline float lensqr(const Vector2& v)
    {
        return dot(v, v);
    }

    /* Compute length of vector
     */
    inline float length(const Vector2& v)
    {
        return sqrtf(lensqr(v));
    }

    /* Compute distance from 'a' to b
     */
    inline float distance(const Vector2& a, const Vector2& b)
    {
        return length(a - b);
    }

    /* Compute squared distance from 'a' to b
     */
    inline float distsqr(const Vector2& a, const Vector2& b)
    {
        return lensqr(a - b);
    }

    /* Compute normalized vector
     */
    inline Vector2 normalize(const Vector2& v)
    {
        const float lsqr = lensqr(v);
        if (lsqr > 0.0f)
        {
            const float f = rsqrtf(lsqr);
            return Vector2(v.x * f, v.y * f);
        }
        else
        {
            return v;
        }
    }

    /* Compute reflection vector
     */
    inline Vector2 reflect(const Vector2& v, const Vector2& n)
    {
        return v - 2.0f * dot(v, n) * n;
    }

    /* Compute refraction vector
     */
    inline Vector2 refract(const Vector2& v, const Vector2& n, float eta)
    {
        const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
        return k < 0.0f
            ? Vector2(0.0f)
            : eta * v - (eta * dot(v, n) + sqrtf(k)) * v;
    }

    /* Compute faceforward vector
     */
    inline Vector2 faceforward(const Vector2& n, const Vector2& i, const Vector2& nref)
    {
        return dot(i, nref) < 0.0f ? n : -n;
    }
    /* Computes sign of 'x'
 */
    inline Vector3 sign(const Vector3& v)
    {
        return Vector3(signf(v.x), signf(v.y), signf(v.z));
    }

    /* Computes absolute value
     */
    inline Vector3 abs(const Vector3& v)
    {
        return Vector3(fabsf(v.x), fabsf(v.y), fabsf(v.z));
    }

    /* Computes cosine
     */
    inline Vector3 cos(const Vector3& v)
    {
        return Vector3(cosf(v.x), cosf(v.y), cosf(v.z));
    }

    /* Computes sine
     */
    inline Vector3 sin(const Vector3& v)
    {
        return Vector3(sinf(v.x), sinf(v.y), sinf(v.z));
    }

    /* Computes tangent
     */
    inline Vector3 tan(const Vector3& v)
    {
        return Vector3(tanf(v.x), tanf(v.y), tanf(v.z));
    }

    /* Computes hyperbolic cosine
     */
    inline Vector3 cosh(const Vector3& v)
    {
        return Vector3(coshf(v.x), coshf(v.y), coshf(v.z));
    }

    /* Computes hyperbolic sine
     */
    inline Vector3 sinh(const Vector3& v)
    {
        return Vector3(sinhf(v.x), sinhf(v.y), sinhf(v.z));
    }

    /* Computes hyperbolic tangent
     */
    inline Vector3 tanh(const Vector3& v)
    {
        return Vector3(tanhf(v.x), tanhf(v.y), tanhf(v.z));
    }

    /* Computes inverse cosine
     */
    inline Vector3 acos(const Vector3& v)
    {
        return Vector3(acosf(v.x), acosf(v.y), acosf(v.z));
    }

    /* Computes inverse sine
     */
    inline Vector3 asin(const Vector3& v)
    {
        return Vector3(asinf(v.x), asinf(v.y), asinf(v.z));
    }

    /* Computes inverse tangent
     */
    inline Vector3 atan(const Vector3& v)
    {
        return Vector3(atanf(v.x),
            atanf(v.y),
            asinf(v.z));
    }

    /* Computes inverse tangent with 2 args
     */
    inline Vector3 atan2(const Vector3& a, const Vector3& b)
    {
        return Vector3(atan2f(a.x, b.x),
            atan2f(a.y, b.y),
            atan2f(a.z, b.z));
    }

    /* Computes Euler number raised to the power 'x'
     */
    inline Vector3 exp(const Vector3& v)
    {
        return Vector3(expf(v.x),
            expf(v.y),
            expf(v.z));
    }

    /* Computes 2 raised to the power 'x'
     */
    inline Vector3 exp2(const Vector3& v)
    {
        return Vector3(exp2f(v.x),
            exp2f(v.y),
            exp2f(v.z));
    }

    /* Computes the base Euler number logarithm
     */
    inline Vector3 log(const Vector3& v)
    {
        return Vector3(logf(v.x),
            logf(v.y),
            logf(v.z));
    }

    /* Computes the base 2 logarithm
     */
    inline Vector3 log2(const Vector3& v)
    {
        return Vector3(log2f(v.x),
            log2f(v.y),
            log2f(v.z));
    }

    /* Computes the base 10 logarithm
     */
    inline Vector3 log10(const Vector3& v)
    {
        return Vector3(log10f(v.x),
            log10f(v.y),
            log10f(v.z));
    }

    /* Computes the value of base raised to the power exponent
     */
    inline Vector3 pow(const Vector3& a, const Vector3& b)
    {
        return Vector3(powf(a.x, b.x),
            powf(a.y, b.y),
            powf(a.z, b.z));
    }

    /* Get the fractal part of floating point
     */
    inline Vector3 frac(const Vector3& v)
    {
        return Vector3(fracf(v.x),
            fracf(v.y),
            fracf(v.z));
    }

    /* Computes the floating-point remainder of the division operation x/y
     */
    inline Vector3 fmod(const Vector3& a, const Vector3& b)
    {
        return Vector3(fmodf(a.x, b.x),
            fmodf(a.y, b.y),
            fmodf(a.z, b.z));
    }

    /* Computes the smallest integer value not less than 'x'
     */
    inline Vector3 ceil(const Vector3& v)
    {
        return Vector3(ceilf(v.x),
            ceilf(v.y),
            ceilf(v.z));
    }

    /* Computes the largest integer value not greater than 'x'
     */
    inline Vector3 floor(const Vector3& v)
    {
        return Vector3(floorf(v.x),
            floorf(v.y),
            floorf(v.z));
    }

    /* Computes the nearest integer value
     */
    inline Vector3 round(const Vector3& v)
    {
        return Vector3(roundf(v.x),
            roundf(v.y),
            roundf(v.z));
    }

    /* Computes the nearest integer not greater in magnitude than 'x'
     */
    inline Vector3 trunc(const Vector3& v)
    {
        return Vector3(truncf(v.x),
            truncf(v.y),
            truncf(v.z));
    }

    /* Get the smaller value
     */
    inline Vector3 min(const Vector3& a, const Vector3& b)
    {
        return Vector3(minf(a.x, b.x),
            minf(a.y, b.y),
            minf(a.z, b.z));
    }

    /* Get the larger value
     */
    inline Vector3 max(const Vector3& a, const Vector3& b)
    {
        return Vector3(maxf(a.x, b.x),
            maxf(a.y, b.y),
            maxf(a.z, b.z));
    }

    /* Clamps the 'x' value to the [min, max].
     */
    inline Vector3 clamp(const Vector3& v, const Vector3& min, const Vector3& max)
    {
        return Vector3(clampf(v.x, min.x, max.x),
            clampf(v.y, min.y, max.y),
            clampf(v.z, min.z, max.z));
    }

    /* Clamps the specified value within the range of 0 to 1
     */
    inline Vector3 saturate(const Vector3& v)
    {
        return Vector3(saturatef(v.x),
            saturatef(v.y),
            saturatef(v.z));
    }

    /* Compares two values, returning 0 or 1 based on which value is greater.
     */
    inline Vector3 step(const Vector3& a, const Vector3& b)
    {
        return Vector3(
            stepf(a.x, b.x),
            stepf(a.y, b.y),
            stepf(a.z, b.z)
        );
    }

    /* Performs a linear interpolation.
     */
    inline Vector3 lerp(const Vector3& a, const Vector3& b, const Vector3& t)
    {
        return Vector3(lerpf(a.x, b.x, t.x),
            lerpf(a.y, b.y, t.y),
            lerpf(a.z, b.z, t.z));
    }

    /* Performs a linear interpolation.
     */
    inline Vector3 lerp(const Vector3& a, const Vector3& b, float t)
    {
        return Vector3(lerpf(a.x, b.x, t),
            lerpf(a.y, b.y, t),
            lerpf(a.z, b.z, t));
    }

    /* Compute a smooth Hermite interpolation
     */
    inline Vector3 smoothstep(const Vector3& a, const Vector3& b, const Vector3& t)
    {
        return Vector3(smoothstepf(a.x, b.x, t.x),
            smoothstepf(a.y, b.y, t.y),
            smoothstepf(a.z, b.z, t.z));
    }

    /* Computes square root of 'x'.
     */
    inline Vector3 sqrt(const Vector3& v)
    {
        return Vector3(sqrtf(v.x),
            sqrtf(v.y),
            sqrtf(v.z));
    }

    /* Computes inverse square root of 'x'.
     */
    inline Vector3 rsqrt(const Vector3& v)
    {
        return Vector3(rsqrtf(v.x),
            rsqrtf(v.y),
            rsqrtf(v.z));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline Vector3 fsqrt(const Vector3& v)
    {
        return Vector3(fsqrtf(v.x),
            fsqrtf(v.y),
            fsqrtf(v.z));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline Vector3 frsqrt(const Vector3& v)
    {
        return Vector3(frsqrtf(v.x),
            frsqrtf(v.y),
            frsqrtf(v.z));
    }

    //
    // @region: Graphics functions
    //

    /* Compute cross product of two vectors
     */
    inline Vector3 cross(const Vector3& a, const Vector3& b)
    {
        return Vector3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    /* Compute dot product of two vectors
     */
    inline float dot(const Vector3& a, const Vector3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    /* Compute squared length of vector
     */
    inline float lensqr(const Vector3& v)
    {
        return dot(v, v);
    }

    /* Compute length of vector
     */
    inline float length(const Vector3& v)
    {
        return sqrtf(lensqr(v));
    }

    /* Compute distance from 'a' to b
     */
    inline float distance(const Vector3& a, const Vector3& b)
    {
        return length(a - b);
    }

    /* Compute squared distance from 'a' to b
     */
    inline float distsqr(const Vector3& a, const Vector3& b)
    {
        return lensqr(a - b);
    }

    /* Compute normalized vector
     */
    inline Vector3 normalize(const Vector3& v)
    {
        const float lsqr = lensqr(v);
        if (lsqr > 0.0f)
        {
            const float f = rsqrtf(lsqr);
            return Vector3(v.x * f, v.y * f, v.z * f);
        }
        else
        {
            return v;
        }
    }

    /* Compute reflection vector
     */
    inline Vector3 reflect(const Vector3& v, const Vector3& n)
    {
        return v - 2.0f * dot(v, n) * n;
    }

    /* Compute refraction vector
     */
    inline Vector3 refract(const Vector3& v, const Vector3& n, float eta)
    {
        const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
        return k < 0.0f
            ? Vector3(0.0f)
            : eta * v - (eta * dot(v, n) + sqrtf(k)) * n;
    }

    /* Compute faceforward vector
     */
    inline Vector3 faceforward(const Vector3& n, const Vector3& i, const Vector3& nref)
    {
        return dot(i, nref) < 0.0f ? n : -n;
    }

    /* Computes sign of 'x'
 */
    inline Vector4 sign(const Vector4& v)
    {
        return Vector4(signf(v.x),
            signf(v.y),
            signf(v.z),
            signf(v.w));
    }

    /* Computes absolute value
     */
    inline Vector4 abs(const Vector4& v)
    {
        return Vector4(fabsf(v.x),
            fabsf(v.y),
            fabsf(v.z),
            fabsf(v.w));
    }

    /* Computes cosine
     */
    inline Vector4 cos(const Vector4& v)
    {
        return Vector4(cosf(v.x),
            cosf(v.y),
            cosf(v.z),
            cosf(v.w));
    }

    /* Computes sine
     */
    inline Vector4 sin(const Vector4& v)
    {
        return Vector4(sinf(v.x),
            sinf(v.y),
            sinf(v.z),
            sinf(v.w));
    }

    /* Computes tangent
     */
    inline Vector4 tan(const Vector4& v)
    {
        return Vector4(tanf(v.x),
            tanf(v.y),
            tanf(v.z),
            tanf(v.w));
    }

    /* Computes hyperbolic cosine
     */
    inline Vector4 cosh(const Vector4& v)
    {
        return Vector4(coshf(v.x),
            coshf(v.y),
            coshf(v.z),
            coshf(v.w));
    }

    /* Computes hyperbolic sine
     */
    inline Vector4 sinh(const Vector4& v)
    {
        return Vector4(sinhf(v.x),
            sinhf(v.y),
            sinhf(v.z),
            sinhf(v.w));
    }

    /* Computes hyperbolic tangent
     */
    inline Vector4 tanh(const Vector4& v)
    {
        return Vector4(tanhf(v.x),
            tanhf(v.y),
            tanhf(v.z),
            tanhf(v.w));
    }

    /* Computes inverse cosine
     */
    inline Vector4 acos(const Vector4& v)
    {
        return Vector4(acosf(v.x),
            acosf(v.y),
            acosf(v.z),
            acosf(v.w));
    }

    /* Computes inverse sine
     */
    inline Vector4 asin(const Vector4& v)
    {
        return Vector4(asinf(v.x),
            asinf(v.y),
            asinf(v.z),
            asinf(v.w));
    }

    /* Computes inverse tangent
     */
    inline Vector4 atan(const Vector4& v)
    {
        return Vector4(atanf(v.x),
            atanf(v.y),
            atanf(v.z),
            atanf(v.w));
    }

    /* Computes inverse tangent with 2 args
     */
    inline Vector4 atan2(const Vector4& a, const Vector4& b)
    {
        return Vector4(atan2f(a.x, b.x), atan2f(a.y, b.y), atan2f(a.z, b.z), atan2f(a.w, b.w));
    }

    /* Computes Euler number raised to the power 'x'
     */
    inline Vector4 exp(const Vector4& v)
    {
        return Vector4(expf(v.x), expf(v.y), expf(v.z), expf(v.w));
    }

    /* Computes 2 raised to the power 'x'
     */
    inline Vector4 exp2(const Vector4& v)
    {
        return Vector4(exp2f(v.x), exp2f(v.y), exp2f(v.z), exp2f(v.w));
    }

    /* Computes the base Euler number logarithm
     */
    inline Vector4 log(const Vector4& v)
    {
        return Vector4(logf(v.x), logf(v.y), logf(v.z), logf(v.w));
    }

    /* Computes the base 2 logarithm
     */
    inline Vector4 log2(const Vector4& v)
    {
        return Vector4(log2f(v.x), log2f(v.y), log2f(v.z), log2f(v.w));
    }

    /* Computes the base 10 logarithm
     */
    inline Vector4 log10(const Vector4& v)
    {
        return Vector4(log10f(v.x), log10f(v.y), log10f(v.z), log10f(v.w));
    }

    /* Computes the value of base raised to the power exponent
     */
    inline Vector4 pow(const Vector4& a, const Vector4& b)
    {
        return Vector4(
            powf(a.x, b.x),
            powf(a.y, b.y),
            powf(a.z, b.z),
            powf(a.w, b.w));
    }

    /* Get the fractal part of floating point
     */
    inline Vector4 frac(const Vector4& v)
    {
        return Vector4(
            fracf(v.x),
            fracf(v.y),
            fracf(v.z),
            fracf(v.w));
    }

    /* Computes the floating-point remainder of the division operation x/y
     */
    inline Vector4 fmod(const Vector4& a, const Vector4& b)
    {
        return Vector4(
            fmodf(a.x, b.x),
            fmodf(a.y, b.y),
            fmodf(a.z, b.z),
            fmodf(a.w, b.w));
    }

    /* Computes the smallest integer value not less than 'x'
     */
    inline Vector4 ceil(const Vector4& v)
    {
        return Vector4(
            ceilf(v.x),
            ceilf(v.y),
            ceilf(v.z),
            ceilf(v.w));
    }

    /* Computes the largest integer value not greater than 'x'
     */
    inline Vector4 floor(const Vector4& v)
    {
        return Vector4(
            floorf(v.x),
            floorf(v.y),
            floorf(v.z),
            floorf(v.w));
    }

    /* Computes the nearest integer value
     */
    inline Vector4 round(const Vector4& v)
    {
        return Vector4(
            roundf(v.x),
            roundf(v.y),
            roundf(v.z),
            roundf(v.w));
    }

    /* Computes the nearest integer not greater in magnitude than 'x'
     */
    inline Vector4 trunc(const Vector4& v)
    {
        return Vector4(
            truncf(v.x),
            truncf(v.y),
            truncf(v.z),
            truncf(v.w));
    }

    /* Get the smaller value
     */
    inline Vector4 min(const Vector4& a, const Vector4& b)
    {
        return Vector4(
            minf(a.x, b.x),
            minf(a.y, b.y),
            minf(a.z, b.z),
            minf(a.w, b.w));
    }

    /* Get the larger value
     */
    inline Vector4 max(const Vector4& a, const Vector4& b)
    {
        return Vector4(
            maxf(a.x, b.x),
            maxf(a.y, b.y),
            maxf(a.z, b.z),
            maxf(a.w, b.w));
    }

    /* Clamps the 'x' value to the [min, max].
     */
    inline Vector4 clamp(const Vector4& v, const Vector4& min, const Vector4& max)
    {
        return Vector4(
            clampf(v.x, min.x, max.x),
            clampf(v.y, min.y, max.y),
            clampf(v.z, min.z, max.z),
            clampf(v.w, min.w, max.w));
    }

    /* Clamps the specified value within the range of 0 to 1
     */
    inline Vector4 saturate(const Vector4& v)
    {
        return Vector4(
            saturatef(v.x),
            saturatef(v.y),
            saturatef(v.z),
            saturatef(v.w));
    }

    /* Compares two values, returning 0 or 1 based on which value is greater.
     */
    inline Vector4 step(const Vector4& a, const Vector4& b)
    {
        return Vector4(
            stepf(a.x, b.x),
            stepf(a.y, b.y),
            stepf(a.z, b.z),
            stepf(a.w, b.w));
    }

    /* Performs a linear interpolation.
     */
    inline Vector4 lerp(const Vector4& a, const Vector4& b, const Vector4& t)
    {
        return Vector4(
            lerpf(a.x, b.x, t.x),
            lerpf(a.y, b.y, t.y),
            lerpf(a.z, b.z, t.z),
            lerpf(a.w, b.w, t.w));
    }

    /* Performs a linear interpolation.
     */
    inline Vector4 lerp(const Vector4& a, const Vector4& b, float t)
    {
        return Vector4(
            lerpf(a.x, b.x, t),
            lerpf(a.y, b.y, t),
            lerpf(a.z, b.z, t),
            lerpf(a.w, b.w, t));
    }

    /* Compute a smooth Hermite interpolation
     */
    inline Vector4 smoothstep(const Vector4& a, const Vector4& b, const Vector4& t)
    {
        return Vector4(
            smoothstepf(a.x, b.x, t.x),
            smoothstepf(a.y, b.y, t.y),
            smoothstepf(a.z, b.z, t.z),
            smoothstepf(a.w, b.w, t.w));
    }

    /* Computes square root of 'x'.
     */
    inline Vector4 sqrt(const Vector4& v)
    {
        return Vector4(sqrtf(v.x), sqrtf(v.y), sqrtf(v.z), sqrtf(v.w));
    }

    /* Computes inverse square root of 'x'.
     */
    inline Vector4 rsqrt(const Vector4& v)
    {
        return Vector4(rsqrtf(v.x), rsqrtf(v.y), rsqrtf(v.z), rsqrtf(v.w));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline Vector4 fsqrt(const Vector4& v)
    {
        return Vector4(fsqrtf(v.x), fsqrtf(v.y), fsqrtf(v.z), fsqrtf(v.w));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline Vector4 frsqrt(const Vector4& v)
    {
        return Vector4(frsqrtf(v.x), frsqrtf(v.y), frsqrtf(v.z), frsqrtf(v.w));
    }

    //
    // @region: Graphics functions
    //

    /* Compute dot product of two vectors
     */
    inline float dot(const Vector4& a, const Vector4& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    /* Compute squared length of vector
     */
    inline float lensqr(const Vector4& v)
    {
        return dot(v, v);
    }

    /* Compute length of vector
     */
    inline float length(const Vector4& v)
    {
        return sqrtf(lensqr(v));
    }

    /* Compute distance from 'a' to b
     */
    inline float distance(const Vector4& a, const Vector4& b)
    {
        return length(a - b);
    }

    /* Compute squared distance from 'a' to b
     */
    inline float distsqr(const Vector4& a, const Vector4& b)
    {
        return lensqr(a - b);
    }

    /* Compute normalized vector
     */
    inline Vector4 normalize(const Vector4& v)
    {
        const float lsqr = lensqr(v);
        if (lsqr > 0.0f)
        {
            const float f = rsqrtf(lsqr);
            return Vector4(v.x * f, v.y * f, v.z * f, v.w * f);
        }
        else
        {
            return v;
        }
    }

    /* Compute reflection vector
     */
    inline Vector4 reflect(const Vector4& v, const Vector4& n)
    {
        return v - 2.0f * dot(v, n) * n;
    }

    /* Compute refraction vector
     */
    inline Vector4 refract(const Vector4& v, const Vector4& n, float eta)
    {
        const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
        return k < 0.0f
            ? Vector4(0.0f)
            : eta * v - (eta * dot(v, n) + sqrtf(k)) * n;
    }

    /* Compute faceforward vector
     */
    inline Vector4 faceforward(const Vector4& n, const Vector4& i, const Vector4& nref)
    {
        return dot(i, nref) < 0.0f ? n : -n;
    }

    //
    // @region: Quaternion
    //

    /* Quaternion multiplication
     */
    inline Quaterion mul(const Quaterion& a, const Quaterion& b)
    {
        const Vector3 a3 = Vector3(a.x, a.y, a.z);
        const Vector3 b3 = Vector3(b.x, b.y, b.z);

        Vector3 v = a3 * b.w + b3 * a.w + cross(a3, b3);
        float  w = a.w * b.w - dot(a3, b3);
        return Quaterion(v.x, v.y, v.z, w);
    }

    inline Quaterion inverse(const Quaterion& q)
    {
        return Quaterion(q.x, q.y, q.z, -q.w);
    }

    inline Quaterion conj(const Quaterion& q)
    {
        return Quaterion(-q.x, -q.y, -q.z, q.w);
    }

    //
    // @region: Quaternion
    //

    inline Vector4 mul(const Matrix4& a, const Vector4& b)
    {
        const Vector4 c0 = Vector4(a[0][0], a[1][0], a[2][0], a[3][0]);
        const Vector4 c1 = Vector4(a[0][1], a[1][1], a[2][1], a[3][1]);
        const Vector4 c2 = Vector4(a[0][2], a[1][2], a[2][2], a[3][2]);
        const Vector4 c3 = Vector4(a[0][3], a[1][3], a[2][3], a[3][3]);

        return Vector4(
            dot(c0, b),
            dot(c1, b),
            dot(c2, b),
            dot(c3, b)
        );
    }

    inline Vector4 mul(const Vector4& a, const Matrix4& b)
    {
        return Vector4(
            dot(a, b[0]),
            dot(a, b[1]),
            dot(a, b[2]),
            dot(a, b[3])
        );
    }

    inline Vector3 mul(const Matrix4& a, const Vector3& b)
    {
        const Vector4 b0 = Vector4(b.x, b.y, b.z, 1.0f);
        const Vector4 b1 = mul(a, b0);

        const float iw = 1.0f / b1.w;
        return Vector3(b1.x * iw, b1.y * iw, b1.z * iw);
    }

    inline Vector3 mul(const Vector3& a, const Matrix4& b)
    {
        const Vector4 a0 = Vector4(a.x, a.y, a.z, 1.0f);
        const Vector4 a1 = mul(a0, b);

        const float iw = 1.0f / a1.w;
        return Vector3(a1.x * iw, a1.y * iw, a1.z * iw);
    }

    inline Matrix4 mul(const Matrix4& a, const Matrix4& b)
    {
        Matrix4 result;
        result[0] = mul(a, b[0]);
        result[1] = mul(a, b[1]);
        result[2] = mul(a, b[2]);
        result[3] = mul(a, b[3]);
        return result;
    }

    namespace Math
    {
        //-------------------------------------------
        // Linear math
        //-------------------------------------------

        // Computes sign of 'x'
        inline Vector2 Sign(const Vector2& v)
        {
            return Vector2(signf(v.x), signf(v.y));
        }

        /* Computes absolute value
         */
        inline Vector2 Abs(const Vector2& v)
        {
            return Vector2(fabsf(v.x), fabsf(v.y));
        }

        /* Computes cosine
         */
        inline Vector2 Cos(const Vector2& v)
        {
            return Vector2(cosf(v.x), cosf(v.y));
        }

        /* Computes sine
         */
        inline Vector2 Sin(const Vector2& v)
        {
            return Vector2(sinf(v.x), sinf(v.y));
        }

        /* Computes tangent
         */
        inline Vector2 Tan(const Vector2& v)
        {
            return Vector2(tanf(v.x), tanf(v.y));
        }

        /* Computes hyperbolic cosine
         */
        inline Vector2 Cosh(const Vector2& v)
        {
            return Vector2(coshf(v.x), coshf(v.y));
        }

        /* Computes hyperbolic sine
         */
        inline Vector2 Sinh(const Vector2& v)
        {
            return Vector2(sinhf(v.x), sinhf(v.y));
        }

        /* Computes hyperbolic tangent
         */
        inline Vector2 Tanh(const Vector2& v)
        {
            return Vector2(tanhf(v.x), tanhf(v.y));
        }

        /* Computes inverse cosine
         */
        inline Vector2 Acos(const Vector2& v)
        {
            return Vector2(acosf(v.x), acosf(v.y));
        }

        /* Computes inverse sine
         */
        inline Vector2 Asin(const Vector2& v)
        {
            return Vector2(asinf(v.x), asinf(v.y));
        }

        /* Computes inverse tangent
         */
        inline Vector2 Atan(const Vector2& v)
        {
            return Vector2(atanf(v.x), atanf(v.y));
        }

        /* Computes inverse tangent with 2 args
         */
        inline Vector2 Atan2(const Vector2& a, const Vector2& b)
        {
            return Vector2(atan2f(a.x, b.x), atan2f(a.y, b.y));
        }

        /* Computes Euler number raised to the power 'x'
         */
        inline Vector2 Exp(const Vector2& v)
        {
            return Vector2(expf(v.x), expf(v.y));
        }

        /* Computes 2 raised to the power 'x'
         */
        inline Vector2 Exp2(const Vector2& v)
        {
            return Vector2(exp2f(v.x), exp2f(v.y));
        }

        /* Computes the base Euler number logarithm
         */
        inline Vector2 Log(const Vector2& v)
        {
            return Vector2(logf(v.x), logf(v.y));
        }

        /* Computes the base 2 logarithm
         */
        inline Vector2 Log2(const Vector2& v)
        {
            return Vector2(log2f(v.x), log2f(v.y));
        }

        /* Computes the base 10 logarithm
         */
        inline Vector2 Log10(const Vector2& v)
        {
            return Vector2(log10f(v.x), log10f(v.y));
        }

        /* Computes the value of base raised to the power exponent
         */
        inline Vector2 Pow(const Vector2& a, const Vector2& b)
        {
            return Vector2(powf(a.x, b.x), powf(a.y, b.y));
        }

        /* Get the fractal part of floating point
         */
        inline Vector2 Frac(const Vector2& v)
        {
            return Vector2(fracf(v.x), fracf(v.y));
        }

        /* Computes the floating-point remainder of the division operation x/y
         */
        inline Vector2 Fmod(const Vector2& a, const Vector2& b)
        {
            return Vector2(fmodf(a.x, b.x), fmodf(a.y, b.y));
        }

        /* Computes the smallest integer value not less than 'x'
         */
        inline Vector2 Ceil(const Vector2& v)
        {
            return Vector2(ceilf(v.x), ceilf(v.y));
        }

        /* Computes the largest integer value not greater than 'x'
         */
        inline Vector2 Floor(const Vector2& v)
        {
            return Vector2(floorf(v.x), floorf(v.y));
        }

        /* Computes the nearest integer value
         */
        inline Vector2 Round(const Vector2& v)
        {
            return Vector2(roundf(v.x), roundf(v.y));
        }

        /* Computes the nearest integer not greater in magnitude than 'x'
         */
        inline Vector2 Trunc(const Vector2& v)
        {
            return Vector2(truncf(v.x), truncf(v.y));
        }

        /* Get the smaller value
         */
        inline Vector2 Min(const Vector2& a, const Vector2& b)
        {
            return Vector2(minf(a.x, b.x), minf(a.y, b.y));
        }

        /* Get the larger value
         */
        inline Vector2 Max(const Vector2& a, const Vector2& b)
        {
            return Vector2(maxf(a.x, b.x), maxf(a.y, b.y));
        }

        /* Clamps the 'x' value to the [min, max].
         */
        inline Vector2 Clamp(const Vector2& v, const Vector2& min, const Vector2& max)
        {
            return Vector2(clampf(v.x, min.x, max.x), clampf(v.y, min.y, max.y));
        }

        /* Clamps the specified value within the range of 0 to 1
         */
        inline Vector2 Saturate(const Vector2& v)
        {
            return Vector2(saturatef(v.x), saturatef(v.y));
        }

        /* Compares two values, returning 0 or 1 based on which value is greater.
         */
        inline Vector2 Step(const Vector2& a, const Vector2& b)
        {
            return Vector2(stepf(a.x, b.x), stepf(a.y, b.y));
        }

        /* Performs a linear interpolation.
         */
        inline Vector2 Lerp(const Vector2& a, const Vector2& b, const Vector2& t)
        {
            return Vector2(lerpf(a.x, b.x, t.x), lerpf(a.y, b.y, t.y));
        }

        /* Performs a linear interpolation.
         */
        inline Vector2 Lerp(const Vector2& a, const Vector2& b, float t)
        {
            return Vector2(lerpf(a.x, b.x, t), lerpf(a.y, b.y, t));
        }

        /* Compute a smooth Hermite interpolation
         */
        inline Vector2 Smoothstep(const Vector2& a, const Vector2& b, const Vector2& t)
        {
            return Vector2(smoothstepf(a.x, b.x, t.x), smoothstepf(a.y, b.y, t.y));
        }

        /* Computes square root of 'x'.
         */
        inline Vector2 Sqrt(const Vector2& v)
        {
            return Vector2(sqrtf(v.x), sqrtf(v.y));
        }

        /* Computes inverse square root of 'x'.
         */
        inline Vector2 InvSqrt(const Vector2& v)
        {
            return Vector2(rsqrtf(v.x), rsqrtf(v.y));
        }

        /* Computes fast inverse square root of 'x'.
         */
        inline Vector2 FastSqrt(const Vector2& v)
        {
            return Vector2(fsqrtf(v.x), fsqrtf(v.y));
        }

        /* Computes fast inverse square root of 'x'.
         */
        inline Vector2 FastInvSqrt(const Vector2& v)
        {
            return Vector2(frsqrtf(v.x), frsqrtf(v.y));
        }

        //
        // @region: Graphics funtions
        //

        /* Compute dot product of two vectors
         */
        inline float Dot(const Vector2& a, const Vector2& b)
        {
            return a.x * b.x + a.y * b.y;
        }

        /* Compute squared length of vector
         */
        inline float LengthSq(const Vector2& v)
        {
            return dot(v, v);
        }

        /* Compute length of vector
         */
        inline float Length(const Vector2& v)
        {
            return sqrtf(lensqr(v));
        }

        /* Compute distance from 'a' to b
         */
        inline float Distance(const Vector2& a, const Vector2& b)
        {
            return length(a - b);
        }

        /* Compute squared distance from 'a' to b
         */
        inline float DistanceSq(const Vector2& a, const Vector2& b)
        {
            return lensqr(a - b);
        }

        /* Compute normalized vector
         */
        inline Vector2 Normalize(const Vector2& v)
        {
            const float lsqr = lensqr(v);
            if (lsqr > 0.0f)
            {
                const float f = rsqrtf(lsqr);
                return Vector2(v.x * f, v.y * f);
            }
            else
            {
                return v;
            }
        }

        /* Compute reflection vector
         */
        inline Vector2 Reflect(const Vector2& v, const Vector2& n)
        {
            return v - 2.0f * dot(v, n) * n;
        }

        /* Compute refraction vector
         */
        inline Vector2 Refract(const Vector2& v, const Vector2& n, float eta)
        {
            const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
            return k < 0.0f
                ? Vector2(0.0f)
                : eta * v - (eta * dot(v, n) + sqrtf(k)) * v;
        }

        /* Compute faceforward vector
         */
        inline Vector2 Faceforward(const Vector2& n, const Vector2& i, const Vector2& nref)
        {
            return dot(i, nref) < 0.0f ? n : -n;
        }

        /* Computes sign of 'x'
        */
        inline Vector3 Sign(const Vector3& v)
        {
            return Vector3(signf(v.x), signf(v.y), signf(v.z));
        }

        /* Computes absolute value
         */
        inline Vector3 Abs(const Vector3& v)
        {
            return Vector3(fabsf(v.x), fabsf(v.y), fabsf(v.z));
        }

        /* Computes cosine
         */
        inline Vector3 Cos(const Vector3& v)
        {
            return Vector3(cosf(v.x), cosf(v.y), cosf(v.z));
        }

        /* Computes sine
         */
        inline Vector3 Sin(const Vector3& v)
        {
            return Vector3(sinf(v.x), sinf(v.y), sinf(v.z));
        }

        /* Computes tangent
         */
        inline Vector3 Tan(const Vector3& v)
        {
            return Vector3(tanf(v.x), tanf(v.y), tanf(v.z));
        }

        /* Computes hyperbolic cosine
         */
        inline Vector3 Cosh(const Vector3& v)
        {
            return Vector3(coshf(v.x), coshf(v.y), coshf(v.z));
        }

        /* Computes hyperbolic sine
         */
        inline Vector3 Sinh(const Vector3& v)
        {
            return Vector3(sinhf(v.x), sinhf(v.y), sinhf(v.z));
        }

        /* Computes hyperbolic tangent
         */
        inline Vector3 Tanh(const Vector3& v)
        {
            return Vector3(tanhf(v.x), tanhf(v.y), tanhf(v.z));
        }

        /* Computes inverse cosine
         */
        inline Vector3 Acos(const Vector3& v)
        {
            return Vector3(acosf(v.x), acosf(v.y), acosf(v.z));
        }

        /* Computes inverse sine
         */
        inline Vector3 Asin(const Vector3& v)
        {
            return Vector3(asinf(v.x), asinf(v.y), asinf(v.z));
        }

        /* Computes inverse tangent
         */
        inline Vector3 Atan(const Vector3& v)
        {
            return Vector3(atanf(v.x),
                atanf(v.y),
                asinf(v.z));
        }

        /* Computes inverse tangent with 2 args
         */
        inline Vector3 Atan2(const Vector3& a, const Vector3& b)
        {
            return Vector3(atan2f(a.x, b.x),
                atan2f(a.y, b.y),
                atan2f(a.z, b.z));
        }

        /* Computes Euler number raised to the power 'x'
         */
        inline Vector3 Exp(const Vector3& v)
        {
            return Vector3(expf(v.x),
                expf(v.y),
                expf(v.z));
        }

        /* Computes 2 raised to the power 'x'
         */
        inline Vector3 Exp2(const Vector3& v)
        {
            return Vector3(exp2f(v.x),
                exp2f(v.y),
                exp2f(v.z));
        }

        /* Computes the base Euler number logarithm
         */
        inline Vector3 Log(const Vector3& v)
        {
            return Vector3(logf(v.x),
                logf(v.y),
                logf(v.z));
        }

        /* Computes the base 2 logarithm
         */
        inline Vector3 Log2(const Vector3& v)
        {
            return Vector3(log2f(v.x),
                log2f(v.y),
                log2f(v.z));
        }

        /* Computes the base 10 logarithm
         */
        inline Vector3 Log10(const Vector3& v)
        {
            return Vector3(log10f(v.x),
                log10f(v.y),
                log10f(v.z));
        }

        /* Computes the value of base raised to the power exponent
         */
        inline Vector3 Pow(const Vector3& a, const Vector3& b)
        {
            return Vector3(powf(a.x, b.x),
                powf(a.y, b.y),
                powf(a.z, b.z));
        }

        /* Get the fractal part of floating point
         */
        inline Vector3 Frac(const Vector3& v)
        {
            return Vector3(fracf(v.x),
                fracf(v.y),
                fracf(v.z));
        }

        /* Computes the floating-point remainder of the division operation x/y
         */
        inline Vector3 Fmod(const Vector3& a, const Vector3& b)
        {
            return Vector3(fmodf(a.x, b.x),
                fmodf(a.y, b.y),
                fmodf(a.z, b.z));
        }

        /* Computes the smallest integer value not less than 'x'
         */
        inline Vector3 Ceil(const Vector3& v)
        {
            return Vector3(ceilf(v.x),
                ceilf(v.y),
                ceilf(v.z));
        }

        /* Computes the largest integer value not greater than 'x'
         */
        inline Vector3 Floor(const Vector3& v)
        {
            return Vector3(floorf(v.x),
                floorf(v.y),
                floorf(v.z));
        }

        /* Computes the nearest integer value
         */
        inline Vector3 Round(const Vector3& v)
        {
            return Vector3(roundf(v.x),
                roundf(v.y),
                roundf(v.z));
        }

        /* Computes the nearest integer not greater in magnitude than 'x'
         */
        inline Vector3 Trunc(const Vector3& v)
        {
            return Vector3(truncf(v.x),
                truncf(v.y),
                truncf(v.z));
        }

        /* Get the smaller value
         */
        inline Vector3 Min(const Vector3& a, const Vector3& b)
        {
            return Vector3(minf(a.x, b.x),
                minf(a.y, b.y),
                minf(a.z, b.z));
        }

        /* Get the larger value
         */
        inline Vector3 Max(const Vector3& a, const Vector3& b)
        {
            return Vector3(maxf(a.x, b.x),
                maxf(a.y, b.y),
                maxf(a.z, b.z));
        }

        /* Clamps the 'x' value to the [min, max].
         */
        inline Vector3 Clamp(const Vector3& v, const Vector3& min, const Vector3& max)
        {
            return Vector3(clampf(v.x, min.x, max.x),
                clampf(v.y, min.y, max.y),
                clampf(v.z, min.z, max.z));
        }

        /* Clamps the specified value within the range of 0 to 1
         */
        inline Vector3 Saturate(const Vector3& v)
        {
            return Vector3(saturatef(v.x),
                saturatef(v.y),
                saturatef(v.z));
        }

        /* Compares two values, returning 0 or 1 based on which value is greater.
         */
        inline Vector3 Step(const Vector3& a, const Vector3& b)
        {
            return Vector3(
                stepf(a.x, b.x),
                stepf(a.y, b.y),
                stepf(a.z, b.z)
            );
        }

        /* Performs a linear interpolation.
         */
        inline Vector3 Lerp(const Vector3& a, const Vector3& b, const Vector3& t)
        {
            return Vector3(lerpf(a.x, b.x, t.x),
                lerpf(a.y, b.y, t.y),
                lerpf(a.z, b.z, t.z));
        }

        /* Performs a linear interpolation.
         */
        inline Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
        {
            return Vector3(lerpf(a.x, b.x, t),
                lerpf(a.y, b.y, t),
                lerpf(a.z, b.z, t));
        }

        /* Compute a smooth Hermite interpolation
         */
        inline Vector3 Smoothstep(const Vector3& a, const Vector3& b, const Vector3& t)
        {
            return Vector3(smoothstepf(a.x, b.x, t.x),
                smoothstepf(a.y, b.y, t.y),
                smoothstepf(a.z, b.z, t.z));
        }

        /* Computes square root of 'x'.
         */
        inline Vector3 Sqrt(const Vector3& v)
        {
            return Vector3(sqrtf(v.x),
                sqrtf(v.y),
                sqrtf(v.z));
        }

        /* Computes inverse square root of 'x'.
         */
        inline Vector3 InvSqrt(const Vector3& v)
        {
            return Vector3(rsqrtf(v.x),
                rsqrtf(v.y),
                rsqrtf(v.z));
        }

        /* Computes fast square root of 'x'.
         */
        inline Vector3 FastSqrt(const Vector3& v)
        {
            return Vector3(fsqrtf(v.x),
                fsqrtf(v.y),
                fsqrtf(v.z));
        }

        /* Computes fast inverse square root of 'x'.
         */
        inline Vector3 FastInvSqrt(const Vector3& v)
        {
            return Vector3(frsqrtf(v.x),
                frsqrtf(v.y),
                frsqrtf(v.z));
        }

        //
        // @region: Graphics functions
        //

        /* Compute cross product of two vectors
         */
        inline Vector3 Cross(const Vector3& a, const Vector3& b)
        {
            return Vector3(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }

        /* Compute dot product of two vectors
         */
        inline float Dot(const Vector3& a, const Vector3& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        /* Compute squared length of vector
         */
        inline float LengthSq(const Vector3& v)
        {
            return dot(v, v);
        }

        /* Compute length of vector
         */
        inline float Length(const Vector3& v)
        {
            return sqrtf(lensqr(v));
        }

        /* Compute distance from 'a' to b
         */
        inline float Distance(const Vector3& a, const Vector3& b)
        {
            return length(a - b);
        }

        /* Compute squared distance from 'a' to b
         */
        inline float DistanceSq(const Vector3& a, const Vector3& b)
        {
            return lensqr(a - b);
        }

        /* Compute normalized vector
         */
        inline Vector3 Normalize(const Vector3& v)
        {
            const float lsqr = lensqr(v);
            if (lsqr > 0.0f)
            {
                const float f = rsqrtf(lsqr);
                return Vector3(v.x * f, v.y * f, v.z * f);
            }
            else
            {
                return v;
            }
        }

        /* Compute reflection vector
         */
        inline Vector3 Reflect(const Vector3& v, const Vector3& n)
        {
            return v - 2.0f * dot(v, n) * n;
        }

        /* Compute refraction vector
         */
        inline Vector3 Refract(const Vector3& v, const Vector3& n, float eta)
        {
            const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
            return k < 0.0f
                ? Vector3(0.0f)
                : eta * v - (eta * dot(v, n) + sqrtf(k)) * n;
        }

        /* Compute faceforward vector
         */
        inline Vector3 Faceforward(const Vector3& n, const Vector3& i, const Vector3& nref)
        {
            return dot(i, nref) < 0.0f ? n : -n;
        }

        /* Computes sign of 'x'
     */
        inline Vector4 Sign(const Vector4& v)
        {
            return Vector4(signf(v.x),
                signf(v.y),
                signf(v.z),
                signf(v.w));
        }

        /* Computes absolute value
         */
        inline Vector4 Abs(const Vector4& v)
        {
            return Vector4(fabsf(v.x),
                fabsf(v.y),
                fabsf(v.z),
                fabsf(v.w));
        }

        /* Computes cosine
         */
        inline Vector4 Cos(const Vector4& v)
        {
            return Vector4(cosf(v.x),
                cosf(v.y),
                cosf(v.z),
                cosf(v.w));
        }

        /* Computes sine
         */
        inline Vector4 Sin(const Vector4& v)
        {
            return Vector4(sinf(v.x),
                sinf(v.y),
                sinf(v.z),
                sinf(v.w));
        }

        /* Computes tangent
         */
        inline Vector4 Tan(const Vector4& v)
        {
            return Vector4(tanf(v.x),
                tanf(v.y),
                tanf(v.z),
                tanf(v.w));
        }

        /* Computes hyperbolic cosine
         */
        inline Vector4 Cosh(const Vector4& v)
        {
            return Vector4(coshf(v.x),
                coshf(v.y),
                coshf(v.z),
                coshf(v.w));
        }

        /* Computes hyperbolic sine
         */
        inline Vector4 Sinh(const Vector4& v)
        {
            return Vector4(sinhf(v.x),
                sinhf(v.y),
                sinhf(v.z),
                sinhf(v.w));
        }

        /* Computes hyperbolic tangent
         */
        inline Vector4 Tanh(const Vector4& v)
        {
            return Vector4(tanhf(v.x),
                tanhf(v.y),
                tanhf(v.z),
                tanhf(v.w));
        }

        /* Computes inverse cosine
         */
        inline Vector4 Acos(const Vector4& v)
        {
            return Vector4(acosf(v.x),
                acosf(v.y),
                acosf(v.z),
                acosf(v.w));
        }

        /* Computes inverse sine
         */
        inline Vector4 Asin(const Vector4& v)
        {
            return Vector4(asinf(v.x),
                asinf(v.y),
                asinf(v.z),
                asinf(v.w));
        }

        /* Computes inverse tangent
         */
        inline Vector4 atan(const Vector4& v)
        {
            return Vector4(atanf(v.x),
                atanf(v.y),
                atanf(v.z),
                atanf(v.w));
        }

        /* Computes inverse tangent with 2 args
         */
        inline Vector4 Atan2(const Vector4& a, const Vector4& b)
        {
            return Vector4(atan2f(a.x, b.x), atan2f(a.y, b.y), atan2f(a.z, b.z), atan2f(a.w, b.w));
        }

        /* Computes Euler number raised to the power 'x'
         */
        inline Vector4 Exp(const Vector4& v)
        {
            return Vector4(expf(v.x), expf(v.y), expf(v.z), expf(v.w));
        }

        /* Computes 2 raised to the power 'x'
         */
        inline Vector4 Exp2(const Vector4& v)
        {
            return Vector4(exp2f(v.x), exp2f(v.y), exp2f(v.z), exp2f(v.w));
        }

        /* Computes the base Euler number logarithm
         */
        inline Vector4 Log(const Vector4& v)
        {
            return Vector4(logf(v.x), logf(v.y), logf(v.z), logf(v.w));
        }

        /* Computes the base 2 logarithm
         */
        inline Vector4 Log2(const Vector4& v)
        {
            return Vector4(log2f(v.x), log2f(v.y), log2f(v.z), log2f(v.w));
        }

        /* Computes the base 10 logarithm
         */
        inline Vector4 Log10(const Vector4& v)
        {
            return Vector4(log10f(v.x), log10f(v.y), log10f(v.z), log10f(v.w));
        }

        /* Computes the value of base raised to the power exponent
         */
        inline Vector4 Pow(const Vector4& a, const Vector4& b)
        {
            return Vector4(
                powf(a.x, b.x),
                powf(a.y, b.y),
                powf(a.z, b.z),
                powf(a.w, b.w));
        }

        /* Get the fractal part of floating point
         */
        inline Vector4 Frac(const Vector4& v)
        {
            return Vector4(
                fracf(v.x),
                fracf(v.y),
                fracf(v.z),
                fracf(v.w));
        }

        /* Computes the floating-point remainder of the division operation x/y
         */
        inline Vector4 Fmod(const Vector4& a, const Vector4& b)
        {
            return Vector4(
                fmodf(a.x, b.x),
                fmodf(a.y, b.y),
                fmodf(a.z, b.z),
                fmodf(a.w, b.w));
        }

        /* Computes the smallest integer value not less than 'x'
         */
        inline Vector4 Ceil(const Vector4& v)
        {
            return Vector4(
                ceilf(v.x),
                ceilf(v.y),
                ceilf(v.z),
                ceilf(v.w));
        }

        /* Computes the largest integer value not greater than 'x'
         */
        inline Vector4 Floor(const Vector4& v)
        {
            return Vector4(
                floorf(v.x),
                floorf(v.y),
                floorf(v.z),
                floorf(v.w));
        }

        /* Computes the nearest integer value
         */
        inline Vector4 Round(const Vector4& v)
        {
            return Vector4(
                roundf(v.x),
                roundf(v.y),
                roundf(v.z),
                roundf(v.w));
        }

        /* Computes the nearest integer not greater in magnitude than 'x'
         */
        inline Vector4 Trunc(const Vector4& v)
        {
            return Vector4(
                truncf(v.x),
                truncf(v.y),
                truncf(v.z),
                truncf(v.w));
        }

        /* Get the smaller value
         */
        inline Vector4 Min(const Vector4& a, const Vector4& b)
        {
            return Vector4(
                minf(a.x, b.x),
                minf(a.y, b.y),
                minf(a.z, b.z),
                minf(a.w, b.w));
        }

        /* Get the larger value
         */
        inline Vector4 Max(const Vector4& a, const Vector4& b)
        {
            return Vector4(
                maxf(a.x, b.x),
                maxf(a.y, b.y),
                maxf(a.z, b.z),
                maxf(a.w, b.w));
        }

        /* Clamps the 'x' value to the [min, max].
         */
        inline Vector4 Clamp(const Vector4& v, const Vector4& min, const Vector4& max)
        {
            return Vector4(
                clampf(v.x, min.x, max.x),
                clampf(v.y, min.y, max.y),
                clampf(v.z, min.z, max.z),
                clampf(v.w, min.w, max.w));
        }

        /* Clamps the specified value within the range of 0 to 1
         */
        inline Vector4 Saturate(const Vector4& v)
        {
            return Vector4(
                saturatef(v.x),
                saturatef(v.y),
                saturatef(v.z),
                saturatef(v.w));
        }

        /* Compares two values, returning 0 or 1 based on which value is greater.
         */
        inline Vector4 Step(const Vector4& a, const Vector4& b)
        {
            return Vector4(
                stepf(a.x, b.x),
                stepf(a.y, b.y),
                stepf(a.z, b.z),
                stepf(a.w, b.w));
        }

        /* Performs a linear interpolation.
         */
        inline Vector4 Lerp(const Vector4& a, const Vector4& b, const Vector4& t)
        {
            return Vector4(
                lerpf(a.x, b.x, t.x),
                lerpf(a.y, b.y, t.y),
                lerpf(a.z, b.z, t.z),
                lerpf(a.w, b.w, t.w));
        }

        /* Performs a linear interpolation.
         */
        inline Vector4 Lerp(const Vector4& a, const Vector4& b, float t)
        {
            return Vector4(
                lerpf(a.x, b.x, t),
                lerpf(a.y, b.y, t),
                lerpf(a.z, b.z, t),
                lerpf(a.w, b.w, t));
        }

        /* Compute a smooth Hermite interpolation
         */
        inline Vector4 Smoothstep(const Vector4& a, const Vector4& b, const Vector4& t)
        {
            return Vector4(
                smoothstepf(a.x, b.x, t.x),
                smoothstepf(a.y, b.y, t.y),
                smoothstepf(a.z, b.z, t.z),
                smoothstepf(a.w, b.w, t.w));
        }

        /* Computes square root of 'x'.
         */
        inline Vector4 Sqrt(const Vector4& v)
        {
            return Vector4(sqrtf(v.x), sqrtf(v.y), sqrtf(v.z), sqrtf(v.w));
        }

        /* Computes inverse square root of 'x'.
         */
        inline Vector4 InvSqrt(const Vector4& v)
        {
            return Vector4(rsqrtf(v.x), rsqrtf(v.y), rsqrtf(v.z), rsqrtf(v.w));
        }

        /* Computes fast square root of 'x'.
         */
        inline Vector4 FastSqrt(const Vector4& v)
        {
            return Vector4(fsqrtf(v.x), fsqrtf(v.y), fsqrtf(v.z), fsqrtf(v.w));
        }

        /* Computes fast inverse square root of 'x'.
         */
        inline Vector4 FastInvSqrt(const Vector4& v)
        {
            return Vector4(frsqrtf(v.x), frsqrtf(v.y), frsqrtf(v.z), frsqrtf(v.w));
        }

        //
        // @region: Graphics functions
        //

        /* Compute dot product of two vectors
         */
        inline float Dot(const Vector4& a, const Vector4& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }

        /* Compute squared length of vector
         */
        inline float LengthSq(const Vector4& v)
        {
            return dot(v, v);
        }

        /* Compute length of vector
         */
        inline float Length(const Vector4& v)
        {
            return sqrtf(lensqr(v));
        }

        /* Compute distance from 'a' to b
         */
        inline float Distance(const Vector4& a, const Vector4& b)
        {
            return length(a - b);
        }

        /* Compute squared distance from 'a' to b
         */
        inline float DistanceSq(const Vector4& a, const Vector4& b)
        {
            return lensqr(a - b);
        }

        /* Compute normalized vector
         */
        inline Vector4 Normalize(const Vector4& v)
        {
            const float lsqr = lensqr(v);
            if (lsqr > 0.0f)
            {
                const float f = rsqrtf(lsqr);
                return Vector4(v.x * f, v.y * f, v.z * f, v.w * f);
            }
            else
            {
                return v;
            }
        }

        /* Compute reflection vector
         */
        inline Vector4 Reflect(const Vector4& v, const Vector4& n)
        {
            return v - 2.0f * dot(v, n) * n;
        }

        /* Compute refraction vector
         */
        inline Vector4 Refract(const Vector4& v, const Vector4& n, float eta)
        {
            const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
            return k < 0.0f
                ? Vector4(0.0f)
                : eta * v - (eta * dot(v, n) + sqrtf(k)) * n;
        }

        /* Compute faceforward vector
         */
        inline Vector4 Faceforward(const Vector4& n, const Vector4& i, const Vector4& nref)
        {
            return dot(i, nref) < 0.0f ? n : -n;
        }

        //
        // @region: Quaternion
        //

        /* Quaternion multiplication
         */
        inline Quaterion Mul(const Quaterion& a, const Quaterion& b)
        {
            const Vector3 a3 = Vector3(a.x, a.y, a.z);
            const Vector3 b3 = Vector3(b.x, b.y, b.z);

            Vector3 v = a3 * b.w + b3 * a.w + cross(a3, b3);
            float  w = a.w * b.w - dot(a3, b3);
            return Quaterion(v.x, v.y, v.z, w);
        }

        inline Quaterion Inverse(const Quaterion& q)
        {
            return Quaterion(q.x, q.y, q.z, -q.w);
        }

        inline Quaterion Conj(const Quaterion& q)
        {
            return Quaterion(-q.x, -q.y, -q.z, q.w);
        }

        //
        // @region: Quaternion
        //

        inline Vector4 Mul(const Matrix4& a, const Vector4& b)
        {
            const Vector4 c0 = Vector4(a[0][0], a[1][0], a[2][0], a[3][0]);
            const Vector4 c1 = Vector4(a[0][1], a[1][1], a[2][1], a[3][1]);
            const Vector4 c2 = Vector4(a[0][2], a[1][2], a[2][2], a[3][2]);
            const Vector4 c3 = Vector4(a[0][3], a[1][3], a[2][3], a[3][3]);

            return Vector4(
                dot(c0, b),
                dot(c1, b),
                dot(c2, b),
                dot(c3, b)
            );
        }

        inline Vector4 Mul(const Vector4& a, const Matrix4& b)
        {
            return Vector4(
                dot(a, b[0]),
                dot(a, b[1]),
                dot(a, b[2]),
                dot(a, b[3])
            );
        }

        inline Vector3 Mul(const Matrix4& a, const Vector3& b)
        {
            const Vector4 b0 = Vector4(b.x, b.y, b.z, 1.0f);
            const Vector4 b1 = mul(a, b0);

            const float iw = 1.0f / b1.w;
            return Vector3(b1.x * iw, b1.y * iw, b1.z * iw);
        }

        inline Vector3 Mul(const Vector3& a, const Matrix4& b)
        {
            const Vector4 a0 = Vector4(a.x, a.y, a.z, 1.0f);
            const Vector4 a1 = mul(a0, b);

            const float iw = 1.0f / a1.w;
            return Vector3(a1.x * iw, a1.y * iw, a1.z * iw);
        }

        inline Matrix4 Mul(const Matrix4& a, const Matrix4& b)
        {
            Matrix4 result;
            result[0] = mul(a, b[0]);
            result[1] = mul(a, b[1]);
            result[2] = mul(a, b[2]);
            result[3] = mul(a, b[3]);
            return result;
        }

        //-------------------------------------------
        // Graphics math
        //-------------------------------------------

        inline Vector4 ToAxisAngle(const Quaterion& q)
        {
            Vector4 c = Vector4(q);
            if (c.w != 0.0f)
            {
                c = normalize(c);
            }

            Vector3 axis;
            const float den = sqrtf(1.0f - c.w * c.w);
            if (den > 0.0001f)
            {
                axis = Vector3(c.x, c.y, c.z) / den;
            }
            else
            {
                axis = Vector3(1, 0, 0);
            }

            float angle = 2.0f * cosf(c.w);
            return Vector4(axis, angle);
        }

        inline Matrix4 Transform(const Vector2& position, float rotation, const Vector2& scale)
        {
            return mul(mul(Math::Translation(position), Math::RotationZ(rotation)), Math::Scalation(scale));
        }

        inline Matrix4 Transform(const Vector3& position, const Quaterion& rotation, const Vector3& scale)
        {
            return mul(mul(Math::Translation(position), Math::Rotation(rotation)), Math::Scalation(scale));
        }

        inline Matrix4 Ortho(float l, float r, float b, float t, float n, float f)
        {
            const float x = 1.0f / (r - l);
            const float y = 1.0f / (t - b);
            const float z = 1.0f / (f - n);

            Matrix4 result;
            result[0] = Vector4(2.0f * x, 0, 0, 0);
            result[1] = Vector4(0, 2.0f * y, 0, 0);
            result[2] = Vector4(0, 0, 2.0f * z, 0);
            result[3] = Vector4(-x * (l + r), -y * (b + t), -z * (n + f), 1.0f);
            return result;
        }

        inline Matrix4 Frustum(float l, float r, float b, float t, float n, float f)
        {
            const float x = 1.0f / (r - l);
            const float y = 1.0f / (t - b);
            const float z = 1.0f / (f - n);

            Matrix4 result;
            result[0] = Vector4(2.0f * x, 0, 0, 0);
            result[1] = Vector4(0, 2.0f * y, 0, 0);
            result[2] = Vector4(x * (l + r), y * (b + t), z * (n + f), 1.0f);
            result[3] = Vector4(0, 0, 2.0f * z, 0);
            return result;
        }

        inline Matrix4 Perspective(float fov, float aspect, float znear, float zfar)
        {
            const float a = 1.0f / tanf(fov * 0.5f);
            const float b = zfar / (znear - zfar);

            Matrix4 result;
            result[0] = Vector4(a / aspect, 0, 0, 0);
            result[1] = Vector4(0, a, 0, 0);
            result[2] = Vector4(0, 0, b, -1);
            result[3] = Vector4(0, 0, znear * b, 0);
            return result;
        }

        inline Matrix4 Lookat(const Vector3& eye, const Vector3& target, const Vector3& up)
        {
            const Vector3 z = normalize(eye - target);
            const Vector3 x = normalize(cross(up, z));
            const Vector3 y = normalize(cross(z, x));

            Matrix4 result;
            result[0] = Vector4(x.x, y.x, z.x, 0);
            result[1] = Vector4(x.y, y.y, z.y, 0);
            result[2] = Vector4(x.z, y.z, z.z, 0);
            result[3] = Vector4(-dot(x, eye), -dot(y, eye), -dot(z, eye), 1.0f);
            return result;
        }

        inline Matrix4 Scalation(float s)
        {
            return Scalation(s, s, s);
        }

        inline Matrix4 Scalation(const Vector2& v)
        {
            return Scalation(v.x, v.y);
        }

        inline Matrix4 Scalation(const Vector3& v)
        {
            return Scalation(v.x, v.y, v.z);
        }

        inline Matrix4 Scalation(float x, float y, float z)
        {
            return Matrix4(
                x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0,
                0, 0, 0, 1
            );
        }

        inline Matrix4 Translation(const Vector2& v)
        {
            return Translation(v.x, v.y);
        }

        inline Matrix4 Translation(const Vector3& v)
        {
            return Translation(v.x, v.y, v.z);
        }

        inline Matrix4 Translation(float x, float y, float z)
        {
            return Matrix4(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                x, y, z, 1
            );
        }

        inline Matrix4 Rotation(const Vector3& axis, float angle)
        {
            return Rotation(axis.x, axis.y, axis.z, angle);
        }

        inline Matrix4 Rotation(float x, float y, float z, float angle)
        {
            const float c = cosf(-angle);
            const float s = sinf(-angle);
            const float t = 1.0f - c;

            Matrix4 result;
            /* Row 1 */
            result[0] = Vector4(t * x * x + c,
                t * x * y - s * z,
                t * x * z + s * y,
                0.0f);

            /* Row 2 */
            result[1] = Vector4(t * x * y + s * z,
                t * y * y + c,
                t * y * z - s * x,
                0.0f);

            /* Row 3 */
            result[2] = Vector4(t * x * z - s * y,
                t * y * z + s * x,
                t * z * z + c,
                0.0f);

            /* Row 4 */
            result[3] = Vector4(0, 0, 0, 1.0f);
            return result;
        }

        inline Matrix4 RotationX(float angle)
        {
            const float s = sinf(angle);
            const float c = cosf(angle);

            return Matrix4(
                1, 0, 0, 0,
                0, c, s, 0,
                0, -s, c, 0,
                0, 0, 0, 1
            );
        }

        inline Matrix4 RotationY(float angle)
        {
            const float s = sinf(angle);
            const float c = cosf(angle);

            return Matrix4(
                c, 0, s, 0,
                0, 1, 0, 0,
                -s, 0, c, 0,
                0, 0, 0, 1
            );
        }

        inline Matrix4 RotationZ(float angle)
        {
            const float s = sinf(angle);
            const float c = cosf(angle);

            return Matrix4(
                c, s, 0, 0,
                -s, c, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            );
        }

        inline Matrix4 Rotation(const Quaterion& quaternion)
        {
            Vector4 axisangle = ToAxisAngle(quaternion);
            return Rotation(axisangle.x, axisangle.y, axisangle.z, axisangle.w);
        }

        inline void Decompose(const Matrix4& m, Vector3* scalation, Quaterion* quaternion, Vector3* translation)
        {
            if (translation)
            {
                *translation = Vector3(m[3][0], m[3][1], m[3][2]);
            }

            if (!scalation && !quaternion)
            {
                return;
            }

            Vector3 xaxis(m[0][0], m[0][1], m[0][2]);
            Vector3 yaxis(m[1][0], m[1][1], m[1][2]);
            Vector3 zaxis(m[2][0], m[2][1], m[2][2]);

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
                *scalation = Vector3(scale_x, scale_y, scale_z);
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

        inline void Decompose(const Matrix4& m, Vector3* scalation, Vector3* axis, float* angle, Vector3* translation)
        {
            if (axis || angle)
            {
                Quaterion Quaterion;
                Decompose(m, scalation, &Quaterion, translation);

                Vector4 axisangle = ToAxisAngle(Quaterion);
                *axis = Vector3(axisangle);
                *angle = axisangle.w;
            }
            else
            {
                Decompose(m, scalation, (Quaterion*)0, translation);
            }
        }
    }
}