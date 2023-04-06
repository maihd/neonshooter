using System;

namespace Mojo
{
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
        Matrix4 Scalation(Vector2 v);
        Matrix4 Scalation(Vector3 v);
        Matrix4 Scalation(float x, float y, float z = 1.0f);

        Matrix4 Translation(Vector2 v);
        Matrix4 Translation(Vector3 v);
        Matrix4 Translation(float x, float y, float z = 0.0f);

        Matrix4 Rotation(Quaterion quaternion);
        Matrix4 Rotation(Vector3 axis, float angle);
        Matrix4 Rotation(float x, float y, float z, float angle);

        Matrix4 RotationX(float angle);
        Matrix4 RotationY(float angle);
        Matrix4 RotationZ(float angle);

        Matrix4 Lookat(Vector3 eye, Vector3 target, Vector3 up);

        Matrix4 Ortho(float l, float r, float b, float t, float n, float f);
        Matrix4 Frustum(float l, float r, float b, float t, float n, float f);
        Matrix4 Perspective(float fov, float aspect, float znear, float zfar);

        void Decompose(Matrix4 m, Vector3* scalation, Quaterion* quaternion, Vector3* translation);
        void Decompose(Matrix4 m, Vector3* scalation, Vector3* axis, float* angle, Vector3* translation);
    }

    //
    // @region: Operators
    //

    inline bool operator==(Matrix4 a, Matrix4 b)
    {
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
    }

    inline bool operator!=(Matrix4 a, Matrix4 b)
    {
        return a[0] == b[0] || a[1] == b[1] || a[2] == b[2] || a[3] == b[3];
    }

    inline Matrix4 operator-(Matrix4 m)
    {
        Matrix4 result;
        result[0] = -m[0];
        result[1] = -m[1];
        result[2] = -m[2];
        result[3] = -m[3];
        return result;
    }

    inline Matrix4 operator+(Matrix4 m)
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

    inline Matrix4 operator--(Matrix4& m, int)
    {
        m[0]--;
        m[1]--;
        m[2]--;
        m[3]--;
        return m;
    }

    inline Matrix4 operator++(Matrix4& m, int)
    {
        m[0]++;
        m[1]++;
        m[2]++;
        m[3]++;
        return m;
    }

    inline Matrix4 operator+(Matrix4 a, Matrix4 b)
    {
        Matrix4 result;
        result[0] = a[0] + b[0];
        result[1] = a[1] + b[1];
        result[2] = a[2] + b[2];
        result[3] = a[3] + b[3];
        return result;
    }

    inline Matrix4 operator+(Matrix4 a, float b)
    {
        Matrix4 result;
        result[0] = a[0] + b;
        result[1] = a[1] + b;
        result[2] = a[2] + b;
        result[3] = a[3] + b;
        return result;
    }

    inline Matrix4 operator+(float a, Matrix4 b)
    {
        Matrix4 result;
        result[0] = a + b[0];
        result[1] = a + b[1];
        result[2] = a + b[2];
        result[3] = a + b[3];
        return result;
    }

    inline Matrix4 operator-(Matrix4 a, Matrix4 b)
    {
        Matrix4 result;
        result[0] = a[0] - b[0];
        result[1] = a[1] - b[1];
        result[2] = a[2] - b[2];
        result[3] = a[3] - b[3];
        return result;
    }

    inline Matrix4 operator-(Matrix4 a, float b)
    {
        Matrix4 result;
        result[0] = a[0] - b;
        result[1] = a[1] - b;
        result[2] = a[2] - b;
        result[3] = a[3] - b;
        return result;
    }

    inline Matrix4 operator-(float a, Matrix4 b)
    {
        Matrix4 result;
        result[0] = a - b[0];
        result[1] = a - b[1];
        result[2] = a - b[2];
        result[3] = a - b[3];
        return result;
    }

    inline Matrix4 operator*(Matrix4 a, float b)
    {
        Matrix4 result;
        result[0] = a[0] * b;
        result[1] = a[1] * b;
        result[2] = a[2] * b;
        result[3] = a[3] * b;
        return result;
    }

    inline Matrix4 operator*(float a, Matrix4 b)
    {
        Matrix4 result;
        result[0] = a * b[0];
        result[1] = a * b[1];
        result[2] = a * b[2];
        result[3] = a * b[3];
        return result;
    }

    inline Matrix4 operator*(Matrix4 a, Matrix4 b)
    {
        Matrix4 result;
        result[0] = a[0] * b[0];
        result[1] = a[1] * b[1];
        result[2] = a[2] * b[2];
        result[3] = a[3] * b[3];
        return result;
    }

    inline Matrix4 operator/(Matrix4 a, Matrix4 b)
    {
        Matrix4 result;
        result[0] = a[0] / b[0];
        result[1] = a[1] / b[1];
        result[2] = a[2] / b[2];
        result[3] = a[3] / b[3];
        return result;
    }

    inline Matrix4 operator/(Matrix4 a, float b)
    {
        Matrix4 result;
        result[0] = a[0] / b;
        result[1] = a[1] / b;
        result[2] = a[2] / b;
        result[3] = a[3] / b;
        return result;
    }

    inline Matrix4 operator/(float a, Matrix4 b)
    {
        Matrix4 result;
        result[0] = a / b[0];
        result[1] = a / b[1];
        result[2] = a / b[2];
        result[3] = a / b[3];
        return result;
    }

    inline Matrix4& operator+=(Matrix4& a, Matrix4 b)
    {
        return (a = a + b);
    }

    inline Matrix4& operator+=(Matrix4& a, float b)
    {
        return (a = a + b);
    }

    inline Matrix4& operator-=(Matrix4& a, Matrix4 b)
    {
        return (a = a - b);
    }

    inline Matrix4& operator-=(Matrix4& a, float b)
    {
        return (a = a - b);
    }

    inline Matrix4& operator*=(Matrix4& a, Matrix4 b)
    {
        return (a = a * b);
    }

    inline Matrix4& operator*=(Matrix4& a, float b)
    {
        return (a = a * b);
    }

    inline Matrix4& operator/=(Matrix4& a, Matrix4 b)
    {
        return (a = a / b);
    }

    inline Matrix4& operator/=(Matrix4& a, float b)
    {
        return (a = a + b);
    }

    namespace Math
    {
        //
        // @region: Quaternion
        //

        

        //
        // @region: Quaternion
        //

        inline Vector4 Mul(Matrix4 a, Vector4 b)
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

        inline Vector4 Mul(Vector4 a, Matrix4 b)
        {
            return Vector4(
                Dot(a, b[0]),
                Dot(a, b[1]),
                Dot(a, b[2]),
                Dot(a, b[3])
            );
        }

        inline Vector3 Mul(Matrix4 a, Vector3 b)
        {
            const Vector4 b0 = Vector4(b.x, b.y, b.z, 1.0f);
            const Vector4 b1 = Mul(a, b0);

            const float iw = 1.0f / b1.w;
            return Vector3(b1.x * iw, b1.y * iw, b1.z * iw);
        }

        inline Vector3 Mul(Vector3 a, Matrix4 b)
        {
            const Vector4 a0 = Vector4(a.x, a.y, a.z, 1.0f);
            const Vector4 a1 = Mul(a0, b);

            const float iw = 1.0f / a1.w;
            return Vector3(a1.x * iw, a1.y * iw, a1.z * iw);
        }

        inline Matrix4 Mul(Matrix4 a, Matrix4 b)
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

        inline Vector4 ToAxisAngle(Quaterion q)
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

        inline Matrix4 Transform(Vector2 position, float rotation, Vector2 scale)
        {
            return Mul(Mul(Math::Translation(position), Math::RotationZ(rotation)), Math::Scalation(scale));
        }

        inline Matrix4 Transform(Vector3 position, Quaterion rotation, Vector3 scale)
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

        inline Matrix4 Lookat(Vector3 eye, Vector3 target, Vector3 up)
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

        inline Matrix4 Scalation(Vector2 v)
        {
            return Scalation(v.x, v.y);
        }

        inline Matrix4 Scalation(Vector3 v)
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

        inline Matrix4 Translation(Vector2 v)
        {
            return Translation(v.x, v.y);
        }

        inline Matrix4 Translation(Vector3 v)
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

        inline Matrix4 Rotation(Vector3 axis, float angle)
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

        inline Matrix4 Rotation(Quaterion quaternion)
        {
            Vector4 axisangle = ToAxisAngle(quaternion);
            return Rotation(axisangle.x, axisangle.y, axisangle.z, axisangle.w);
        }

        inline void Decompose(Matrix4 m, Vector3* scalation, Quaterion* quaternion, Vector3* translation)
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

        inline void Decompose(Matrix4 m, Vector3* scalation, Vector3* axis, float* angle, Vector3* translation)
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