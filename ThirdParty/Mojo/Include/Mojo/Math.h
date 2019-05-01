#pragma once

#include <math.h>

inline namespace Mojo
{
    struct vec2;
    struct vec3;
    struct vec4;
    struct quat;
    struct mat4;

    struct vec2
    {
        float x, y;

        constexpr vec2(void)
            : x(0)
            , y(0)
        {
        }

        inline vec2(float x, float y)
            : x(x)
            , y(y)
        {
        }

        explicit vec2(float s);
        explicit vec2(const vec3& v);
        explicit vec2(const vec4& v);
        explicit vec2(const quat& q);
    };

    struct vec3
    {
        float x, y, z;

        constexpr vec3(void)
            : x(0)
            , y(0)
            , z(0)
        {
        }

        inline vec3(float x, float y, float z = 0.0f)
            : x(x)
            , y(y)
            , z(z)
        {
        }

        explicit vec3(float s);
        explicit vec3(const vec4& v);
        explicit vec3(const quat& v);
        explicit vec3(const vec2& v, float z = 0.0f);
    };

    struct vec4
    {
        float x, y, z, w;

        constexpr vec4(void)
            : x(0)
            , y(0)
            , z(0)
            , w(0)
        {
        }

        inline vec4(float x, float y, float z, float w)
            : x(x)
            , y(y)
            , z(z)
            , w(w)
        {
        }

        explicit vec4(float s);
        explicit vec4(const quat& v);
        explicit vec4(const vec3& v, float w);
        explicit vec4(const vec2& v, float z, float w);

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
        explicit quat(const vec4& v);
        explicit quat(const vec3& v, float z);
        explicit quat(const vec2& v, float z, float w);

        static quat euler(float x, float y, float z);

        static vec4 toaxis(const quat& q);
        static quat axisangle(const vec3& axis, float angle);
    };

    struct mat4
    {
        vec4 rows[4];

        constexpr mat4(void) {}

        inline mat4(float s)
        {
            rows[0] = vec4(s, 0, 0, 0);
            rows[1] = vec4(0, s, 0, 0);
            rows[2] = vec4(0, 0, s, 0);
            rows[3] = vec4(0, 0, 0, s);
        }

        inline mat4(const vec4& m0, const vec4& m1, const vec4& m2, const vec4& m3)
        {
            rows[0] = m0;
            rows[1] = m1;
            rows[2] = m2;
            rows[3] = m3;
        }

        inline mat4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33)
        {
            rows[0] = vec4(m00, m01, m02, m03);
            rows[1] = vec4(m10, m11, m12, m13);
            rows[2] = vec4(m20, m21, m22, m23);
            rows[3] = vec4(m30, m31, m32, m33);
        }

        vec4& operator[](int index)
        {
            return rows[index];
        }

        const vec4& operator[](int index) const
        {
            return rows[index];
        }

        static mat4 scalation(float s);
        static mat4 scalation(const vec2& v);
        static mat4 scalation(const vec3& v);
        static mat4 scalation(float x, float y, float z = 1.0f);

        static mat4 translation(const vec2& v);
        static mat4 translation(const vec3& v);
        static mat4 translation(float x, float y, float z = 0.0f);

        static mat4 rotation(const quat& quaternion);
        static mat4 rotation(const vec3& axis, float angle);
        static mat4 rotation(float x, float y, float z, float angle);

        static mat4 rotation_x(float angle);
        static mat4 rotation_y(float angle);
        static mat4 rotation_z(float angle);

        static mat4 lookat(const vec3& eye, const vec3& target, const vec3& up);

        static mat4 ortho(float l, float r, float b, float t, float n, float f);
        static mat4 frustum(float l, float r, float b, float t, float n, float f);
        static mat4 perspective(float fov, float aspect, float znear, float zfar);

        static void decompose(const mat4& m, vec3* scalation, quat* quaternion, vec3* translation);
        static void decompose(const mat4& m, vec3* scalation, vec3* axis, float* angle, vec3* translation);
    };

    inline vec2::vec2(float s)
        : x(s)
        , y(s)
    {
    }

    inline vec2::vec2(const vec3& v)
        : x(v.x)
        , y(v.y)
    {
    }

    inline vec2::vec2(const vec4& v)
        : x(v.x)
        , y(v.y)
    {
    }

    inline vec2::vec2(const quat& q)
        : x(q.x)
        , y(q.y)
    {
    }

    inline vec3::vec3(float s)
        : x(s)
        , y(s)
        , z(s)
    {
    }

    inline vec3::vec3(const vec2& v, float z)
        : x(v.x)
        , y(v.y)
        , z(z)
    {
    }

    inline vec3::vec3(const vec4& v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
    {
    }

    inline vec3::vec3(const quat& q)
        : x(q.x)
        , y(q.y)
        , z(q.z)
    {
    }

    inline vec4::vec4(float s)
        : x(s)
        , y(s)
        , z(s)
        , w(s)
    {
    }

    inline vec4::vec4(const quat& q)
        : x(q.x)
        , y(q.y)
        , z(q.z)
        , w(q.w)
    {
    }

    inline vec4::vec4(const vec3& v, float w)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(w)
    {
    }

    inline vec4::vec4(const vec2& v, float z, float w)
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

    inline quat::quat(const vec4& v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(v.w)
    {
    }

    inline quat::quat(const vec3& v, float w)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(w)
    {
    }

    inline quat::quat(const vec2& v, float z, float w)
        : x(v.x)
        , y(v.y)
        , z(z)
        , w(w)
    {
    }

    inline vec2 operator-(const vec2& v)
    {
        return vec2(-v.x, -v.y);
    }

    inline const vec2& operator+(const vec2& v)
    {
        return v;
    }

    inline vec2& operator--(vec2& v)
    {
        --v.x;
        --v.y;
        return v;
    }

    inline vec2& operator++(vec2& v)
    {
        ++v.x;
        ++v.y;
        return v;
    }

    inline vec2 operator--(vec2& v, int)
    {
        const vec2 result = v;

        v.x--;
        v.y--;

        return result;
    }

    inline vec2 operator++(vec2& v, int)
    {
        const vec2 result = v;

        v.x++;
        v.y++;

        return result;
    }

    inline vec2 operator+(const vec2& a, const vec2& b)
    {
#if MATH_ENABLE_NEON   
        return vec2(vadd_f32(a, b));
#else
        return vec2(a.x + b.x, a.y + b.y);
#endif
    }

    inline vec2 operator-(const vec2& a, const vec2& b)
    {
#if MATH_ENABLE_NEON   
        return vec2(vsub_f32(a, b));
#else
        return vec2(a.x - b.x, a.y - b.y);
#endif
    }

    inline vec2 operator*(const vec2& a, const vec2& b)
    {
#if MATH_ENABLE_NEON   
        return vec2(vmul_f32(a, b));
#else
        return vec2(a.x * b.x, a.y * b.y);
#endif
    }

    inline vec2 operator/(const vec2& a, const vec2& b)
    {
#if MATH_ENABLE_NEON && 0 // experimental
        vec2 res;
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
        return vec2(a.x / b.x, a.y / b.y);
#endif
    }

    inline vec2 operator+(const vec2& a, float b)
    {
        return a + vec2(b);
    }

    inline vec2 operator-(const vec2& a, float b)
    {
        return a - vec2(b);
    }

    inline vec2 operator*(const vec2& a, float b)
    {
        return a * vec2(b);
    }

    inline vec2 operator/(const vec2& a, float b)
    {
        return a / vec2(b);
    }

    inline vec2 operator+(float a, const vec2& b)
    {
        return vec2(a) + b;
    }

    inline vec2 operator-(float a, const vec2& b)
    {
        return vec2(a) - b;
    }

    inline vec2 operator*(float a, const vec2& b)
    {
        return vec2(a) * b;
    }

    inline vec2 operator/(float a, const vec2& b)
    {
        return vec2(a) / b;
    }

    inline vec2& operator+=(vec2& a, const vec2& b)
    {
        return (a = a + b);
    }

    inline vec2& operator+=(vec2& a, float b)
    {
        return (a = a + b);
    }

    inline vec2& operator-=(vec2& a, const vec2& b)
    {
        return (a = a - b);
    }

    inline vec2& operator-=(vec2& a, float b)
    {
        return (a = a - b);
    }

    inline vec2& operator*=(vec2& a, const vec2& b)
    {
        return (a = a * b);
    }

    inline vec2& operator*=(vec2& a, float b)
    {
        return (a = a * b);
    }

    inline vec2& operator/=(vec2& a, const vec2& b)
    {
        return (a = a / b);
    }

    inline vec2& operator/=(vec2& a, float b)
    {
        return (a = a + b);
    }

    inline bool operator==(const vec2& a, const vec2& b)
    {
        return a.x == b.x && a.y == b.y;
    }

    inline bool operator!=(const vec2& a, const vec2& b)
    {
        return a.x != b.x || a.y != b.y;
    }

    inline vec3 operator-(const vec3& v)
    {
        return vec3(-v.x, -v.y, -v.z);
    }

    inline const vec3& operator+(const vec3& v)
    {
        return v;
    }

    inline vec3& operator--(vec3& v)
    {
        --v.x;
        --v.y;
        --v.z;
        return v;
    }

    inline vec3& operator++(vec3& v)
    {
        ++v.x;
        ++v.y;
        ++v.z;
        return v;
    }

    inline vec3 operator--(vec3& v, int)
    {
        const vec3 result = v;

        v.x--;
        v.y--;
        v.z--;

        return result;
    }

    inline vec3 operator++(vec3& v, int)
    {
        const vec3 result = v;

        v.x++;
        v.y++;
        v.z++;

        return result;
    }

    inline vec3 operator+(const vec3& a, const vec3& b)
    {
        return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    inline vec3 operator-(const vec3& a, const vec3& b)
    {
        return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    inline vec3 operator*(const vec3& a, const vec3& b)
    {
        return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
    }

    inline vec3 operator/(const vec3& a, const vec3& b)
    {
        return vec3(a.x / b.x, a.y / b.y, a.z / b.z);
    }

    inline vec3 operator+(const vec3& a, float b)
    {
        return vec3(a.x + b, a.y + b, a.z + b);
    }

    inline vec3 operator-(const vec3& a, float b)
    {
        return vec3(a.x - b, a.y - b, a.z - b);
    }

    inline vec3 operator*(const vec3& a, float b)
    {
        return vec3(a.x * b, a.y * b, a.z * b);
    }

    inline vec3 operator/(const vec3& a, float b)
    {
        return vec3(a.x / b, a.y / b, a.z / b);
    }

    inline vec3 operator+(float a, const vec3& b)
    {
        return vec3(a + b.x, a + b.y, a + b.z);
    }

    inline vec3 operator-(float a, const vec3& b)
    {
        return vec3(a - b.x, a - b.y, a - b.z);
    }

    inline vec3 operator*(float a, const vec3& b)
    {
        return vec3(a * b.x, a * b.y, a * b.z);
    }

    inline vec3 operator/(float a, const vec3& b)
    {
        return vec3(a / b.x, a / b.y, a / b.z);
    }

    inline vec3& operator+=(vec3& a, const vec3& b)
    {
        return (a = a + b);
    }

    inline vec3& operator+=(vec3& a, float b)
    {
        return (a = a + b);
    }

    inline vec3& operator-=(vec3& a, const vec3& b)
    {
        return (a = a - b);
    }

    inline vec3& operator-=(vec3& a, float b)
    {
        return (a = a - b);
    }

    inline vec3& operator*=(vec3& a, const vec3& b)
    {
        return (a = a * b);
    }

    inline vec3& operator*=(vec3& a, float b)
    {
        return (a = a * b);
    }

    inline vec3& operator/=(vec3& a, const vec3& b)
    {
        return (a = a / b);
    }

    inline vec3& operator/=(vec3& a, float b)
    {
        return (a = a + b);
    }

    inline bool operator==(const vec3& a, const vec3& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }

    inline bool operator!=(const vec3& a, const vec3& b)
    {
        return a.x != b.x || a.y != b.y || a.z != b.z;
    }

    inline vec4 operator-(const vec4& v)
    {
        return vec4(-v.x, -v.y, -v.z, -v.w);
    }

    inline const vec4& operator+(const vec4& v)
    {
        return v;
    }

    inline vec4& operator--(vec4& v)
    {
        --v.x;
        --v.y;
        --v.z;
        --v.w;
        return v;
    }

    inline vec4& operator++(vec4& v)
    {
        ++v.x;
        ++v.y;
        ++v.z;
        ++v.w;
        return v;
    }

    inline vec4 operator--(vec4& v, int)
    {
        const vec4 result = v;

        v.x--;
        v.y--;
        v.z--;
        v.w--;

        return result;
    }

    inline vec4 operator++(vec4& v, int)
    {
        const vec4 result = v;

        v.x++;
        v.y++;
        v.z++;
        v.w++;

        return result;
    }

    inline vec4 operator+(const vec4& a, const vec4& b)
    {
        return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    inline vec4 operator-(const vec4& a, const vec4& b)
    {
        return vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }

    inline vec4 operator*(const vec4& a, const vec4& b)
    {
        return vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
    }

    inline vec4 operator/(const vec4& a, const vec4& b)
    {
        return vec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
    }

    inline vec4 operator+(const vec4& a, float b)
    {
        return vec4(a.x + b, a.y + b, a.z + b, a.w + b);
    }

    inline vec4 operator-(const vec4& a, float b)
    {
        return vec4(a.x - b, a.y - b, a.z - b, a.w - b);
    }

    inline vec4 operator*(const vec4& a, float b)
    {
        return vec4(a.x * b, a.y * b, a.z * b, a.w * b);
    }

    inline vec4 operator/(const vec4& a, float b)
    {
        return vec4(a.x / b, a.y / b, a.z / b, a.w / b);
    }

    inline vec4 operator+(float a, const vec4& b)
    {
        return vec4(a + b.x, a + b.y, a + b.z, a + b.w);
    }

    inline vec4 operator-(float a, const vec4& b)
    {
        return vec4(a - b.x, a - b.y, a - b.z, a - b.w);
    }

    inline vec4 operator*(float a, const vec4& b)
    {
        return vec4(a * b.x, a * b.y, a * b.z, a * b.w);
    }

    inline vec4 operator/(float a, const vec4& b)
    {
        return vec4(a / b.x, a / b.y, a / b.z, a / b.w);
    }

    inline vec4& operator+=(vec4& a, const vec4& b)
    {
        return (a = a + b);
    }

    inline vec4& operator+=(vec4& a, float b)
    {
        return (a = a + b);
    }

    inline vec4& operator-=(vec4& a, const vec4& b)
    {
        return (a = a - b);
    }

    inline vec4& operator-=(vec4& a, float b)
    {
        return (a = a - b);
    }

    inline vec4& operator*=(vec4& a, const vec4& b)
    {
        return (a = a * b);
    }

    inline vec4& operator*=(vec4& a, float b)
    {
        return (a = a * b);
    }

    inline vec4& operator/=(vec4& a, const vec4& b)
    {
        return (a = a / b);
    }

    inline vec4& operator/=(vec4& a, float b)
    {
        return (a = a + b);
    }

    inline bool operator==(const vec4& a, const vec4& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }

    inline bool operator!=(const vec4& a, const vec4& b)
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

    inline bool operator==(const mat4& a, const mat4& b)
    {
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
    }

    inline bool operator!=(const mat4& a, const mat4& b)
    {
        return a[0] == b[0] || a[1] == b[1] || a[2] == b[2] || a[3] == b[3];
    }

    // Computes sign of 'x'
    inline float sign(float x)
    {
        return x < 0.0f ? -1.0f : x == 0.0f ? 0 : 1.0f;
    }
    
    /* Computes absolute value
    */
    inline float abs(float x)
    {
        return fabsf(x);
    }

    /* Computes cosine
     */
    inline float cos(float x)
    {
        return cosf(x);
    }

    /* Computes sine
     */
    inline float sin(float x)
    {
        return sinf(x);
    }

    /* Computes tangent
     */
    inline float tan(float x)
    {
        return tanf(x);
    }

    /* Computes hyperbolic cosine
     */
    inline float cosh(float x)
    {
        return coshf(x);
    }

    /* Computes hyperbolic sine
     */
    inline float sinh(float x)
    {
        return sinhf(x);
    }

    /* Computes hyperbolic tangent
     */
    inline float tanh(float x)
    {
        return tanhf(x);
    }

    /* Computes inverse cosine
     */
    inline float acos(float x)
    {
        return acosf(x);
    }

    /* Computes inverse sine
     */
    inline float asin(float x)
    {
        return asinf(x);
    }

    /* Computes inverse tangent
     */
    inline float atan(float x)
    {
        return atanf(x);
    }

    /* Computes inverse tangent with 2 args
     */
    inline float atan2(float y, float x)
    {
        return atan2f(y, x);
    }

    /* Computes Euler number raised to the power 'x'
     */
    inline float exp(float x)
    {
        return expf(x);
    }

    /* Computes 2 raised to the power 'x'
     */
    inline float exp2(float x)
    {
        return exp2f(x);
    }

    /* Computes the base Euler number logarithm
     */
    inline float log(float x)
    {
        return logf(x);
    }

    /* Computes the base 2 logarithm
     */
    inline float log2(float x)
    {
        return log2f(x);
    }

    /* Computes the base 10 logarithm
     */
    inline float log10(float x)
    {
        return log10f(x);
    }

    /* Computes the value of base raised to the power exponent
     */
    inline float pow(float x, float y)
    {
        return powf(x, y);
    }

    /* Computes the floating-point remainder of the division operation x/y
     */
    inline float fmod(float x, float y)
    {
        return fmodf(x, y);
    }

    /* Computes the smallest integer value not less than 'x'
     */
    inline float ceil(float x)
    {
        return ceilf(x);
    }

    /* Computes the largest integer value not greater than 'x'
     */
    inline float floor(float x)
    {
        return floorf(x);
    }

    /* Computes the nearest integer value
     */
    inline float round(float x)
    {
        return roundf(x);
    }

    /* Computes the nearest integer not greater in magnitude than 'x'
     */
    inline float trunc(float x)
    {
        return truncf(x);
    }

    /* Get the fractal part of floating point
    */
    inline float frac(float x)
    {
        return modff(x, 0);
    }

    /* Get the smaller value
     */
    inline float min(float x, float y)
    {
        return x < y ? x : y;
    }

    /* Get the larger value
     */
    inline float max(float x, float y)
    {
        return x > y ? x : y;
    }

    /* Clamps the 'x' value to the [min, max].
     */
    inline float clamp(float x, float min, float max)
    {
        return x < min ? min : (x > max ? max : x);
    }

    /* Clamps the specified value within the range of 0 to 1
     */
    inline float saturate(float x)
    {
        return clamp(x, 0.0f, 1.0f);
    }

    /* Compares two values, returning 0 or 1 based on which value is greater.
     */
    inline float step(float y, float x)
    {
        return x >= y;
    }

    /* Performs a linear interpolation.
     */
    inline float lerp(float x, float y, float s)
    {
        return x + (y - x) * s;
    }

    /* Compute a smooth Hermite interpolation
     * @return: 0 if x <= min
     *          1 if x >= max
     *          (0, 1) otherwise
     */
    inline float smoothstep(float min, float max, float x)
    {
        return (clamp(x, min, max) - min) / (max - min);
    }

    /* Computes square root of 'x'.
     */
    inline float sqrt(float x)
    {
        return sqrtf(x);
    }

    /* Computes inverse square root of 'x'.
     */
    inline float rsqrt(float x)
    {
        return 1.0f / sqrtf(x);
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline float frsqrt(float x)
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
    inline float fsqrt(float x)
    {
        return x == 0.0f ? 0.0f : 1.0f / frsqrt(x);
    }

    // Computes sign of 'x'
    inline vec2 sign(const vec2& v)
    {
        return vec2(sign(v.x), sign(v.y));
    }

    /* Computes absolute value
     */
    inline vec2 abs(const vec2& v)
    {
        return vec2(abs(v.x), abs(v.y));
    }

    /* Computes cosine
     */
    inline vec2 cos(const vec2& v)
    {
        return vec2(cos(v.x),
            cos(v.y));
    }

    /* Computes sine
     */
    inline vec2 sin(const vec2& v)
    {
        return vec2(sin(v.x),
            sin(v.y));
    }

    /* Computes tangent
     */
    inline vec2 tan(const vec2& v)
    {
        return vec2(tan(v.x),
            tan(v.y));
    }

    /* Computes hyperbolic cosine
     */
    inline vec2 cosh(const vec2& v)
    {
        return vec2(cosh(v.x),
            cosh(v.y));
    }

    /* Computes hyperbolic sine
     */
    inline vec2 sinh(const vec2& v)
    {
        return vec2(sinh(v.x),
            sinh(v.y));
    }

    /* Computes hyperbolic tangent
     */
    inline vec2 tanh(const vec2& v)
    {
        return vec2(tanh(v.x),
            tanh(v.y));
    }

    /* Computes inverse cosine
     */
    inline vec2 acos(const vec2& v)
    {
        return vec2(acos(v.x),
            acos(v.y));
    }

    /* Computes inverse sine
     */
    inline vec2 asin(const vec2& v)
    {
        return vec2(asin(v.x),
            asin(v.y));
    }

    /* Computes inverse tangent
     */
    inline vec2 atan(const vec2& v)
    {
        return vec2(atan(v.x),
            atan(v.y));
    }

    /* Computes inverse tangent with 2 args
     */
    inline vec2 atan2(const vec2& a, const vec2& b)
    {
        return vec2(atan2(a.x, b.x),
            atan2(a.y, b.y));
    }

    /* Computes Euler number raised to the power 'x'
     */
    inline vec2 exp(const vec2& v)
    {
        return vec2(exp(v.x),
            exp(v.y));
    }

    /* Computes 2 raised to the power 'x'
     */
    inline vec2 exp2(const vec2& v)
    {
        return vec2(exp2(v.x),
            exp2(v.y));
    }

    /* Computes the base Euler number logarithm
     */
    inline vec2 log(const vec2& v)
    {
        return vec2(log(v.x),
            log(v.y));
    }

    /* Computes the base 2 logarithm
     */
    inline vec2 log2(const vec2& v)
    {
        return vec2(log2(v.x),
            log2(v.y));
    }

    /* Computes the base 10 logarithm
     */
    inline vec2 log10(const vec2& v)
    {
        return vec2(log10(v.x),
            log10(v.y));
    }

    /* Computes the value of base raised to the power exponent
     */
    inline vec2 pow(const vec2& a, const vec2& b)
    {
        return vec2(pow(a.x, b.x),
            pow(a.y, b.y));
    }

    /* Get the fractal part of floating point
     */
    inline vec2 frac(const vec2& v)
    {
        return vec2(frac(v.x), frac(v.y));
    }

    /* Computes the floating-point remainder of the division operation x/y
     */
    inline vec2 fmod(const vec2& a, const vec2& b)
    {
        return vec2(fmod(a.x, b.x),
            fmod(a.y, b.y));
    }

    /* Computes the smallest integer value not less than 'x'
     */
    inline vec2 ceil(const vec2& v)
    {
        return vec2(ceil(v.x),
            ceil(v.y));
    }

    /* Computes the largest integer value not greater than 'x'
     */
    inline vec2 floor(const vec2& v)
    {
        return vec2(floor(v.x),
            floor(v.y));
    }

    /* Computes the nearest integer value
     */
    inline vec2 round(const vec2& v)
    {
        return vec2(round(v.x),
            round(v.y));
    }

    /* Computes the nearest integer not greater in magnitude than 'x'
     */
    inline vec2 trunc(const vec2& v)
    {
        return vec2(trunc(v.x),
            trunc(v.y));
    }

    /* Get the smaller value
     */
    inline vec2 min(const vec2& a, const vec2& b)
    {
        return vec2(min(a.x, b.x),
            min(a.y, b.y));
    }

    /* Get the larger value
     */
    inline vec2 max(const vec2& a, const vec2& b)
    {
        return vec2(max(a.x, b.x),
            max(a.y, b.y));
    }

    /* Clamps the 'x' value to the [min, max].
     */
    inline vec2 clamp(const vec2& v, const vec2& min, const vec2& max)
    {
        return vec2(clamp(v.x, min.x, max.x),
            clamp(v.y, min.y, max.y));
    }

    /* Clamps the specified value within the range of 0 to 1
     */
    inline vec2 saturate(const vec2& v)
    {
        return vec2(saturate(v.x),
            saturate(v.y));
    }

    /* Compares two values, returning 0 or 1 based on which value is greater.
     */
    inline vec2 step(const vec2& a, const vec2& b)
    {
        return vec2(step(a.x, b.x),
            step(a.y, b.y));
    }

    /* Performs a linear interpolation.
     */
    inline vec2 lerp(const vec2& a, const vec2& b, const vec2& t)
    {
        return vec2(lerp(a.x, b.x, t.x),
            lerp(a.y, b.y, t.y));
    }

    /* Performs a linear interpolation.
     */
    inline vec2 lerp(const vec2& a, const vec2& b, float t)
    {
        return vec2(lerp(a.x, b.x, t),
            lerp(a.y, b.y, t));
    }

    /* Compute a smooth Hermite interpolation
     */
    inline vec2 smoothstep(const vec2& a, const vec2& b, const vec2& t)
    {
        return vec2(smoothstep(a.x, b.x, t.x),
            smoothstep(a.y, b.y, t.y));
    }

    /* Computes square root of 'x'.
     */
    inline vec2 sqrt(const vec2& v)
    {
        return vec2(sqrt(v.x),
            sqrt(v.y));
    }

    /* Computes inverse square root of 'x'.
     */
    inline vec2 rsqrt(const vec2& v)
    {
        return vec2(rsqrt(v.x),
            rsqrt(v.y));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline vec2 fsqrt(const vec2& v)
    {
        return vec2(fsqrt(v.x),
            fsqrt(v.y));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline vec2 frsqrt(const vec2& v)
    {
        return vec2(frsqrt(v.x),
            frsqrt(v.y));
    }

    //
    // @region: Graphics funtions
    //

    /* Compute dot product of two vectors
     */
    inline float dot(const vec2& a, const vec2& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    /* Compute squared length of vector
     */
    inline float lensqr(const vec2& v)
    {
        return dot(v, v);
    }

    /* Compute length of vector
     */
    inline float length(const vec2& v)
    {
        return sqrt(lensqr(v));
    }

    /* Compute distance from 'a' to b
     */
    inline float distance(const vec2& a, const vec2& b)
    {
        return length(a - b);
    }

    /* Compute squared distance from 'a' to b
     */
    inline float distsqr(const vec2& a, const vec2& b)
    {
        return lensqr(a - b);
    }

    /* Compute normalized vector
     */
    inline vec2 normalize(const vec2& v)
    {
        const float lsqr = lensqr(v);
        if (lsqr > 0.0f)
        {
            const float f = rsqrt(lsqr);
            return vec2(v.x * f, v.y * f);
        }
        else
        {
            return v;
        }
    }

    /* Compute reflection vector
     */
    inline vec2 reflect(const vec2& v, const vec2& n)
    {
        return v - 2.0f * dot(v, n) * n;
    }

    /* Compute refraction vector
     */
    inline vec2 refract(const vec2& v, const vec2& n, float eta)
    {
        const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
        return k < 0.0f
            ? vec2(0.0f)
            : eta * v - (eta * dot(v, n) + sqrt(k)) * v;
    }

    /* Compute faceforward vector
     */
    inline vec2 faceforward(const vec2& n, const vec2& i, const vec2& nref)
    {
        return dot(i, nref) < 0.0f ? n : -n;
    }
    /* Computes sign of 'x'
 */
    inline vec3 sign(const vec3& v)
    {
        return vec3(sign(v.x),
            sign(v.y),
            sign(v.z));
    }

    /* Computes absolute value
     */
    inline vec3 abs(const vec3& v)
    {
        return vec3(abs(v.x),
            abs(v.y),
            abs(v.z));
    }

    /* Computes cosine
     */
    inline vec3 cos(const vec3& v)
    {
        return vec3(cos(v.x),
            cos(v.y),
            cos(v.z));
    }

    /* Computes sine
     */
    inline vec3 sin(const vec3& v)
    {
        return vec3(sin(v.x),
            sin(v.y),
            sin(v.z));
    }

    /* Computes tangent
     */
    inline vec3 tan(const vec3& v)
    {
        return vec3(tan(v.x),
            tan(v.y),
            tan(v.z));
    }

    /* Computes hyperbolic cosine
     */
    inline vec3 cosh(const vec3& v)
    {
        return vec3(cosh(v.x),
            cosh(v.y),
            cosh(v.z));
    }

    /* Computes hyperbolic sine
     */
    inline vec3 sinh(const vec3& v)
    {
        return vec3(sinh(v.x),
            sinh(v.y),
            sinh(v.z));
    }

    /* Computes hyperbolic tangent
     */
    inline vec3 tanh(const vec3& v)
    {
        return vec3(tanh(v.x),
            tanh(v.y),
            tanh(v.z));
    }

    /* Computes inverse cosine
     */
    inline vec3 acos(const vec3& v)
    {
        return vec3(acos(v.x),
            acos(v.y),
            acos(v.z));
    }

    /* Computes inverse sine
     */
    inline vec3 asin(const vec3& v)
    {
        return vec3(asin(v.x),
            asin(v.y),
            asin(v.z));
    }

    /* Computes inverse tangent
     */
    inline vec3 atan(const vec3& v)
    {
        return vec3(atan(v.x),
            atan(v.y),
            asin(v.z));
    }

    /* Computes inverse tangent with 2 args
     */
    inline vec3 atan2(const vec3& a, const vec3& b)
    {
        return vec3(atan2(a.x, b.x),
            atan2(a.y, b.y),
            atan2(a.z, b.z));
    }

    /* Computes Euler number raised to the power 'x'
     */
    inline vec3 exp(const vec3& v)
    {
        return vec3(exp(v.x),
            exp(v.y),
            exp(v.z));
    }

    /* Computes 2 raised to the power 'x'
     */
    inline vec3 exp2(const vec3& v)
    {
        return vec3(exp2(v.x),
            exp2(v.y),
            exp2(v.z));
    }

    /* Computes the base Euler number logarithm
     */
    inline vec3 log(const vec3& v)
    {
        return vec3(log(v.x),
            log(v.y),
            log(v.z));
    }

    /* Computes the base 2 logarithm
     */
    inline vec3 log2(const vec3& v)
    {
        return vec3(log2(v.x),
            log2(v.y),
            log2(v.z));
    }

    /* Computes the base 10 logarithm
     */
    inline vec3 log10(const vec3& v)
    {
        return vec3(log10(v.x),
            log10(v.y),
            log10(v.z));
    }

    /* Computes the value of base raised to the power exponent
     */
    inline vec3 pow(const vec3& a, const vec3& b)
    {
        return vec3(pow(a.x, b.x),
            pow(a.y, b.y),
            pow(a.z, b.z));
    }

    /* Get the fractal part of floating point
     */
    inline vec3 frac(const vec3& v)
    {
        return vec3(frac(v.x),
            frac(v.y),
            frac(v.z));
    }

    /* Computes the floating-point remainder of the division operation x/y
     */
    inline vec3 fmod(const vec3& a, const vec3& b)
    {
        return vec3(fmod(a.x, b.x),
            fmod(a.y, b.y),
            fmod(a.z, b.z));
    }

    /* Computes the smallest integer value not less than 'x'
     */
    inline vec3 ceil(const vec3& v)
    {
        return vec3(ceil(v.x),
            ceil(v.y),
            ceil(v.z));
    }

    /* Computes the largest integer value not greater than 'x'
     */
    inline vec3 floor(const vec3& v)
    {
        return vec3(floor(v.x),
            floor(v.y),
            floor(v.z));
    }

    /* Computes the nearest integer value
     */
    inline vec3 round(const vec3& v)
    {
        return vec3(round(v.x),
            round(v.y),
            round(v.z));
    }

    /* Computes the nearest integer not greater in magnitude than 'x'
     */
    inline vec3 trunc(const vec3& v)
    {
        return vec3(trunc(v.x),
            trunc(v.y),
            trunc(v.z));
    }

    /* Get the smaller value
     */
    inline vec3 min(const vec3& a, const vec3& b)
    {
        return vec3(min(a.x, b.x),
            min(a.y, b.y),
            min(a.z, b.z));
    }

    /* Get the larger value
     */
    inline vec3 max(const vec3& a, const vec3& b)
    {
        return vec3(max(a.x, b.x),
            max(a.y, b.y),
            max(a.z, b.z));
    }

    /* Clamps the 'x' value to the [min, max].
     */
    inline vec3 clamp(const vec3& v, const vec3& min, const vec3& max)
    {
        return vec3(clamp(v.x, min.x, max.x),
            clamp(v.y, min.y, max.y),
            clamp(v.z, min.z, max.z));
    }

    /* Clamps the specified value within the range of 0 to 1
     */
    inline vec3 saturate(const vec3& v)
    {
        return vec3(saturate(v.x),
            saturate(v.y),
            saturate(v.z));
    }

    /* Compares two values, returning 0 or 1 based on which value is greater.
     */
    inline vec3 step(const vec3& a, const vec3& b)
    {
        return vec3(
            step(a.x, b.x),
            step(a.y, b.y),
            step(a.z, b.z)
        );
    }

    /* Performs a linear interpolation.
     */
    inline vec3 lerp(const vec3& a, const vec3& b, const vec3& t)
    {
        return vec3(lerp(a.x, b.x, t.x),
            lerp(a.y, b.y, t.y),
            lerp(a.z, b.z, t.z));
    }

    /* Performs a linear interpolation.
     */
    inline vec3 lerp(const vec3& a, const vec3& b, float t)
    {
        return vec3(lerp(a.x, b.x, t),
            lerp(a.y, b.y, t),
            lerp(a.z, b.z, t));
    }

    /* Compute a smooth Hermite interpolation
     */
    inline vec3 smoothstep(const vec3& a, const vec3& b, const vec3& t)
    {
        return vec3(smoothstep(a.x, b.x, t.x),
            smoothstep(a.y, b.y, t.y),
            smoothstep(a.z, b.z, t.z));
    }

    /* Computes square root of 'x'.
     */
    inline vec3 sqrt(const vec3& v)
    {
        return vec3(sqrt(v.x),
            sqrt(v.y),
            sqrt(v.z));
    }

    /* Computes inverse square root of 'x'.
     */
    inline vec3 rsqrt(const vec3& v)
    {
        return vec3(rsqrt(v.x),
            rsqrt(v.y),
            rsqrt(v.z));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline vec3 fsqrt(const vec3& v)
    {
        return vec3(fsqrt(v.x),
            fsqrt(v.y),
            fsqrt(v.z));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline vec3 frsqrt(const vec3& v)
    {
        return vec3(frsqrt(v.x),
            frsqrt(v.y),
            frsqrt(v.z));
    }

    //
    // @region: Graphics functions
    //

    /* Compute cross product of two vectors
     */
    inline vec3 cross(const vec3& a, const vec3& b)
    {
        return vec3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    /* Compute dot product of two vectors
     */
    inline float dot(const vec3& a, const vec3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    /* Compute squared length of vector
     */
    inline float lensqr(const vec3& v)
    {
        return dot(v, v);
    }

    /* Compute length of vector
     */
    inline float length(const vec3& v)
    {
        return sqrt(lensqr(v));
    }

    /* Compute distance from 'a' to b
     */
    inline float distance(const vec3& a, const vec3& b)
    {
        return length(a - b);
    }

    /* Compute squared distance from 'a' to b
     */
    inline float distsqr(const vec3& a, const vec3& b)
    {
        return lensqr(a - b);
    }

    /* Compute normalized vector
     */
    inline vec3 normalize(const vec3& v)
    {
        const float lsqr = lensqr(v);
        if (lsqr > 0.0f)
        {
            const float f = rsqrt(lsqr);
            return vec3(v.x * f, v.y * f, v.z * f);
        }
        else
        {
            return v;
        }
    }

    /* Compute reflection vector
     */
    inline vec3 reflect(const vec3& v, const vec3& n)
    {
        return v - 2.0f * dot(v, n) * n;
    }

    /* Compute refraction vector
     */
    inline vec3 refract(const vec3& v, const vec3& n, float eta)
    {
        const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
        return k < 0.0f
            ? vec3(0.0f)
            : eta * v - (eta * dot(v, n) + sqrt(k)) * n;
    }

    /* Compute faceforward vector
     */
    inline vec3 faceforward(const vec3& n, const vec3& i, const vec3& nref)
    {
        return dot(i, nref) < 0.0f ? n : -n;
    }

    /* Computes sign of 'x'
 */
    inline vec4 sign(const vec4& v)
    {
        return vec4(sign(v.x),
            sign(v.y),
            sign(v.z),
            sign(v.w));
    }

    /* Computes absolute value
     */
    inline vec4 abs(const vec4& v)
    {
        return vec4(abs(v.x),
            abs(v.y),
            abs(v.z),
            abs(v.w));
    }

    /* Computes cosine
     */
    inline vec4 cos(const vec4& v)
    {
        return vec4(cos(v.x),
            cos(v.y),
            cos(v.z),
            cos(v.w));
    }

    /* Computes sine
     */
    inline vec4 sin(const vec4& v)
    {
        return vec4(sin(v.x),
            sin(v.y),
            sin(v.z),
            sin(v.w));
    }

    /* Computes tangent
     */
    inline vec4 tan(const vec4& v)
    {
        return vec4(tan(v.x),
            tan(v.y),
            tan(v.z),
            tan(v.w));
    }

    /* Computes hyperbolic cosine
     */
    inline vec4 cosh(const vec4& v)
    {
        return vec4(cosh(v.x),
            cosh(v.y),
            cosh(v.z),
            cosh(v.w));
    }

    /* Computes hyperbolic sine
     */
    inline vec4 sinh(const vec4& v)
    {
        return vec4(sinh(v.x),
            sinh(v.y),
            sinh(v.z),
            sinh(v.w));
    }

    /* Computes hyperbolic tangent
     */
    inline vec4 tanh(const vec4& v)
    {
        return vec4(tanh(v.x),
            tanh(v.y),
            tanh(v.z),
            tanh(v.w));
    }

    /* Computes inverse cosine
     */
    inline vec4 acos(const vec4& v)
    {
        return vec4(acos(v.x),
            acos(v.y),
            acos(v.z),
            acos(v.w));
    }

    /* Computes inverse sine
     */
    inline vec4 asin(const vec4& v)
    {
        return vec4(asin(v.x),
            asin(v.y),
            asin(v.z),
            asin(v.w));
    }

    /* Computes inverse tangent
     */
    inline vec4 atan(const vec4& v)
    {
        return vec4(atan(v.x),
            atan(v.y),
            atan(v.z),
            atan(v.w));
    }

    /* Computes inverse tangent with 2 args
     */
    inline vec4 atan2(const vec4& a, const vec4& b)
    {
        return vec4(atan2(a.x, b.x),
            atan2(a.y, b.y),
            atan2(a.z, b.z),
            atan2(a.w, b.w));
    }

    /* Computes Euler number raised to the power 'x'
     */
    inline vec4 exp(const vec4& v)
    {
        return vec4(exp(v.x),
            exp(v.y),
            exp(v.z),
            exp(v.w));
    }

    /* Computes 2 raised to the power 'x'
     */
    inline vec4 exp2(const vec4& v)
    {
        return vec4(exp2(v.x),
            exp2(v.y),
            exp2(v.z),
            exp2(v.w));
    }

    /* Computes the base Euler number logarithm
     */
    inline vec4 log(const vec4& v)
    {
        return vec4(log(v.x),
            log(v.y),
            log(v.z),
            log(v.w));
    }

    /* Computes the base 2 logarithm
     */
    inline vec4 log2(const vec4& v)
    {
        return vec4(log2(v.x),
            log2(v.y),
            log2(v.z),
            log2(v.w));
    }

    /* Computes the base 10 logarithm
     */
    inline vec4 log10(const vec4& v)
    {
        return vec4(log10(v.x),
            log10(v.y),
            log10(v.z),
            log10(v.w));
    }

    /* Computes the value of base raised to the power exponent
     */
    inline vec4 pow(const vec4& a, const vec4& b)
    {
        return vec4(pow(a.x, b.x),
            pow(a.y, b.y),
            pow(a.z, b.z),
            pow(a.w, b.w));
    }

    /* Get the fractal part of floating point
     */
    inline vec4 frac(const vec4& v)
    {
        return vec4(frac(v.x),
            frac(v.y),
            frac(v.z),
            frac(v.w));
    }

    /* Computes the floating-point remainder of the division operation x/y
     */
    inline vec4 fmod(const vec4& a, const vec4& b)
    {
        return vec4(fmod(a.x, b.x),
            fmod(a.y, b.y),
            fmod(a.z, b.z),
            fmod(a.w, b.w));
    }

    /* Computes the smallest integer value not less than 'x'
     */
    inline vec4 ceil(const vec4& v)
    {
        return vec4(ceil(v.x),
            ceil(v.y),
            ceil(v.z),
            ceil(v.w));
    }

    /* Computes the largest integer value not greater than 'x'
     */
    inline vec4 floor(const vec4& v)
    {
        return vec4(floor(v.x),
            floor(v.y),
            floor(v.z),
            floor(v.w));
    }

    /* Computes the nearest integer value
     */
    inline vec4 round(const vec4& v)
    {
        return vec4(round(v.x),
            round(v.y),
            round(v.z),
            round(v.w));
    }

    /* Computes the nearest integer not greater in magnitude than 'x'
     */
    inline vec4 trunc(const vec4& v)
    {
        return vec4(trunc(v.x),
            trunc(v.y),
            trunc(v.z),
            trunc(v.w));
    }

    /* Get the smaller value
     */
    inline vec4 min(const vec4& a, const vec4& b)
    {
        return vec4(min(a.x, b.x),
            min(a.y, b.y),
            min(a.z, b.z),
            min(a.w, b.w));
    }

    /* Get the larger value
     */
    inline vec4 max(const vec4& a, const vec4& b)
    {
        return vec4(max(a.x, b.x),
            max(a.y, b.y),
            max(a.z, b.z),
            max(a.w, b.w));
    }

    /* Clamps the 'x' value to the [min, max].
     */
    inline vec4 clamp(const vec4& v, const vec4& min, const vec4& max)
    {
        return vec4(clamp(v.x, min.x, max.x),
            clamp(v.y, min.y, max.y),
            clamp(v.z, min.z, max.z),
            clamp(v.w, min.w, max.w));
    }

    /* Clamps the specified value within the range of 0 to 1
     */
    inline vec4 saturate(const vec4& v)
    {
        return vec4(saturate(v.x),
            saturate(v.y),
            saturate(v.z),
            saturate(v.w));
    }

    /* Compares two values, returning 0 or 1 based on which value is greater.
     */
    inline vec4 step(const vec4& a, const vec4& b)
    {
        return vec4(step(a.x, b.x),
            step(a.y, b.y),
            step(a.z, b.z),
            step(a.w, b.w));
    }

    /* Performs a linear interpolation.
     */
    inline vec4 lerp(const vec4& a, const vec4& b, const vec4& t)
    {
        return vec4(lerp(a.x, b.x, t.x),
            lerp(a.y, b.y, t.y),
            lerp(a.z, b.z, t.z),
            lerp(a.w, b.w, t.w));
    }

    /* Performs a linear interpolation.
     */
    inline vec4 lerp(const vec4& a, const vec4& b, float t)
    {
        return vec4(lerp(a.x, b.x, t),
            lerp(a.y, b.y, t),
            lerp(a.z, b.z, t),
            lerp(a.w, b.w, t));
    }

    /* Compute a smooth Hermite interpolation
     */
    inline vec4 smoothstep(const vec4& a, const vec4& b, const vec4& t)
    {
        return vec4(smoothstep(a.x, b.x, t.x),
            smoothstep(a.y, b.y, t.y),
            smoothstep(a.z, b.z, t.z),
            smoothstep(a.w, b.w, t.w));
    }

    /* Computes square root of 'x'.
     */
    inline vec4 sqrt(const vec4& v)
    {
        return vec4(sqrt(v.x),
            sqrt(v.y),
            sqrt(v.z),
            sqrt(v.w));
    }

    /* Computes inverse square root of 'x'.
     */
    inline vec4 rsqrt(const vec4& v)
    {
        return vec4(rsqrt(v.x),
            rsqrt(v.y),
            rsqrt(v.z),
            rsqrt(v.w));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline vec4 fsqrt(const vec4& v)
    {
        return vec4(fsqrt(v.x),
            fsqrt(v.y),
            fsqrt(v.z),
            fsqrt(v.w));
    }

    /* Computes fast inverse square root of 'x'.
     */
    inline vec4 frsqrt(const vec4& v)
    {
        return vec4(frsqrt(v.x),
            frsqrt(v.y),
            frsqrt(v.z),
            frsqrt(v.w));
    }

    //
    // @region: Graphics functions
    //

    /* Compute dot product of two vectors
     */
    inline float dot(const vec4& a, const vec4& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    /* Compute squared length of vector
     */
    inline float lensqr(const vec4& v)
    {
        return dot(v, v);
    }

    /* Compute length of vector
     */
    inline float length(const vec4& v)
    {
        return sqrt(lensqr(v));
    }

    /* Compute distance from 'a' to b
     */
    inline float distance(const vec4& a, const vec4& b)
    {
        return length(a - b);
    }

    /* Compute squared distance from 'a' to b
     */
    inline float distsqr(const vec4& a, const vec4& b)
    {
        return lensqr(a - b);
    }

    /* Compute normalized vector
     */
    inline vec4 normalize(const vec4& v)
    {
        const float lsqr = lensqr(v);
        if (lsqr > 0.0f)
        {
            const float f = rsqrt(lsqr);
            return vec4(v.x * f, v.y * f, v.z * f, v.w * f);
        }
        else
        {
            return v;
        }
    }

    /* Compute reflection vector
     */
    inline vec4 reflect(const vec4& v, const vec4& n)
    {
        return v - 2.0f * dot(v, n) * n;
    }

    /* Compute refraction vector
     */
    inline vec4 refract(const vec4& v, const vec4& n, float eta)
    {
        const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
        return k < 0.0f
            ? vec4(0.0f)
            : eta * v - (eta * dot(v, n) + sqrt(k)) * n;
    }

    /* Compute faceforward vector
     */
    inline vec4 faceforward(const vec4& n, const vec4& i, const vec4& nref)
    {
        return dot(i, nref) < 0.0f ? n : -n;
    }

    /* Quaternion multiplication
     */
    inline vec4 qmul(const vec4& a, const vec4& b)
    {
        const vec3 a3 = vec3(a.x, a.y, a.z);
        const vec3 b3 = vec3(b.x, b.y, b.z);

        vec3 xyz = a3 * b.w + b3 * a.w + cross(a3, b3);
        float  w = a.w * b.w - dot(a3, b3);
        return vec4(xyz, w);
    }

    inline vec4 qinverse(const vec4& q)
    {
        return vec4(q.x, q.y, q.z, -q.w);
    }

    inline vec4 qconj(const vec4& q)
    {
        return vec4(-q.x, -q.y, -q.z, q.w);
    }

    inline quat quat::axisangle(const vec3& axis, float angle)
    {
        if (lensqr(axis) == 0.0f)
        {
            return quat(0, 0, 0, 1);
        }

        vec4 r = vec4(normalize(axis) * sin(angle * 0.5f), cosf(angle * 0.5f));
        return quat(r);
    }

    inline vec4 quat::toaxis(const quat& q)
    {
        vec4 c = vec4(q);
        if (c.w != 0.0f)
        {
            c = normalize(c);
        }
    
        vec3 axis;
        const float den = sqrtf(1.0f - c.w * c.w);
        if (den > 0.0001f)
        {
            axis = vec3(c.x, c.y, c.z) / den;
        }
        else
        {
            axis = vec3(1, 0, 0);
        }
    
        float angle = 2.0f * cosf(c.w);
        return vec4(axis, angle);
    }

    inline quat quat::euler(float x, float y, float z)
    {
        float r;
        float p;

        r = z * 0.5f;
        p = x * 0.5f;
        y = y * 0.5f; // Now y mean yaw

        const float c1 = cos(y);
        const float c2 = cos(p);
        const float c3 = cos(r);
        const float s1 = sin(y);
        const float s2 = sin(p);
        const float s3 = sin(r);

        return quat(
            s1 * s2 * c3 + c1 * c2 * s3,
            s1 * c2 * c3 + c1 * s2 * s3,
            c1 * s2 * c3 - s1 * c2 * s3,
            c1 * c2 * c3 - s1 * s2 * s3
        );
    }

    inline mat4 mat4::ortho(float l, float r, float b, float t, float n, float f)
    {
        const float x = 1.0f / (r - l);
        const float y = 1.0f / (t - b);
        const float z = 1.0f / (f - n);

        mat4 result;
        result[0] = vec4(2.0f * x, 0, 0, 0);
        result[1] = vec4(0, 2.0f * y, 0, 0);
        result[2] = vec4(0, 0, 2.0f * z, 0);
        result[3] = vec4(-x * (l + r), -y * (b + t), -z * (n + f), 1.0f);
        return result;
    }

    inline mat4 mat4::frustum(float l, float r, float b, float t, float n, float f)
    {
        const float x = 1.0f / (r - l);
        const float y = 1.0f / (t - b);
        const float z = 1.0f / (f - n);

        mat4 result;
        result[0] = vec4(2.0f * x, 0, 0, 0);
        result[1] = vec4(0, 2.0f * y, 0, 0);
        result[2] = vec4(x * (l + r), y * (b + t), z * (n + f), 1.0f);
        result[3] = vec4(0, 0, 2.0f * z, 0);
        return result;
    }

    inline mat4 mat4::perspective(float fov, float aspect, float znear, float zfar)
    {
        const float a = 1.0f / tan(fov * 0.5f);
        const float b = zfar / (znear - zfar);

        mat4 result;
        result[0] = vec4(a / aspect, 0, 0, 0);
        result[1] = vec4(0, a, 0, 0);
        result[2] = vec4(0, 0, b, -1);
        result[3] = vec4(0, 0, znear * b, 0);
        return result;
    }

    inline mat4 mat4::lookat(const vec3& eye, const vec3& target, const vec3& up)
    {
        const vec3 z = normalize(eye - target);
        const vec3 x = normalize(cross(up, z));
        const vec3 y = normalize(cross(z, x));

        mat4 result;
        result[0] = vec4(x.x, y.x, z.x, 0);
        result[1] = vec4(x.y, y.y, z.y, 0);
        result[2] = vec4(x.z, y.z, z.z, 0);
        result[3] = vec4(-dot(x, eye), -dot(y, eye), -dot(z, eye), 1.0f);
        return result;
    }

    inline mat4 mat4::scalation(float s)
    {
        return mat4::scalation(s, s, s);
    }

    inline mat4 mat4::scalation(const vec2& v)
    {
        return mat4::scalation(v.x, v.y);
    }

    inline mat4 mat4::scalation(const vec3& v)
    {
        return mat4::scalation(v.x, v.y, v.z);
    }

    inline mat4 mat4::scalation(float x, float y, float z)
    {
        return mat4(
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1
        );
    }

    inline mat4 mat4::translation(const vec2& v)
    {
        return mat4::translation(v.x, v.y);
    }

    inline mat4 mat4::translation(const vec3& v)
    {
        return mat4::translation(v.x, v.y, v.z);
    }

    inline mat4 mat4::translation(float x, float y, float z)
    {
        return mat4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            x, y, z, 1
        );
    }

    inline mat4 mat4::rotation(const vec3& axis, float angle)
    {
        return mat4::rotation(axis.x, axis.y, axis.z, angle);
    }

    inline mat4 mat4::rotation(float x, float y, float z, float angle)
    {
        const float c = cos(-angle);
        const float s = sin(-angle);
        const float t = 1.0f - c;

        mat4 result;
        /* Row 1 */
        result[0] = vec4(t * x * x + c,
            t * x * y - s * z,
            t * x * z + s * y,
            0.0f);

        /* Row 2 */
        result[1] = vec4(t * x * y + s * z,
            t * y * y + c,
            t * y * z - s * x,
            0.0f);

        /* Row 3 */
        result[2] = vec4(t * x * z - s * y,
            t * y * z + s * x,
            t * z * z + c,
            0.0f);

        /* Row 4 */
        result[3] = vec4(0, 0, 0, 1.0f);
        return result;
    }

    inline mat4 mat4::rotation_x(float angle)
    {
        const float s = sin(angle);
        const float c = cos(angle);

        return mat4(
            1, 0, 0, 0,
            0, c, s, 0,
            0, -s, c, 0,
            0, 0, 0, 1
        );
    }

    inline mat4 mat4::rotation_y(float angle)
    {
        const float s = sin(angle);
        const float c = cos(angle);

        return mat4(
            c, 0, s, 0,
            0, 1, 0, 0,
            -s, 0, c, 0,
            0, 0, 0, 1
        );
    }

    inline mat4 mat4::rotation_z(float angle)
    {
        const float s = sin(angle);
        const float c = cos(angle);

        return mat4(
            c, s, 0, 0,
            -s, c, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
    }

    inline mat4 mat4::rotation(const quat& quaternion)
    {
        vec4 axisangle = quat::toaxis(quaternion);
        return mat4::rotation(axisangle.x, axisangle.y, axisangle.z, axisangle.w);
    }

    inline void mat4::decompose(const mat4& m, vec3* scalation, quat* quaternion, vec3* translation)
    {
        if (translation)
        {
            *translation = vec3(m[3][0], m[3][1], m[3][2]);
        }

        if (!scalation && !quaternion)
        {
            return;
        }

        vec3 xaxis(m[0][0], m[0][1], m[0][2]);
        vec3 yaxis(m[1][0], m[1][1], m[1][2]);
        vec3 zaxis(m[2][0], m[2][1], m[2][2]);

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
            *scalation = vec3(scale_x, scale_y, scale_z);
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
            float s = 0.5f / sqrt(trace);
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
                float s = 0.5f / sqrt(1.0f + xaxis.x - yaxis.y - zaxis.z);
                quaternion->w = (yaxis.z - zaxis.y) * s;
                quaternion->x = 0.25f / s;
                quaternion->y = (yaxis.x + xaxis.y) * s;
                quaternion->z = (zaxis.x + xaxis.z) * s;
            }
            else if (yaxis.y > zaxis.z)
            {
                float s = 0.5f / sqrt(1.0f + yaxis.y - xaxis.x - zaxis.z);
                quaternion->w = (zaxis.x - xaxis.z) * s;
                quaternion->x = (yaxis.x + xaxis.y) * s;
                quaternion->y = 0.25f / s;
                quaternion->z = (zaxis.y + yaxis.z) * s;
            }
            else
            {
                float s = 0.5f / sqrt(1.0f + zaxis.z - xaxis.x - yaxis.y);
                quaternion->w = (xaxis.y - yaxis.x) * s;
                quaternion->x = (zaxis.x + xaxis.z) * s;
                quaternion->y = (zaxis.y + yaxis.z) * s;
                quaternion->z = 0.25f / s;
            }
        }
    }

    inline void mat4::decompose(const mat4& m, vec3* scalation, vec3* axis, float* angle, vec3* translation)
    {
        if (axis || angle)
        {
            quat quat;
            mat4::decompose(m, scalation, &quat, translation);

            vec4 axisangle = quat::toaxis(quat);
            *axis = vec3(axisangle);
            *angle = axisangle.w;
        }
        else
        {
            mat4::decompose(m, scalation, (quat*)0, translation);
        }
    }

    inline mat4 operator-(const mat4& m)
    {
        mat4 result;
        result[0] = -m[0];
        result[1] = -m[1];
        result[2] = -m[2];
        result[3] = -m[3];
        return result;
    }

    inline const mat4& operator+(const mat4& m)
    {
        return m;
    }

    inline mat4& operator--(mat4& m)
    {
        --m[0];
        --m[1];
        --m[2];
        --m[3];
        return m;
    }

    inline mat4& operator++(mat4& m)
    {
        ++m[0];
        ++m[1];
        ++m[2];
        ++m[3];
        return m;
    }

    inline const mat4& operator--(mat4& m, int)
    {
        m[0]--;
        m[1]--;
        m[2]--;
        m[3]--;
        return m;
    }

    inline const mat4& operator++(mat4& m, int)
    {
        m[0]++;
        m[1]++;
        m[2]++;
        m[3]++;
        return m;
    }

    inline mat4 operator+(const mat4& a, const mat4& b)
    {
        mat4 result;
        result[0] = a[0] + b[0];
        result[1] = a[1] + b[1];
        result[2] = a[2] + b[2];
        result[3] = a[3] + b[3];
        return result;
    }

    inline mat4 operator+(const mat4& a, float b)
    {
        mat4 result;
        result[0] = a[0] + b;
        result[1] = a[1] + b;
        result[2] = a[2] + b;
        result[3] = a[3] + b;
        return result;
    }

    inline mat4 operator+(float a, const mat4& b)
    {
        mat4 result;
        result[0] = a + b[0];
        result[1] = a + b[1];
        result[2] = a + b[2];
        result[3] = a + b[3];
        return result;
    }

    inline mat4 operator-(const mat4& a, const mat4& b)
    {
        mat4 result;
        result[0] = a[0] - b[0];
        result[1] = a[1] - b[1];
        result[2] = a[2] - b[2];
        result[3] = a[3] - b[3];
        return result;
    }

    inline mat4 operator-(const mat4& a, float b)
    {
        mat4 result;
        result[0] = a[0] - b;
        result[1] = a[1] - b;
        result[2] = a[2] - b;
        result[3] = a[3] - b;
        return result;
    }

    inline mat4 operator-(float a, const mat4& b)
    {
        mat4 result;
        result[0] = a - b[0];
        result[1] = a - b[1];
        result[2] = a - b[2];
        result[3] = a - b[3];
        return result;
    }

    inline mat4 operator*(const mat4& a, float b)
    {
        mat4 result;
        result[0] = a[0] * b;
        result[1] = a[1] * b;
        result[2] = a[2] * b;
        result[3] = a[3] * b;
        return result;
    }

    inline mat4 operator*(float a, const mat4& b)
    {
        mat4 result;
        result[0] = a * b[0];
        result[1] = a * b[1];
        result[2] = a * b[2];
        result[3] = a * b[3];
        return result;
    }

    inline vec4 operator*(const mat4& a, const vec4& b)
    {
        const vec4 c0 = vec4(a[0][0], a[1][0], a[2][0], a[3][0]);
        const vec4 c1 = vec4(a[0][1], a[1][1], a[2][1], a[3][1]);
        const vec4 c2 = vec4(a[0][2], a[1][2], a[2][2], a[3][2]);
        const vec4 c3 = vec4(a[0][3], a[1][3], a[2][3], a[3][3]);

        return vec4(
            dot(c0, b),
            dot(c1, b),
            dot(c2, b),
            dot(c3, b)
        );
    }

    inline vec4 operator*(const vec4& a, const mat4& b)
    {
        return vec4(
            dot(a, b[0]),
            dot(a, b[1]),
            dot(a, b[2]),
            dot(a, b[3])
        );
    }

    inline vec3 mul(const mat4& a, const vec3& b)
    {
        const vec4 b0 = vec4(b.x, b.y, b.z, 1.0f);
        const vec4 b1 = a * b0;

        const float iw = 1.0f / b1.w;
        return vec3(b1.x * iw, b1.y * iw, b1.z * iw);
    }

    inline vec3 mul(const vec3& a, const mat4& b)
    {
        const vec4 a0 = vec4(a.x, a.y, a.z, 1.0f);
        const vec4 a1 = a0 * b;

        const float iw = 1.0f / a1.w;
        return vec3(a1.x * iw, a1.y * iw, a1.z * iw);
    }

    inline mat4 operator*(const mat4& a, const mat4& b)
    {
        mat4 result;
        result[0] = a * b[0];
        result[1] = a * b[1];
        result[2] = a * b[2];
        result[3] = a * b[3];
        return result;
    }

    inline mat4 operator/(const mat4& a, const mat4& b)
    {
        mat4 result;
        result[0] = a[0] / b[0];
        result[1] = a[1] / b[1];
        result[2] = a[2] / b[2];
        result[3] = a[3] / b[3];
        return result;
    }

    inline mat4 operator/(const mat4& a, float b)
    {
        mat4 result;
        result[0] = a[0] / b;
        result[1] = a[1] / b;
        result[2] = a[2] / b;
        result[3] = a[3] / b;
        return result;
    }

    inline mat4 operator/(float a, const mat4& b)
    {
        mat4 result;
        result[0] = a / b[0];
        result[1] = a / b[1];
        result[2] = a / b[2];
        result[3] = a / b[3];
        return result;
    }

    inline mat4& operator+=(mat4& a, const mat4& b)
    {
        return (a = a + b);
    }

    inline mat4& operator+=(mat4& a, float b)
    {
        return (a = a + b);
    }

    inline mat4& operator-=(mat4& a, const mat4& b)
    {
        return (a = a - b);
    }

    inline mat4& operator-=(mat4& a, float b)
    {
        return (a = a - b);
    }

    inline mat4& operator*=(mat4& a, const mat4& b)
    {
        return (a = a * b);
    }

    inline mat4& operator*=(mat4& a, float b)
    {
        return (a = a * b);
    }

    inline mat4& operator/=(mat4& a, const mat4& b)
    {
        return (a = a / b);
    }

    inline mat4& operator/=(mat4& a, float b)
    {
        return (a = a + b);
    }
}