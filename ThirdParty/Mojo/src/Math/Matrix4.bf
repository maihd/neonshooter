namespace Mojo;

using System;

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
    public this(Vector4[4] rows)
    {
        this.rows[0] = rows[0];
        this.rows[1] = rows[1];
        this.rows[2] = rows[2];
        this.rows[3] = rows[3];
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

	public Vector4 this[int index]
	{
	    get { return rows[index];			}
	}

    public ref Vector4 this[int index]
    {
        get mut { return ref rows[index];	}
        set mut { rows[index] = value; 		}
    }

    [Inline]
 	public static bool operator==(Matrix4 a, Matrix4 b)
    {
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
    }

    [Inline]
 	public static bool operator!=(Matrix4 a, Matrix4 b)
    {
        return a[0] == b[0] || a[1] == b[1] || a[2] == b[2] || a[3] == b[3];
    }

    [Inline]
 	public static Matrix4 operator-(Matrix4 m)
    {
        return .(
			-m[0],
			-m[1],
			-m[2],
			-m[3]
		);
    }

    [Inline]
 	public static Matrix4 operator+(Matrix4 m)
    {
        return m;
    }

    [Inline]
 	public void operator--()
    {
        --this[0];
        --this[1];
        --this[2];
        --this[3];
    }

    [Inline]
 	public void operator++()
    {
        ++this[0];
        ++this[1];
        ++this[2];
        ++this[3];
    }

    [Inline]
 	public static Matrix4 operator+(Matrix4 a, Matrix4 b)
    {
        return .(
			a[0] + b[0],
			a[1] + b[1],
			a[2] + b[2],
			a[3] + b[3]
		);
    }

    [Inline]
 	public static Matrix4 operator+(Matrix4 a, float b)
    {
        return .(
			a[0] + b,
			a[1] + b,
			a[2] + b,
			a[3] + b
		);
    }

    [Inline]
 	public static Matrix4 operator+(float a, Matrix4 b)
    {
        return .(
			a + b[0],
			a + b[1],
			a + b[2],
			a + b[3]
		);
    }

    [Inline]
 	public static Matrix4 operator-(Matrix4 a, Matrix4 b)
    {
        return .(
			a[0] - b[0],
			a[1] - b[1],
			a[2] - b[2],
			a[3] - b[3]
		);
    }

    [Inline]
 	public static Matrix4 operator-(Matrix4 a, float b)
    {
       	return .(
			a[0] - b,
			a[1] - b,
			a[2] - b,
			a[3] - b
		);
    }

    [Inline]
 	public static Matrix4 operator-(float a, Matrix4 b)
    {
        return .(
			a - b[0],
			a - b[1],
			a - b[2],
			a - b[3]
		);
    }

    [Inline]
 	public static Matrix4 operator*(Matrix4 a, float b)
    {
        return .(
			a[0] * b,
			a[1] * b,
			a[2] * b,
			a[3] * b
		);
    }

    [Inline]
 	public static Matrix4 operator*(float a, Matrix4 b)
    {
        return .(
			a * b[0],
			a * b[1],
			a * b[2],
			a * b[3]
		);
    }

    [Inline]
 	public static Matrix4 operator/(Matrix4 a, float b)
    {
        return .(
			a[0] / b,
			a[1] / b,
			a[2] / b,
			a[3] / b
		);
    }

    [Inline]
 	public static Matrix4 operator/(float a, Matrix4 b)
    {
        return .(
			a / b[0],
			a / b[1],
			a / b[2],
			a / b[3]
		);
    }

    [Inline]
 	public void operator+=(Matrix4 b) mut
    {
        this = this + b;
    }

    [Inline]
 	public void operator+=(float b) mut
    {
		this = this + b;
    }

    [Inline]
 	public void operator-=(Matrix4 b) mut
    {
		this = this - b;
    }

    [Inline]
 	public void operator-=(float b) mut
    {
		this = this - b;
    }

    [Inline]
 	public void operator*=(float b) mut
    {
		this = this * b;
    }

    [Inline]
 	public void operator/=(float b) mut
    {
		this = this / b;
    }
}