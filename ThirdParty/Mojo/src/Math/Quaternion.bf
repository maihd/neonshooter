namespace Mojo;

using System;

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
    public this(Vector3 axis, float angle)
    {
        let lsqr = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
        if (lsqr == 0.0f)
        {
            this.x = 0.0f;
            this.y = 0.0f;
            this.z = 0.0f;
            this.w = 1.0f;
        }
        else
        {
            let f = 1.0f / lsqr * Math.Sin(angle * 0.5f);

            this.x = axis.x * f;
            this.y = axis.y * f;
            this.z = axis.z * f;
            this.w = Math.Cos(angle * 0.5f);
        }
    }

    [Inline]
    public this(float ex, float ey, float ez)
    {
        let r = ez * 0.5f; // Roll
        let p = ex * 0.5f; // Pitch
        let y = ey * 0.5f; // Yaw

        let c1 = Math.Cos(y);
        let c2 = Math.Cos(p);
        let c3 = Math.Cos(r);
        let s1 = Math.Sin(y);
        let s2 = Math.Sin(p);
        let s3 = Math.Sin(r);

        this.x = s1 * s2 * c3 + c1 * c2 * s3;
        this.y = s1 * c2 * c3 + c1 * s2 * s3;
        this.z = c1 * s2 * c3 - s1 * c2 * s3;
        this.w = c1 * c2 * c3 - s1 * s2 * s3;
    }
}