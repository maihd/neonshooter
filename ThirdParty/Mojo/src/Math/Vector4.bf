namespace Mojo;

using System;

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

	//
	// Vector4 operators
	//

	[Inline]
	public static Self operator-(Self v)
	{
	    return .(-v.x, -v.y, -v.z, -v.w);
	}

	[Inline]
	public static Self operator+(Self v)
	{
	    return v;
	}

	[Inline]
	public static Self operator--(ref Self v)
	{
	    --v.x;
		--v.y;
		--v.z;
	    --v.w;
	    return v;
	}

	[Inline]
	public static Self operator++(ref Self v)
	{
	    ++v.x;
		++v.y;
		++v.z;
	    ++v.w;
	    return v;
	}

	[Inline]
	public static Self operator+(Self a, Self b)
	{
	    return .(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}

	[Inline]
	public static Self operator-(Self a, Self b)
	{
	    return .(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	}

	[Inline]
	public static Self operator*(Self a, Self b)
	{
	    return .(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
	}

	[Inline]
	public static Self operator/(Self a, Self b)
	{
	    return .(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
	}

	[Inline]
	public static Self operator+(Self a, float b)
	{
	    return a + .(b);
	}
	
	[Inline]
	public static Self operator-(Self a, float b)
	{
	    return a - .(b);
	}
	
	[Inline]
	public static Self operator*(Self a, float b)
	{
	    return a * .(b);
	}
	
	[Inline]
	public static Self operator/(Self a, float b)
	{
	    return a / .(b);
	}
	
	[Inline]
	public static Self operator+(float a, Self b)
	{
	    return .(a) + b;
	}
	
	[Inline]
	public static Self operator-(float a, Self b)
	{
	    return .(a) - b;
	}
	
	[Inline]
	public static Self operator*(float a, Self b)
	{
	    return .(a) * b;
	}
	
	[Inline]
	public static Self operator/(float a, Self b)
	{
	    return .(a) / b;
	}
	
	[Inline]
	public void operator+=(Self b) mut
	{
	    this = this + b;
	}
	
	[Inline]
	public void operator+=(float b) mut
	{
	    this = this + b;
	}
	
	[Inline]
	public void operator-=(Self b) mut
	{
	    this = this - b;
	}
	
	[Inline]
	public void operator-=(float b) mut
	{
	    this = this - b;
	}

	[Inline]
	public void operator*=(Self b) mut
	{
	    this = this * b;
	}
	
	[Inline]
	public void operator*=(float b) mut
	{
	    this = this * b;
	}
	
	[Inline]
	public void operator/=(Self b) mut
	{
	    this = this / b;
	}
	
	[Inline]
	public void operator/=(float b) mut
	{
	    this = this / b;
	}
	
	[Inline]
	public static bool operator==(Self a, Self b)
	{
	    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
	}

	[Inline]
	public static bool operator!=(Self a, Self b)
	{
	    return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
	}
}