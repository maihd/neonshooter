namespace Mojo;

using System;

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

	//
	// Vector2 operators
	//

	[Inline]
	public static Self operator-(Self v)
	{
	    return .(-v.x, -v.y);
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
	    return v;
	}

	[Inline]
	public static Self operator++(ref Self v)
	{
	    ++v.x;
	    ++v.y;
	    return v;
	}

	[Inline]
	public static Self operator+(Self a, Self b)
	{
	    return .(a.x + b.x, a.y + b.y);
	}

	[Inline]
	public static Self operator-(Self a, Self b)
	{
	    return .(a.x - b.x, a.y - b.y);
	}

	[Inline]
	public static Self operator*(Self a, Self b)
	{
	    return .(a.x * b.x, a.y * b.y);
	}

	[Inline]
	public static Self operator/(Self a, Self b)
	{
	    return .(a.x / b.x, a.y / b.y);
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
	    return a.x == b.x && a.y == b.y;
	}

	[Inline]
	public static bool operator!=(Self a, Self b)
	{
	    return a.x != b.x || a.y != b.y;
	}
}