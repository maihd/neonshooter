using System;

namespace Mojo
{
	struct Vector2
	{
	    public float x, y;

		[Inline]
	    public this()
	    {
			this = default;
	    }

		[Inline]
	    public this(float x, float y)
	    {
			this.x = x;
			this.y = y;
	    }

		[Inline]
	    public this(float s)
		{
			this.x = s;
			this.y = s;
		}

		[Inline]
	    public this(Vector3 v)
		{
			this.x = v.x;
			this.y = v.y;
		}

		[Inline]
	    public this(Vector4 v)
		{
			this.x = v.x;
			this.y = v.y;
		}
	}

	struct Vector3
	{
	    public float x, y, z;

		[Inline]
	    public this()
	    {
			this = default;
	    }

		[Inline]
	    public this(float x, float y, float z = 0.0f)
	    {
			this.x = x;
			this.y = y;
			this.z = z;
	    }

		[Inline]
	    public this(float s)
		{
			this.x = s;
			this.y = s;
			this.z = s;
		}

		[Inline]
	    public this(Vector4 v)
		{
			this.x = v.x;
			this.y = v.y;
			this.z = v.z;
		}

		[Inline]
	    public this(Vector2 v, float z = 0.0f)
		{
			this.x = v.x;
			this.y = v.y;
			this.z = z;
		}
	}

	struct Vector4
	{
	    public float x, y, z, w;

		[Inline]
	    public this()
	    {
			this = default;
	    }

		[Inline]
	    public this(float x, float y, float z, float w)
	    {
			this.x = x;
			this.y = y;
			this.z = z;
			this.w = w;
	    }

		[Inline]
	    public this(float s)
		{
			this.x = s;
			this.y = s;
			this.z = s;
			this.w = s;
		}

		[Inline]
	    public this(Quaterion q)
		{
			this.x = q.x;
			this.y = q.y;
			this.z = q.z;
			this.w = q.w;
		}

		[Inline]
	    public this(Vector3 v, float w = 0.0f)
		{
			this.x = v.x;
			this.y = v.y;
			this.z = v.z;
			this.w = w;
		}

		[Inline]
	    public this(Vector2 v, float z, float w)
		{
			this.x = v.x;
			this.y = v.y;
			this.z = z;
			this.w = w;
		}

		public float this[int index]
		{
			get mut { return ((float*)&this)[index];  }
			set mut { ((float*)&this)[index] = value; }
		}
	}

	struct Quaterion
	{
	    public float x, y, z, w; 

		[Inline]
	    public this()
	    {
			this = default;
	    }

		
		[Inline]
	    public this(float x, float y, float z, float w)
	    {
			this.x = x;
			this.y = y;
			this.z = z;
			this.w = w;
	    }

		[Inline]
		public this(float s)
		{
			this.x = s;
			this.y = s;
			this.z = s;
			this.w = s;
		}

		[Inline]
		public this(Vector4 v)
		{
			this.x = v.x;
			this.y = v.y;
			this.z = v.z;
			this.w = v.w;
		}

		[Inline]
		public this(Vector3 v, float w = 0.0f)
		{
			this.x = v.x;
			this.y = v.y;
			this.z = v.z;
			this.w = w;
		}

		[Inline]
		public this(Vector2 v, float z, float w)
		{
			this.x = v.x;
			this.y = v.y;
			this.z = z;
			this.w = w;
		}
	}

	struct Matrix4
	{
	    public Vector4[4] rows;

	    [Inline]
		this()
		{
			this = default;
		}

		[Inline]
	    public this(float s)
	    {
	        rows[0] = Vector4(s, 0, 0, 0);
	        rows[1] = Vector4(0, s, 0, 0);
	        rows[2] = Vector4(0, 0, s, 0);
	        rows[3] = Vector4(0, 0, 0, s);
	    }

		[Inline]
	    public this(Vector4 m0, Vector4 m1, Vector4 m2, Vector4 m3)
	    {
	        rows[0] = m0;
	        rows[1] = m1;
	        rows[2] = m2;
	        rows[3] = m3;
	    }

		[Inline]
	    public this(
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

	    public ref Vector4 this[int index]
		{
			get mut { return ref rows[index];	}
			set mut { rows[index] = value; 		}
		}
	}

#if NOT_IMPL
    namespace Math
    {
        Matrix4 Transform(Vector2 position, float rotation, Vector2 scale);
        Matrix4 Transform(Vector3 position, Quaterion rotation, Vector3 scale);

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

    namespace Math
    {
        //-------------------------------------------
        // Linear math
        //-------------------------------------------

        inline int Min(int x, int y)
        {
            return x < y ? x : y;
        }

        inline int Max(int x, int y)
        {
            return x < y ? x : y;
        }

        inline int Clamp(int x, int min, int max)
        {
            return x < min ? min : (x > max ? max : x);
        }

        // Computes sign of 'x'
        inline float Sign(float x)
        {
            return x < 0.0f ? -1.0f : x == 0.0f ? 0 : 1.0f;
        }

        /* Get the fractal part of floating point
        */
        inline float Frac(float x)
        {
            return modff(x, 0);
        }

        /* Computes absolute value
         */
        inline float Abs(float v)
        {
            return v > 0.0f ? v : -v;
        }

        /* Computes cosine
         */
        inline float Cos(float v)
        {
            return cosf(v);
        }

        /* Computes sine
         */
        inline float Sin(float v)
        {
            return sinf(v);
        }

        /* Computes tangent
         */
        inline float Tan(float v)
        {
            return tanf(v);
        }

        /* Computes hyperbolic cosine
         */
        inline float Cosh(float v)
        {
            return coshf(v);
        }

        /* Computes hyperbolic sine
         */
        inline float Sinh(float v)
        {
            return sinhf(v);
        }

        /* Computes hyperbolic tangent
         */
        inline float Tanh(float v)
        {
            return tanhf(v);
        }

        /* Computes inverse cosine
         */
        inline float Acos(float v)
        {
            return acosf(v);
        }

        /* Computes inverse sine
         */
        inline float Asin(float v)
        {
            return asinf(v);
        }

        /* Computes inverse tangent
         */
        inline float Atan(float v)
        {
            return atanf(v);
        }

        /* Computes inverse tangent with 2 args
         */
        inline float Atan2(float a, float b)
        {
            return atan2f(a, b);
        }

        /* Computes Euler number raised to the power 'x'
         */
        inline float Exp(float v)
        {
            return expf(v);
        }

        /* Computes 2 raised to the power 'x'
         */
        inline float Exp2(float v)
        {
            return exp2f(v);
        }

        /* Computes the base Euler number logarithm
         */
        inline float Log(float v)
        {
            return logf(v);
        }

        /* Computes the base 2 logarithm
         */
        inline float Log2(float v)
        {
            return log2f(v);
        }

        /* Computes the base 10 logarithm
         */
        inline float Log10(float v)
        {
            return log10f(v);
        }

        /* Computes the value of base raised to the power exponent
         */
        inline float Pow(float a, float b)
        {
            return powf(a, b);
        }

        /* Computes the floating-point remainder of the division operation x/y
         */
        inline float Mod(float a, float b)
        {
            return fmodf(a, b);
        }

        /* Computes the smallest integer value not less than 'x'
         */
        inline float Ceil(float v)
        {
            return ceilf(v);
        }

        /* Computes the largest integer value not greater than 'x'
         */
        inline float Floor(float v)
        {
            return floorf(v);
        }

        /* Computes the nearest integer value
         */
        inline float Round(float v)
        {
            return roundf(v);
        }

        /* Computes the nearest integer not greater in magnitude than 'x'
         */
        inline float Trunc(float v)
        {
            return truncf(v);
        }

        /* Get the smaller value
         */
        inline float Min(float x, float y)
        {
            return x < y ? x : y;
        }

        /* Get the larger value
         */
        inline float Max(float x, float y)
        {
            return x > y ? x : y;
        }

        /* Clamps the 'x' value to the [min, max].
         */
        inline float Clamp(float x, float min, float max)
        {
            return x < min ? min : (x > max ? max : x);
        }

        /* Clamps the specified value within the range of 0 to 1
         */
        inline float Saturate(float x)
        {
            return Clamp(x, 0.0f, 1.0f);
        }

        /* Compares two values, returning 0 or 1 based on which value is greater.
         */
        inline float Step(float y, float x)
        {
            return x >= y;
        }

        /* Performs a linear interpolation.
         */
        inline float Lerp(float x, float y, float s)
        {
            return x + (y - x) * s;
        }

        /* Compute a smooth Hermite interpolation
         * @return: 0 if x <= min
         *          1 if x >= max
         *          (0, 1) otherwise
         */
        inline float Smoothstep(float min, float max, float x)
        {
            return (Clamp(x, min, max) - min) / (max - min);
        }

        /* Computes inverse square root of 'x'.
         */
        inline float Sqrt(float x)
        {
            return sqrtf(x);
        }

        /* Computes inverse square root of 'x'.
         */
        inline float InvSqrt(float x)
        {
            return 1.0f / Sqrt(x);
        }

        /* Computes fast inverse square root of 'x'.
         */
        inline float FastInvSqrt(float x)
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
        inline float FastSqrt(float x)
        {
            return x == 0.0f ? 0.0f : 1.0f / FastInvSqrt(x);
        }

        // Computes sign of 'x'
        inline Vector2 Sign(const Vector2& v)
        {
            return Vector2(Sign(v.x), Sign(v.y));
        }

        /* Computes absolute value
         */
        inline Vector2 Abs(const Vector2& v)
        {
            return Vector2(Abs(v.x), Abs(v.y));
        }

        /* Computes cosine
         */
        inline Vector2 Cos(const Vector2& v)
        {
            return Vector2(Cos(v.x), Cos(v.y));
        }

        /* Computes sine
         */
        inline Vector2 Sin(const Vector2& v)
        {
            return Vector2(Sin(v.x), Sin(v.y));
        }

        /* Computes tangent
         */
        inline Vector2 Tan(const Vector2& v)
        {
            return Vector2(Tan(v.x), Tan(v.y));
        }

        /* Computes hyperbolic cosine
         */
        inline Vector2 Cosh(const Vector2& v)
        {
            return Vector2(Cosh(v.x), Cosh(v.y));
        }

        /* Computes hyperbolic sine
         */
        inline Vector2 Sinh(const Vector2& v)
        {
            return Vector2(Sinh(v.x), Sinh(v.y));
        }

        /* Computes hyperbolic tangent
         */
        inline Vector2 Tanh(const Vector2& v)
        {
            return Vector2(Tanh(v.x), Tanh(v.y));
        }

        /* Computes inverse cosine
         */
        inline Vector2 Acos(const Vector2& v)
        {
            return Vector2(Acos(v.x), Acos(v.y));
        }

        /* Computes inverse sine
         */
        inline Vector2 Asin(const Vector2& v)
        {
            return Vector2(Asin(v.x), Asin(v.y));
        }

        /* Computes inverse tangent
         */
        inline Vector2 Atan(const Vector2& v)
        {
            return Vector2(Atan(v.x), Atan(v.y));
        }

        /* Computes inverse tangent with 2 args
         */
        inline Vector2 Atan2(const Vector2& a, const Vector2& b)
        {
            return Vector2(Atan2(a.x, b.x), Atan2(a.y, b.y));
        }

        /* Computes Euler number raised to the power 'x'
         */
        inline Vector2 Exp(const Vector2& v)
        {
            return Vector2(Exp(v.x), Exp(v.y));
        }

        /* Computes 2 raised to the power 'x'
         */
        inline Vector2 Exp2(const Vector2& v)
        {
            return Vector2(Exp2(v.x), Exp2(v.y));
        }

        /* Computes the base Euler number logarithm
         */
        inline Vector2 Log(const Vector2& v)
        {
            return Vector2(Log(v.x), Log(v.y));
        }

        /* Computes the base 2 logarithm
         */
        inline Vector2 Log2(const Vector2& v)
        {
            return Vector2(Log2(v.x), Log2(v.y));
        }

        /* Computes the base 10 logarithm
         */
        inline Vector2 Log10(const Vector2& v)
        {
            return Vector2(Log10(v.x), Log10(v.y));
        }

        /* Computes the value of base raised to the power exponent
         */
        inline Vector2 Pow(const Vector2& a, const Vector2& b)
        {
            return Vector2(Pow(a.x, b.x), Pow(a.y, b.y));
        }

        /* Get the fractal part of floating point
         */
        inline Vector2 Frac(const Vector2& v)
        {
            return Vector2(Frac(v.x), Frac(v.y));
        }

        /* Computes the floating-point remainder of the division operation x/y
         */
        inline Vector2 Mod(const Vector2& a, const Vector2& b)
        {
            return Vector2(Mod(a.x, b.x), Mod(a.y, b.y));
        }

        /* Computes the smallest integer value not less than 'x'
         */
        inline Vector2 Ceil(const Vector2& v)
        {
            return Vector2(Ceil(v.x), Ceil(v.y));
        }

        /* Computes the largest integer value not greater than 'x'
         */
        inline Vector2 Floor(const Vector2& v)
        {
            return Vector2(Floor(v.x), Floor(v.y));
        }

        /* Computes the nearest integer value
         */
        inline Vector2 Round(const Vector2& v)
        {
            return Vector2(Round(v.x), Round(v.y));
        }

        /* Computes the nearest integer not greater in magnitude than 'x'
         */
        inline Vector2 Trunc(const Vector2& v)
        {
            return Vector2(Trunc(v.x), Trunc(v.y));
        }

        /* Get the smaller value
         */
        inline Vector2 Min(const Vector2& a, const Vector2& b)
        {
            return Vector2(Min(a.x, b.x), Min(a.y, b.y));
        }

        /* Get the larger value
         */
        inline Vector2 Max(const Vector2& a, const Vector2& b)
        {
            return Vector2(Max(a.x, b.x), Max(a.y, b.y));
        }

        /* Clamps the 'x' value to the [min, max].
         */
        inline Vector2 Clamp(const Vector2& v, const Vector2& min, const Vector2& max)
        {
            return Vector2(Clamp(v.x, min.x, max.x), Clamp(v.y, min.y, max.y));
        }

        /* Clamps the specified value within the range of 0 to 1
         */
        inline Vector2 Saturate(const Vector2& v)
        {
            return Vector2(Saturate(v.x), Saturate(v.y));
        }

        /* Compares two values, returning 0 or 1 based on which value is greater.
         */
        inline Vector2 Step(const Vector2& a, const Vector2& b)
        {
            return Vector2(Step(a.x, b.x), Step(a.y, b.y));
        }

        /* Performs a linear interpolation.
         */
        inline Vector2 Lerp(const Vector2& a, const Vector2& b, const Vector2& t)
        {
            return Vector2(Lerp(a.x, b.x, t.x), Lerp(a.y, b.y, t.y));
        }

        /* Performs a linear interpolation.
         */
        inline Vector2 Lerp(const Vector2& a, const Vector2& b, float t)
        {
            return Vector2(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t));
        }

        /* Compute a smooth Hermite interpolation
         */
        inline Vector2 Smoothstep(const Vector2& a, const Vector2& b, const Vector2& t)
        {
            return Vector2(Smoothstep(a.x, b.x, t.x), Smoothstep(a.y, b.y, t.y));
        }

        /* Computes square root of 'x'.
         */
        inline Vector2 Sqrt(const Vector2& v)
        {
            return Vector2(Sqrt(v.x), Sqrt(v.y));
        }

        /* Computes inverse square root of 'x'.
         */
        inline Vector2 InvSqrt(const Vector2& v)
        {
            return Vector2(InvSqrt(v.x), InvSqrt(v.y));
        }

        /* Computes fast inverse square root of 'x'.
         */
        inline Vector2 FastSqrt(const Vector2& v)
        {
            return Vector2(FastSqrt(v.x), FastSqrt(v.y));
        }

        /* Computes fast inverse square root of 'x'.
         */
        inline Vector2 FastInvSqrt(const Vector2& v)
        {
            return Vector2(FastInvSqrt(v.x), FastInvSqrt(v.y));
        }

        //
        // @region: Graphics funtions
        //

        /* Compute Dot product of two vectors
         */
        inline float Dot(const Vector2& a, const Vector2& b)
        {
            return a.x * b.x + a.y * b.y;
        }

        /* Compute squared Length of vector
         */
        inline float LengthSq(const Vector2& v)
        {
            return Dot(v, v);
        }

        /* Compute Length of vector
         */
        inline float Length(const Vector2& v)
        {
            return Sqrt(LengthSq(v));
        }

        /* Compute distance from 'a' to b
         */
        inline float Distance(const Vector2& a, const Vector2& b)
        {
            return Length(a - b);
        }

        /* Compute squared distance from 'a' to b
         */
        inline float DistanceSq(const Vector2& a, const Vector2& b)
        {
            return LengthSq(a - b);
        }

        /* Compute normalized vector
         */
        inline Vector2 Normalize(const Vector2& v)
        {
            const float lsqr = LengthSq(v);
            if (lsqr > 0.0f)
            {
                const float f = InvSqrt(lsqr);
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
            return v - 2.0f * Dot(v, n) * n;
        }

        /* Compute refraction vector
         */
        inline Vector2 Refract(const Vector2& v, const Vector2& n, float eta)
        {
            const float k = 1.0f - eta * eta * (1.0f - Dot(v, n) * Dot(v, n));
            return k < 0.0f
                ? Vector2(0.0f)
                : eta * v - (eta * Dot(v, n) + Sqrt(k)) * v;
        }

        /* Compute faceforward vector
         */
        inline Vector2 Faceforward(const Vector2& n, const Vector2& i, const Vector2& nref)
        {
            return Dot(i, nref) < 0.0f ? n : -n;
        }

        /* Computes sign of 'x'
        */
        inline Vector3 Sign(const Vector3& v)
        {
            return Vector3(Sign(v.x), Sign(v.y), Sign(v.z));
        }

        /* Computes absolute value
         */
        inline Vector3 Abs(const Vector3& v)
        {
            return Vector3(Abs(v.x), Abs(v.y), Abs(v.z));
        }

        /* Computes cosine
         */
        inline Vector3 Cos(const Vector3& v)
        {
            return Vector3(Cos(v.x), Cos(v.y), Cos(v.z));
        }

        /* Computes sine
         */
        inline Vector3 Sin(const Vector3& v)
        {
            return Vector3(Sin(v.x), Sin(v.y), Sin(v.z));
        }

        /* Computes tangent
         */
        inline Vector3 Tan(const Vector3& v)
        {
            return Vector3(Tan(v.x), Tan(v.y), Tan(v.z));
        }

        /* Computes hyperbolic cosine
         */
        inline Vector3 Cosh(const Vector3& v)
        {
            return Vector3(Cosh(v.x), Cosh(v.y), Cosh(v.z));
        }

        /* Computes hyperbolic sine
         */
        inline Vector3 Sinh(const Vector3& v)
        {
            return Vector3(Sinh(v.x), Sinh(v.y), Sinh(v.z));
        }

        /* Computes hyperbolic tangent
         */
        inline Vector3 Tanh(const Vector3& v)
        {
            return Vector3(Tanh(v.x), Tanh(v.y), Tanh(v.z));
        }

        /* Computes inverse cosine
         */
        inline Vector3 Acos(const Vector3& v)
        {
            return Vector3(Acos(v.x), Acos(v.y), Acos(v.z));
        }

        /* Computes inverse sine
         */
        inline Vector3 Asin(const Vector3& v)
        {
            return Vector3(Asin(v.x), Asin(v.y), Asin(v.z));
        }

        /* Computes inverse tangent
         */
        inline Vector3 Atan(const Vector3& v)
        {
            return Vector3(Atan(v.x),
                Atan(v.y),
                Asin(v.z));
        }

        /* Computes inverse tangent with 2 args
         */
        inline Vector3 Atan2(const Vector3& a, const Vector3& b)
        {
            return Vector3(Atan2(a.x, b.x),
                Atan2(a.y, b.y),
                Atan2(a.z, b.z));
        }

        /* Computes Euler number raised to the power 'x'
         */
        inline Vector3 Exp(const Vector3& v)
        {
            return Vector3(Exp(v.x),
                Exp(v.y),
                Exp(v.z));
        }

        /* Computes 2 raised to the power 'x'
         */
        inline Vector3 Exp2(const Vector3& v)
        {
            return Vector3(Exp2(v.x),
                Exp2(v.y),
                Exp2(v.z));
        }

        /* Computes the base Euler number logarithm
         */
        inline Vector3 Log(const Vector3& v)
        {
            return Vector3(Log(v.x),
                Log(v.y),
                Log(v.z));
        }

        /* Computes the base 2 logarithm
         */
        inline Vector3 Log2(const Vector3& v)
        {
            return Vector3(Log2(v.x),
                Log2(v.y),
                Log2(v.z));
        }

        /* Computes the base 10 logarithm
         */
        inline Vector3 Log10(const Vector3& v)
        {
            return Vector3(Log10(v.x),
                Log10(v.y),
                Log10(v.z));
        }

        /* Computes the value of base raised to the power exponent
         */
        inline Vector3 Pow(const Vector3& a, const Vector3& b)
        {
            return Vector3(Pow(a.x, b.x),
                Pow(a.y, b.y),
                Pow(a.z, b.z));
        }

        /* Get the fractal part of floating point
         */
        inline Vector3 Frac(const Vector3& v)
        {
            return Vector3(Frac(v.x),
                Frac(v.y),
                Frac(v.z));
        }

        /* Computes the floating-point remainder of the division operation x/y
         */
        inline Vector3 Mod(const Vector3& a, const Vector3& b)
        {
            return Vector3(Mod(a.x, b.x),
                Mod(a.y, b.y),
                Mod(a.z, b.z));
        }

        /* Computes the smallest integer value not less than 'x'
         */
        inline Vector3 Ceil(const Vector3& v)
        {
            return Vector3(Ceil(v.x),
                Ceil(v.y),
                Ceil(v.z));
        }

        /* Computes the largest integer value not greater than 'x'
         */
        inline Vector3 Floor(const Vector3& v)
        {
            return Vector3(Floor(v.x),
                Floor(v.y),
                Floor(v.z));
        }

        /* Computes the nearest integer value
         */
        inline Vector3 Round(const Vector3& v)
        {
            return Vector3(Round(v.x),
                Round(v.y),
                Round(v.z));
        }

        /* Computes the nearest integer not greater in magnitude than 'x'
         */
        inline Vector3 Trunc(const Vector3& v)
        {
            return Vector3(Trunc(v.x),
                Trunc(v.y),
                Trunc(v.z));
        }

        /* Get the smaller value
         */
        inline Vector3 Min(const Vector3& a, const Vector3& b)
        {
            return Vector3(Min(a.x, b.x),
                Min(a.y, b.y),
                Min(a.z, b.z));
        }

        /* Get the larger value
         */
        inline Vector3 Max(const Vector3& a, const Vector3& b)
        {
            return Vector3(Max(a.x, b.x),
                Max(a.y, b.y),
                Max(a.z, b.z));
        }

        /* Clamps the 'x' value to the [min, max].
         */
        inline Vector3 Clamp(const Vector3& v, const Vector3& min, const Vector3& max)
        {
            return Vector3(Clamp(v.x, min.x, max.x),
                Clamp(v.y, min.y, max.y),
                Clamp(v.z, min.z, max.z));
        }

        /* Clamps the specified value within the range of 0 to 1
         */
        inline Vector3 Saturate(const Vector3& v)
        {
            return Vector3(Saturate(v.x),
                Saturate(v.y),
                Saturate(v.z));
        }

        /* Compares two values, returning 0 or 1 based on which value is greater.
         */
        inline Vector3 Step(const Vector3& a, const Vector3& b)
        {
            return Vector3(
                Step(a.x, b.x),
                Step(a.y, b.y),
                Step(a.z, b.z)
            );
        }

        /* Performs a linear interpolation.
         */
        inline Vector3 Lerp(const Vector3& a, const Vector3& b, const Vector3& t)
        {
            return Vector3(Lerp(a.x, b.x, t.x),
                Lerp(a.y, b.y, t.y),
                Lerp(a.z, b.z, t.z));
        }

        /* Performs a linear interpolation.
         */
        inline Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
        {
            return Vector3(Lerp(a.x, b.x, t),
                Lerp(a.y, b.y, t),
                Lerp(a.z, b.z, t));
        }

        /* Compute a smooth Hermite interpolation
         */
        inline Vector3 Smoothstep(const Vector3& a, const Vector3& b, const Vector3& t)
        {
            return Vector3(Smoothstep(a.x, b.x, t.x),
                Smoothstep(a.y, b.y, t.y),
                Smoothstep(a.z, b.z, t.z));
        }

        /* Computes square root of 'x'.
         */
        inline Vector3 Sqrt(const Vector3& v)
        {
            return Vector3(Sqrt(v.x),
                Sqrt(v.y),
                Sqrt(v.z));
        }

        /* Computes inverse square root of 'x'.
         */
        inline Vector3 InvSqrt(const Vector3& v)
        {
            return Vector3(InvSqrt(v.x),
                InvSqrt(v.y),
                InvSqrt(v.z));
        }

        /* Computes fast square root of 'x'.
         */
        inline Vector3 FastSqrt(const Vector3& v)
        {
            return Vector3(FastSqrt(v.x),
                FastSqrt(v.y),
                FastSqrt(v.z));
        }

        /* Computes fast inverse square root of 'x'.
         */
        inline Vector3 FastInvSqrt(const Vector3& v)
        {
            return Vector3(FastInvSqrt(v.x),
                FastInvSqrt(v.y),
                FastInvSqrt(v.z));
        }

        //
        // @region: Graphics functions
        //

        /* Compute Cross product of two vectors
         */
        inline Vector3 Cross(const Vector3& a, const Vector3& b)
        {
            return Vector3(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }

        /* Compute Dot product of two vectors
         */
        inline float Dot(const Vector3& a, const Vector3& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        /* Compute squared Length of vector
         */
        inline float LengthSq(const Vector3& v)
        {
            return Dot(v, v);
        }

        /* Compute Length of vector
         */
        inline float Length(const Vector3& v)
        {
            return Sqrt(LengthSq(v));
        }

        /* Compute distance from 'a' to b
         */
        inline float Distance(const Vector3& a, const Vector3& b)
        {
            return Length(a - b);
        }

        /* Compute squared distance from 'a' to b
         */
        inline float DistanceSq(const Vector3& a, const Vector3& b)
        {
            return LengthSq(a - b);
        }

        /* Compute angle of vector, a.k.a direction of vector
         */
        inline float Angle(const Vector2& v)
        {
            return Atan2(v.y, v.x);
        }

        /* Compute angle of two vector
         */
        inline float Angle(const Vector2& a, const Vector2& b)
        {
            return Angle(b - a);
        }

        /* Compute normalized vector
         */
        inline Vector3 Normalize(const Vector3& v)
        {
            const float lsqr = LengthSq(v);
            if (lsqr > 0.0f)
            {
                const float f = InvSqrt(lsqr);
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
            return v - 2.0f * Dot(v, n) * n;
        }

        /* Compute refraction vector
         */
        inline Vector3 Refract(const Vector3& v, const Vector3& n, float eta)
        {
            const float k = 1.0f - eta * eta * (1.0f - Dot(v, n) * Dot(v, n));
            return k < 0.0f
                ? Vector3(0.0f)
                : eta * v - (eta * Dot(v, n) + Sqrt(k)) * n;
        }

        /* Compute faceforward vector
         */
        inline Vector3 Faceforward(const Vector3& n, const Vector3& i, const Vector3& nref)
        {
            return Dot(i, nref) < 0.0f ? n : -n;
        }

        /* Computes sign of 'x'
     */
        inline Vector4 Sign(const Vector4& v)
        {
            return Vector4(Sign(v.x),
                Sign(v.y),
                Sign(v.z),
                Sign(v.w));
        }

        /* Computes absolute value
         */
        inline Vector4 Abs(const Vector4& v)
        {
            return Vector4(Abs(v.x),
                Abs(v.y),
                Abs(v.z),
                Abs(v.w));
        }

        /* Computes cosine
         */
        inline Vector4 Cos(const Vector4& v)
        {
            return Vector4(Cos(v.x),
                Cos(v.y),
                Cos(v.z),
                Cos(v.w));
        }

        /* Computes sine
         */
        inline Vector4 Sin(const Vector4& v)
        {
            return Vector4(Sin(v.x),
                Sin(v.y),
                Sin(v.z),
                Sin(v.w));
        }

        /* Computes tangent
         */
        inline Vector4 Tan(const Vector4& v)
        {
            return Vector4(Tan(v.x),
                Tan(v.y),
                Tan(v.z),
                Tan(v.w));
        }

        /* Computes hyperbolic cosine
         */
        inline Vector4 Cosh(const Vector4& v)
        {
            return Vector4(Cosh(v.x),
                Cosh(v.y),
                Cosh(v.z),
                Cosh(v.w));
        }

        /* Computes hyperbolic sine
         */
        inline Vector4 Sinh(const Vector4& v)
        {
            return Vector4(Sinh(v.x),
                Sinh(v.y),
                Sinh(v.z),
                Sinh(v.w));
        }

        /* Computes hyperbolic tangent
         */
        inline Vector4 Tanh(const Vector4& v)
        {
            return Vector4(Tanh(v.x),
                Tanh(v.y),
                Tanh(v.z),
                Tanh(v.w));
        }

        /* Computes inverse cosine
         */
        inline Vector4 Acos(const Vector4& v)
        {
            return Vector4(Acos(v.x),
                Acos(v.y),
                Acos(v.z),
                Acos(v.w));
        }

        /* Computes inverse sine
         */
        inline Vector4 Asin(const Vector4& v)
        {
            return Vector4(Asin(v.x),
                Asin(v.y),
                Asin(v.z),
                Asin(v.w));
        }

        /* Computes inverse tangent
         */
        inline Vector4 Atan(const Vector4& v)
        {
            return Vector4(Atan(v.x),
                Atan(v.y),
                Atan(v.z),
                Atan(v.w));
        }

        /* Computes inverse tangent with 2 args
         */
        inline Vector4 Atan2(const Vector4& a, const Vector4& b)
        {
            return Vector4(Atan2(a.x, b.x), Atan2(a.y, b.y), Atan2(a.z, b.z), Atan2(a.w, b.w));
        }

        /* Computes Euler number raised to the power 'x'
         */
        inline Vector4 Exp(const Vector4& v)
        {
            return Vector4(Exp(v.x), Exp(v.y), Exp(v.z), Exp(v.w));
        }

        /* Computes 2 raised to the power 'x'
         */
        inline Vector4 Exp2(const Vector4& v)
        {
            return Vector4(Exp2(v.x), Exp2(v.y), Exp2(v.z), Exp2(v.w));
        }

        /* Computes the base Euler number logarithm
         */
        inline Vector4 Log(const Vector4& v)
        {
            return Vector4(Log(v.x), Log(v.y), Log(v.z), Log(v.w));
        }

        /* Computes the base 2 logarithm
         */
        inline Vector4 Log2(const Vector4& v)
        {
            return Vector4(Log2(v.x), Log2(v.y), Log2(v.z), Log2(v.w));
        }

        /* Computes the base 10 logarithm
         */
        inline Vector4 Log10(const Vector4& v)
        {
            return Vector4(Log10(v.x), Log10(v.y), Log10(v.z), Log10(v.w));
        }

        /* Computes the value of base raised to the power exponent
         */
        inline Vector4 Pow(const Vector4& a, const Vector4& b)
        {
            return Vector4(
                Pow(a.x, b.x),
                Pow(a.y, b.y),
                Pow(a.z, b.z),
                Pow(a.w, b.w));
        }

        /* Get the fractal part of floating point
         */
        inline Vector4 Frac(const Vector4& v)
        {
            return Vector4(
                Frac(v.x),
                Frac(v.y),
                Frac(v.z),
                Frac(v.w));
        }

        /* Computes the floating-point remainder of the division operation x/y
         */
        inline Vector4 Mod(const Vector4& a, const Vector4& b)
        {
            return Vector4(
                Mod(a.x, b.x),
                Mod(a.y, b.y),
                Mod(a.z, b.z),
                Mod(a.w, b.w));
        }

        /* Computes the smallest integer value not less than 'x'
         */
        inline Vector4 Ceil(const Vector4& v)
        {
            return Vector4(
                Ceil(v.x),
                Ceil(v.y),
                Ceil(v.z),
                Ceil(v.w));
        }

        /* Computes the largest integer value not greater than 'x'
         */
        inline Vector4 Floor(const Vector4& v)
        {
            return Vector4(
                Floor(v.x),
                Floor(v.y),
                Floor(v.z),
                Floor(v.w));
        }

        /* Computes the nearest integer value
         */
        inline Vector4 Round(const Vector4& v)
        {
            return Vector4(
                Round(v.x),
                Round(v.y),
                Round(v.z),
                Round(v.w));
        }

        /* Computes the nearest integer not greater in magnitude than 'x'
         */
        inline Vector4 Trunc(const Vector4& v)
        {
            return Vector4(
                Trunc(v.x),
                Trunc(v.y),
                Trunc(v.z),
                Trunc(v.w));
        }

        /* Get the smaller value
         */
        inline Vector4 Min(const Vector4& a, const Vector4& b)
        {
            return Vector4(
                Min(a.x, b.x),
                Min(a.y, b.y),
                Min(a.z, b.z),
                Min(a.w, b.w));
        }

        /* Get the larger value
         */
        inline Vector4 Max(const Vector4& a, const Vector4& b)
        {
            return Vector4(
                Max(a.x, b.x),
                Max(a.y, b.y),
                Max(a.z, b.z),
                Max(a.w, b.w));
        }

        /* Clamps the 'x' value to the [min, max].
         */
        inline Vector4 Clamp(const Vector4& v, const Vector4& min, const Vector4& max)
        {
            return Vector4(
                Clamp(v.x, min.x, max.x),
                Clamp(v.y, min.y, max.y),
                Clamp(v.z, min.z, max.z),
                Clamp(v.w, min.w, max.w));
        }

        /* Clamps the specified value within the range of 0 to 1
         */
        inline Vector4 Saturate(const Vector4& v)
        {
            return Vector4(
                Saturate(v.x),
                Saturate(v.y),
                Saturate(v.z),
                Saturate(v.w));
        }

        /* Compares two values, returning 0 or 1 based on which value is greater.
         */
        inline Vector4 Step(const Vector4& a, const Vector4& b)
        {
            return Vector4(
                Step(a.x, b.x),
                Step(a.y, b.y),
                Step(a.z, b.z),
                Step(a.w, b.w));
        }

        /* Performs a linear interpolation.
         */
        inline Vector4 Lerp(const Vector4& a, const Vector4& b, const Vector4& t)
        {
            return Vector4(
                Lerp(a.x, b.x, t.x),
                Lerp(a.y, b.y, t.y),
                Lerp(a.z, b.z, t.z),
                Lerp(a.w, b.w, t.w));
        }

        /* Performs a linear interpolation.
         */
        inline Vector4 Lerp(const Vector4& a, const Vector4& b, float t)
        {
            return Vector4(
                Lerp(a.x, b.x, t),
                Lerp(a.y, b.y, t),
                Lerp(a.z, b.z, t),
                Lerp(a.w, b.w, t));
        }

        /* Compute a smooth Hermite interpolation
         */
        inline Vector4 Smoothstep(const Vector4& a, const Vector4& b, const Vector4& t)
        {
            return Vector4(
                Smoothstep(a.x, b.x, t.x),
                Smoothstep(a.y, b.y, t.y),
                Smoothstep(a.z, b.z, t.z),
                Smoothstep(a.w, b.w, t.w));
        }

        /* Computes square root of 'x'.
         */
        inline Vector4 Sqrt(const Vector4& v)
        {
            return Vector4(Sqrt(v.x), Sqrt(v.y), Sqrt(v.z), Sqrt(v.w));
        }

        /* Computes inverse square root of 'x'.
         */
        inline Vector4 InvSqrt(const Vector4& v)
        {
            return Vector4(InvSqrt(v.x), InvSqrt(v.y), InvSqrt(v.z), InvSqrt(v.w));
        }

        /* Computes fast square root of 'x'.
         */
        inline Vector4 FastSqrt(const Vector4& v)
        {
            return Vector4(FastSqrt(v.x), FastSqrt(v.y), FastSqrt(v.z), FastSqrt(v.w));
        }

        /* Computes fast inverse square root of 'x'.
         */
        inline Vector4 FastInvSqrt(const Vector4& v)
        {
            return Vector4(FastInvSqrt(v.x), FastInvSqrt(v.y), FastInvSqrt(v.z), FastInvSqrt(v.w));
        }

        //
        // @region: Graphics functions
        //

        /* Compute Dot product of two vectors
         */
        inline float Dot(const Vector4& a, const Vector4& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }

        /* Compute squared Length of vector
         */
        inline float LengthSq(const Vector4& v)
        {
            return Dot(v, v);
        }

        /* Compute Length of vector
         */
        inline float Length(const Vector4& v)
        {
            return Sqrt(LengthSq(v));
        }

        /* Compute distance from 'a' to b
         */
        inline float Distance(const Vector4& a, const Vector4& b)
        {
            return Length(a - b);
        }

        /* Compute squared distance from 'a' to b
         */
        inline float DistanceSq(const Vector4& a, const Vector4& b)
        {
            return LengthSq(a - b);
        }

        /* Compute normalized vector
         */
        inline Vector4 Normalize(const Vector4& v)
        {
            const float lsqr = LengthSq(v);
            if (lsqr > 0.0f)
            {
                const float f = InvSqrt(lsqr);
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
            return v - 2.0f * Dot(v, n) * n;
        }

        /* Compute refraction vector
         */
        inline Vector4 Refract(const Vector4& v, const Vector4& n, float eta)
        {
            const float k = 1.0f - eta * eta * (1.0f - Dot(v, n) * Dot(v, n));
            return k < 0.0f
                ? Vector4(0.0f)
                : eta * v - (eta * Dot(v, n) + Sqrt(k)) * n;
        }

        /* Compute faceforward vector
         */
        inline Vector4 Faceforward(const Vector4& n, const Vector4& i, const Vector4& nref)
        {
            return Dot(i, nref) < 0.0f ? n : -n;
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

            Vector3 v = a3 * b.w + b3 * a.w + Cross(a3, b3);
            float  w = a.w * b.w - Dot(a3, b3);
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
                Dot(c0, b),
                Dot(c1, b),
                Dot(c2, b),
                Dot(c3, b)
            );
        }

        inline Vector4 Mul(const Vector4& a, const Matrix4& b)
        {
            return Vector4(
                Dot(a, b[0]),
                Dot(a, b[1]),
                Dot(a, b[2]),
                Dot(a, b[3])
            );
        }

        inline Vector3 Mul(const Matrix4& a, const Vector3& b)
        {
            const Vector4 b0 = Vector4(b.x, b.y, b.z, 1.0f);
            const Vector4 b1 = Mul(a, b0);

            const float iw = 1.0f / b1.w;
            return Vector3(b1.x * iw, b1.y * iw, b1.z * iw);
        }

        inline Vector3 Mul(const Vector3& a, const Matrix4& b)
        {
            const Vector4 a0 = Vector4(a.x, a.y, a.z, 1.0f);
            const Vector4 a1 = Mul(a0, b);

            const float iw = 1.0f / a1.w;
            return Vector3(a1.x * iw, a1.y * iw, a1.z * iw);
        }

        inline Matrix4 Mul(const Matrix4& a, const Matrix4& b)
        {
            Matrix4 result;
            result[0] = Mul(a, b[0]);
            result[1] = Mul(a, b[1]);
            result[2] = Mul(a, b[2]);
            result[3] = Mul(a, b[3]);
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
                c = Normalize(c);
            }

            Vector3 axis;
            const float den = Sqrt(1.0f - c.w * c.w);
            if (den > 0.0001f)
            {
                axis = Vector3(c.x, c.y, c.z) / den;
            }
            else
            {
                axis = Vector3(1, 0, 0);
            }

            float angle = 2.0f * Cos(c.w);
            return Vector4(axis, angle);
        }

        inline Matrix4 Transform(const Vector2& position, float rotation, const Vector2& scale)
        {
            return Mul(Mul(Math::Translation(position), Math::RotationZ(rotation)), Math::Scalation(scale));
        }

        inline Matrix4 Transform(const Vector3& position, const Quaterion& rotation, const Vector3& scale)
        {
            return Mul(Mul(Math::Translation(position), Math::Rotation(rotation)), Math::Scalation(scale));
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
            const float a = 1.0f / Tan(fov * 0.5f);
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
            const Vector3 z = Normalize(eye - target);
            const Vector3 x = Normalize(Cross(up, z));
            const Vector3 y = Normalize(Cross(z, x));

            Matrix4 result;
            result[0] = Vector4(x.x, y.x, z.x, 0);
            result[1] = Vector4(x.y, y.y, z.y, 0);
            result[2] = Vector4(x.z, y.z, z.z, 0);
            result[3] = Vector4(-Dot(x, eye), -Dot(y, eye), -Dot(z, eye), 1.0f);
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
            const float c = Cos(-angle);
            const float s = Sin(-angle);
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
            const float s = Sin(angle);
            const float c = Cos(angle);

            return Matrix4(
                1, 0, 0, 0,
                0, c, s, 0,
                0, -s, c, 0,
                0, 0, 0, 1
            );
        }

        inline Matrix4 RotationY(float angle)
        {
            const float s = Sin(angle);
            const float c = Cos(angle);

            return Matrix4(
                c, 0, s, 0,
                0, 1, 0, 0,
                -s, 0, c, 0,
                0, 0, 0, 1
            );
        }

        inline Matrix4 RotationZ(float angle)
        {
            const float s = Sin(angle);
            const float c = Cos(angle);

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

            float scale_x = Length(xaxis);
            float scale_y = Length(yaxis);
            float scale_z = Length(zaxis);

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
                float s = 0.5f / Sqrt(trace);
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
                    float s = 0.5f / Sqrt(1.0f + xaxis.x - yaxis.y - zaxis.z);
                    quaternion->w = (yaxis.z - zaxis.y) * s;
                    quaternion->x = 0.25f / s;
                    quaternion->y = (yaxis.x + xaxis.y) * s;
                    quaternion->z = (zaxis.x + xaxis.z) * s;
                }
                else if (yaxis.y > zaxis.z)
                {
                    float s = 0.5f / Sqrt(1.0f + yaxis.y - xaxis.x - zaxis.z);
                    quaternion->w = (zaxis.x - xaxis.z) * s;
                    quaternion->x = (yaxis.x + xaxis.y) * s;
                    quaternion->y = 0.25f / s;
                    quaternion->z = (zaxis.y + yaxis.z) * s;
                }
                else
                {
                    float s = 0.5f / Sqrt(1.0f + zaxis.z - xaxis.x - yaxis.y);
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
#endif
}