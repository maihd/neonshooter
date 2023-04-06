namespace Mojo;

using System;

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

	//
	// Vector3 operators
	//

	[Inline]
	public static Self operator-(Self v)
	{
	    return .(-v.x, -v.y, -v.z);
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
	    return v;
	}

	[Inline]
	public static Self operator++(ref Self v)
	{
	    ++v.x;
		++v.y;
	    ++v.z;
	    return v;
	}

	[Inline]
	public static Self operator+(Self a, Self b)
	{
	    return .(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	[Inline]
	public static Self operator-(Self a, Self b)
	{
	    return .(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	[Inline]
	public static Self operator*(Self a, Self b)
	{
	    return .(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	[Inline]
	public static Self operator/(Self a, Self b)
	{
	    return .(a.x / b.x, a.y / b.y, a.z / b.z);
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
	    return a.x == b.x && a.y == b.y && a.z == b.z;
	}

	[Inline]
	public static bool operator!=(Self a, Self b)
	{
	    return a.x != b.x || a.y != b.y || a.z != b.z;
	}
}