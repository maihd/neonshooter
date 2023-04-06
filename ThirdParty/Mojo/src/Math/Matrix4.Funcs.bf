namespace System;

using Mojo;

extension Math
{
    [Inline]
 	public static Vector4 Mul(Matrix4 a, Vector4 b)
    {
        let c0 = Vector4(a[0][0], a[1][0], a[2][0], a[3][0]);
        let c1 = Vector4(a[0][1], a[1][1], a[2][1], a[3][1]);
        let c2 = Vector4(a[0][2], a[1][2], a[2][2], a[3][2]);
        let c3 = Vector4(a[0][3], a[1][3], a[2][3], a[3][3]);

        return Vector4(
            Dot(c0, b),
            Dot(c1, b),
            Dot(c2, b),
            Dot(c3, b)
        );
    }

    [Inline]
 	public static Vector4 Mul(Vector4 a, Matrix4 b)
    {
        return Vector4(
            Dot(a, b[0]),
            Dot(a, b[1]),
            Dot(a, b[2]),
            Dot(a, b[3])
        );
    }

    [Inline]
 	public static Vector3 Mul(Matrix4 a, Vector3 b)
    {
        let b0 = Vector4(b.x, b.y, b.z, 1.0f);
        let b1 = Mul(a, b0);
        let iw = 1.0f / b1.w;
        return Vector3(b1.x * iw, b1.y * iw, b1.z * iw);
    }

    [Inline]
 	public static Vector3 Mul(Vector3 a, Matrix4 b)
    {
        let a0 = Vector4(a.x, a.y, a.z, 1.0f);
        let a1 = Mul(a0, b);
        let iw = 1.0f / a1.w;
        return Vector3(a1.x * iw, a1.y * iw, a1.z * iw);
    }

    [Inline]
 	public static Matrix4 Mul(Matrix4 a, Matrix4 b)
    {
        return .(
			Mul(a, b[0]),
			Mul(a, b[1]),
			Mul(a, b[2]),
			Mul(a, b[3])
		);
    }

    //-------------------------------------------
    // Graphics math
    //-------------------------------------------

    [Inline]
 	public static Vector4 ToAxisAngle(Quaterion q)
    {
        Vector4 c = Vector4(q);
        if (c.w != 0.0f)
        {
            c = Normalize(c);
        }

        let den = Sqrt(1.0f - c.w * c.w);
        let axis = (den > 0.0001f)
			? Vector3(c.x, c.y, c.z) / den
			: Vector3(1, 0, 0);

        float angle = 2.0f * Cos(c.w);
        return Vector4(axis, angle);
    }

    [Inline]
 	public static Matrix4 Transform(Vector2 position, float rotation, Vector2 scale)
    {
        return Mul(Mul(Translation(position), RotationZ(rotation)), Scalation(scale));
    }

    [Inline]
 	public static Matrix4 Transform(Vector3 position, Quaterion rotation, Vector3 scale)
    {
        return Mul(Mul(Translation(position), Rotation(rotation)), Scalation(scale));
    }

    [Inline]
 	public static Matrix4 Ortho(float l, float r, float b, float t, float n, float f)
    {
        let x = 1.0f / (r - l);
        let y = 1.0f / (t - b);
        let z = 1.0f / (f - n);

		return .(
        	Vector4(2.0f * x, 0, 0, 0),
        	Vector4(0, 2.0f * y, 0, 0),
        	Vector4(0, 0, 2.0f * z, 0),
        	Vector4(-x * (l + r), -y * (b + t), -z * (n + f), 1.0f)
		);
    }

    [Inline]
 	public static Matrix4 Frustum(float l, float r, float b, float t, float n, float f)
    {
        let x = 1.0f / (r - l);
        let y = 1.0f / (t - b);
        let z = 1.0f / (f - n);

        return .(
	        Vector4(2.0f * x, 0, 0, 0),
	        Vector4(0, 2.0f * y, 0, 0),
	        Vector4(x * (l + r), y * (b + t), z * (n + f), 1.0f),
	        Vector4(0, 0, 2.0f * z, 0)
        );
    }

    [Inline]
 	public static Matrix4 Perspective(float fov, float aspect, float znear, float zfar)
    {
        let a = 1.0f / Tan(fov * 0.5f);
        let b = zfar / (znear - zfar);

        return .(
	        Vector4(a / aspect, 0, 0, 0),
	        Vector4(0, a, 0, 0),
	        Vector4(0, 0, b, -1),
	        Vector4(0, 0, znear * b, 0)
		);
    }

    [Inline]
 	public static Matrix4 Lookat(Vector3 eye, Vector3 target, Vector3 up)
    {
        let z = Normalize(eye - target);
        let x = Normalize(Cross(up, z));
        let y = Normalize(Cross(z, x));
		
		return .(
	        Vector4(x.x, y.x, z.x, 0),
	        Vector4(x.y, y.y, z.y, 0),
	        Vector4(x.z, y.z, z.z, 0),
	        Vector4(-Dot(x, eye), -Dot(y, eye), -Dot(z, eye), 1.0f)
		);
    }

    [Inline]
 	public static Matrix4 Scalation(float s)
    {
        return Scalation(s, s, s);
    }

    [Inline]
 	public static Matrix4 Scalation(Vector2 v)
    {
        return Scalation(v.x, v.y);
    }

    [Inline]
 	public static Matrix4 Scalation(Vector3 v)
    {
        return Scalation(v.x, v.y, v.z);
    }

    [Inline]
 	public static Matrix4 Scalation(float x, float y, float z = 1.0f)
    {
        return Matrix4(
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1
        );
    }

    [Inline]
 	public static Matrix4 Translation(Vector2 v)
    {
        return Translation(v.x, v.y);
    }

    [Inline]
 	public static Matrix4 Translation(Vector3 v)
    {
        return Translation(v.x, v.y, v.z);
    }

    [Inline]
 	public static Matrix4 Translation(float x, float y, float z = 0.0f)
    {
        return Matrix4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            x, y, z, 1
        );
    }

    [Inline]
 	public static Matrix4 Rotation(Vector3 axis, float angle)
    {
        return Rotation(axis.x, axis.y, axis.z, angle);
    }

    [Inline]
 	public static Matrix4 Rotation(float x, float y, float z, float angle)
    {
        let c = Cos(-angle);
        let s = Sin(-angle);
        let t = 1.0f - c;

        Vector4[4] rows;
        /* Row 1 */
        rows[0] = Vector4(t * x * x + c,
            t * x * y - s * z,
            t * x * z + s * y,
            0.0f);

        /* Row 2 */
        rows[1] = Vector4(t * x * y + s * z,
            t * y * y + c,
            t * y * z - s * x,
            0.0f);

        /* Row 3 */
        rows[2] = Vector4(t * x * z - s * y,
            t * y * z + s * x,
            t * z * z + c,
            0.0f);

        /* Row 4 */
        rows[3] = Vector4(0, 0, 0, 1.0f);
        return .(rows);
    }

    [Inline]
 	public static Matrix4 RotationX(float angle)
    {
        let s = Sin(angle);
        let c = Cos(angle);

        return Matrix4(
            1,  0, 0, 0,
            0,  c, s, 0,
            0, -s, c, 0,
            0,  0, 0, 1
        );
    }

    [Inline]
 	public static Matrix4 RotationY(float angle)
    {
        let s = Sin(angle);
        let c = Cos(angle);

        return Matrix4(
             c, 0, s, 0,
             0, 1, 0, 0,
            -s, 0, c, 0,
             0, 0, 0, 1
        );
    }

    [Inline]
 	public static Matrix4 RotationZ(float angle)
    {
        let s = Sin(angle);
        let c = Cos(angle);

        return Matrix4(
             c, s, 0, 0,
            -s, c, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1
        );
    }

    [Inline]
 	public static Matrix4 Rotation(Quaterion quaternion)
    {
        Vector4 axisangle = ToAxisAngle(quaternion);
        return Rotation(axisangle.x, axisangle.y, axisangle.z, axisangle.w);
    }

    [Inline]
 	public static void Decompose(Matrix4 m, Vector3* scalation, Quaterion* quaternion, Vector3* translation)
    {
        if (translation != null)
        {
            *translation = Vector3(m[3][0], m[3][1], m[3][2]);
        }

        if (scalation == null && quaternion == null)
        {
            return;
        }

        var xaxis = Vector3(m[0][0], m[0][1], m[0][2]);
        var yaxis = Vector3(m[1][0], m[1][1], m[1][2]);
        var zaxis = Vector3(m[2][0], m[2][1], m[2][2]);

        var scale_x = Length(xaxis);
        var scale_y = Length(yaxis);
        var scale_z = Length(zaxis);

        let n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
        let n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
        let n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
        let n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

        let t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
        let t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
        let t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
        let t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

        let det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
        if (det < 0) scale_z = -scale_z;

        if (scalation != null)
        {
            *scalation = Vector3(scale_x, scale_y, scale_z);
        }

        if (quaternion != null)
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
            quaternion.w = 0.25f / s;
            quaternion.x = (yaxis.z - zaxis.y) * s;
            quaternion.y = (zaxis.x - xaxis.z) * s;
            quaternion.z = (xaxis.y - yaxis.x) * s;
        }
        else
        {
            // Note: since xaxis, yaxis, and zaxis are normalized, 
            // we will never divide by zero in the code below.
            if (xaxis.x > yaxis.y && xaxis.x > zaxis.z)
            {
                float s = 0.5f / Sqrt(1.0f + xaxis.x - yaxis.y - zaxis.z);
                quaternion.w = (yaxis.z - zaxis.y) * s;
                quaternion.x = 0.25f / s;
                quaternion.y = (yaxis.x + xaxis.y) * s;
                quaternion.z = (zaxis.x + xaxis.z) * s;
            }
            else if (yaxis.y > zaxis.z)
            {
                float s = 0.5f / Sqrt(1.0f + yaxis.y - xaxis.x - zaxis.z);
                quaternion.w = (zaxis.x - xaxis.z) * s;
                quaternion.x = (yaxis.x + xaxis.y) * s;
                quaternion.y = 0.25f / s;
                quaternion.z = (zaxis.y + yaxis.z) * s;
            }
            else
            {
                float s = 0.5f / Sqrt(1.0f + zaxis.z - xaxis.x - yaxis.y);
                quaternion.w = (xaxis.y - yaxis.x) * s;
                quaternion.x = (zaxis.x + xaxis.z) * s;
                quaternion.y = (zaxis.y + yaxis.z) * s;
                quaternion.z = 0.25f / s;
            }
        }
    }

    [Inline]
 	public static void Decompose(Matrix4 m, Vector3* scalation, Vector3* axis, float* angle, Vector3* translation)
    {
        if (axis != null || angle != null)
        {
            Quaterion quaterion = default;
            Decompose(m, scalation, &quaterion, translation);

            Vector4 axisangle = ToAxisAngle(quaterion);
            *axis = Vector3(axisangle);
            *angle = axisangle.w;
        }
        else
        {
            Decompose(m, scalation, null, translation);
        }
    }
}