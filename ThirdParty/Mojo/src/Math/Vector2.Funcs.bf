namespace System;

using Mojo;

extension Math
{
	// Computes sign of 'x'
	[Inline]
 	public static Vector2 Sign(Vector2 v)
	{
	    return Vector2(Sign(v.x), Sign(v.y));
	}

	/* Computes absolute value
	 */
	[Inline]
 	public static Vector2 Abs(Vector2 v)
	{
	    return Vector2(Abs(v.x), Abs(v.y));
	}

	/* Computes cosine
	 */
	[Inline]
 	public static Vector2 Cos(Vector2 v)
	{
	    return Vector2(Cos(v.x), Cos(v.y));
	}

	/* Computes sine
	 */
	[Inline]
 	public static Vector2 Sin(Vector2 v)
	{
	    return Vector2(Sin(v.x), Sin(v.y));
	}

	/* Computes tangent
	 */
	[Inline]
 	public static Vector2 Tan(Vector2 v)
	{
	    return Vector2(Tan(v.x), Tan(v.y));
	}

	/* Computes hyperbolic cosine
	 */
	[Inline]
 	public static Vector2 Cosh(Vector2 v)
	{
	    return Vector2(Cosh(v.x), Cosh(v.y));
	}

	/* Computes hyperbolic sine
	 */
	[Inline]
 	public static Vector2 Sinh(Vector2 v)
	{
	    return Vector2(Sinh(v.x), Sinh(v.y));
	}

	/* Computes hyperbolic tangent
	 */
	[Inline]
 	public static Vector2 Tanh(Vector2 v)
	{
	    return Vector2(Tanh(v.x), Tanh(v.y));
	}

	/* Computes inverse cosine
	 */
	[Inline]
 	public static Vector2 Acos(Vector2 v)
	{
	    return Vector2(Acos(v.x), Acos(v.y));
	}

	/* Computes inverse sine
	 */
	[Inline]
 	public static Vector2 Asin(Vector2 v)
	{
	    return Vector2(Asin(v.x), Asin(v.y));
	}

	/* Computes inverse tangent
	 */
	[Inline]
 	public static Vector2 Atan(Vector2 v)
	{
	    return Vector2(Atan(v.x), Atan(v.y));
	}

	/* Computes inverse tangent with 2 args
	 */
	[Inline]
 	public static Vector2 Atan2(Vector2 a, Vector2 b)
	{
	    return Vector2(Atan2(a.x, b.x), Atan2(a.y, b.y));
	}

	/* Computes Euler number raised to the power 'x'
	 */
	[Inline]
 	public static Vector2 Exp(Vector2 v)
	{
	    return Vector2(Exp(v.x), Exp(v.y));
	}

	/* Computes 2 raised to the power 'x'
	 */
	[Inline]
 	public static Vector2 Exp2(Vector2 v)
	{
	    return Vector2(Exp2(v.x), Exp2(v.y));
	}

	/* Computes the base Euler number logarithm
	 */
	[Inline]
 	public static Vector2 Log(Vector2 v)
	{
	    return Vector2(Log(v.x), Log(v.y));
	}

	/* Computes the base 2 logarithm
	 */
	[Inline]
 	public static Vector2 Log2(Vector2 v)
	{
	    return Vector2(Log2(v.x), Log2(v.y));
	}

	/* Computes the base 10 logarithm
	 */
	[Inline]
 	public static Vector2 Log10(Vector2 v)
	{
	    return Vector2(Log10(v.x), Log10(v.y));
	}

	/* Computes the value of base raised to the power exponent
	 */
	[Inline]
 	public static Vector2 Pow(Vector2 a, Vector2 b)
	{
	    return Vector2(Pow(a.x, b.x), Pow(a.y, b.y));
	}

	/* Get the fractal part of floating point
	 */
	[Inline]
 	public static Vector2 Frac(Vector2 v)
	{
	    return Vector2(Frac(v.x), Frac(v.y));
	}

	/* Computes the floating-point remainder of the division operation x/y
	 */
	[Inline]
 	public static Vector2 Mod(Vector2 a, Vector2 b)
	{
	    return Vector2(Mod(a.x, b.x), Mod(a.y, b.y));
	}

	/* Computes the smallest integer value not less than 'x'
	 */
	[Inline]
 	public static Vector2 Ceiling(Vector2 v)
	{
	    return Vector2(Ceiling(v.x), Ceiling(v.y));
	}

	/* Computes the largest integer value not greater than 'x'
	 */
	[Inline]
 	public static Vector2 Floor(Vector2 v)
	{
	    return Vector2(Floor(v.x), Floor(v.y));
	}

	/* Computes the nearest integer value
	 */
	[Inline]
 	public static Vector2 Round(Vector2 v)
	{
	    return Vector2(Round(v.x), Round(v.y));
	}

	/* Computes the nearest integer not greater in magnitude than 'x'
	 */
	[Inline]
 	public static Vector2 Truncate(Vector2 v)
	{
	    return Vector2(Truncate(v.x), Truncate(v.y));
	}

	/* Get the smaller value
	 */
	[Inline]
 	public static Vector2 Min(Vector2 a, Vector2 b)
	{
	    return Vector2(Min(a.x, b.x), Min(a.y, b.y));
	}

	/* Get the larger value
	 */
	[Inline]
 	public static Vector2 Max(Vector2 a, Vector2 b)
	{
	    return Vector2(Max(a.x, b.x), Max(a.y, b.y));
	}

	/* Clamps the 'x' value to the [min, max].
	 */
	[Inline]
 	public static Vector2 Clamp(Vector2 v, Vector2 min, Vector2 max)
	{
	    return Vector2(Clamp(v.x, min.x, max.x), Clamp(v.y, min.y, max.y));
	}

	/* Clamps the specified value within the range of 0 to 1
	 */
	[Inline]
 	public static Vector2 Saturate(Vector2 v)
	{
	    return Vector2(Saturate(v.x), Saturate(v.y));
	}

	/* Compares two values, returning 0 or 1 based on which value is greater.
	 */
	[Inline]
 	public static Vector2 Step(Vector2 a, Vector2 b)
	{
	    return Vector2(Step(a.x, b.x), Step(a.y, b.y));
	}

	/* Performs a linear interpolation.
	 */
	[Inline]
 	public static Vector2 Lerp(Vector2 a, Vector2 b, Vector2 t)
	{
	    return Vector2(Lerp(a.x, b.x, t.x), Lerp(a.y, b.y, t.y));
	}

	/* Performs a linear interpolation.
	 */
	[Inline]
 	public static Vector2 Lerp(Vector2 a, Vector2 b, float t)
	{
	    return Vector2(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t));
	}

	/* Compute a smooth Hermite interpolation
	 */
	[Inline]
 	public static Vector2 Smoothstep(Vector2 a, Vector2 b, Vector2 t)
	{
	    return Vector2(Smoothstep(a.x, b.x, t.x), Smoothstep(a.y, b.y, t.y));
	}

	/* Computes square root of 'x'.
	 */
	[Inline]
 	public static Vector2 Sqrt(Vector2 v)
	{
	    return Vector2(Sqrt(v.x), Sqrt(v.y));
	}

	/* Computes inverse square root of 'x'.
	 */
	[Inline]
 	public static Vector2 InvSqrt(Vector2 v)
	{
	    return Vector2(InvSqrt(v.x), InvSqrt(v.y));
	}

	/* Computes fast inverse square root of 'x'.
	 */
	[Inline]
 	public static Vector2 FastSqrt(Vector2 v)
	{
	    return Vector2(FastSqrt(v.x), FastSqrt(v.y));
	}

	/* Computes fast inverse square root of 'x'.
	 */
	[Inline]
 	public static Vector2 FastInvSqrt(Vector2 v)
	{
	    return Vector2(FastInvSqrt(v.x), FastInvSqrt(v.y));
	}

	//
	// @region: Graphics funtions
	//

	/* Compute Dot product of two vectors
	 */
	[Inline]
 	public static float Dot(Vector2 a, Vector2 b)
	{
	    return a.x * b.x + a.y * b.y;
	}

	/* Compute squared Length of vector
	 */
	[Inline]
 	public static float LengthSq(Vector2 v)
	{
	    return Dot(v, v);
	}

	/* Compute Length of vector
	 */
	[Inline]
 	public static float Length(Vector2 v)
	{
	    return Sqrt(LengthSq(v));
	}

	/* Compute distance from 'a' to b
	 */
	[Inline]
 	public static float Distance(Vector2 a, Vector2 b)
	{
	    return Length(a - b);
	}

	/* Compute squared distance from 'a' to b
	 */
	[Inline]
 	public static float DistanceSq(Vector2 a, Vector2 b)
	{
	    return LengthSq(a - b);
	}

	/* Compute normalized vector
	 */
	[Inline]
 	public static Vector2 Normalize(Vector2 v)
	{
	    let lsqr = LengthSq(v);
	    if (lsqr > 0.0f)
	    {
	        let f = InvSqrt(lsqr);
	        return Vector2(v.x * f, v.y * f);
	    }
	    else
	    {
	        return v;
	    }
	}

	/* Compute reflection vector
	 */
	[Inline]
 	public static Vector2 Reflect(Vector2 v, Vector2 n)
	{
	    return v - 2.0f * Dot(v, n) * n;
	}

	/* Compute refraction vector
	 */
	[Inline]
 	public static Vector2 Refract(Vector2 v, Vector2 n, float eta)
	{
	    let k = 1.0f - eta * eta * (1.0f - Dot(v, n) * Dot(v, n));
	    return k < 0.0f
	        ? Vector2(0.0f)
	        : eta * v - (eta * Dot(v, n) + Sqrt(k)) * v;
	}

	/* Compute faceforward vector
	 */
	[Inline]
 	public static Vector2 Faceforward(Vector2 n, Vector2 i, Vector2 nref)
	{
	    return Dot(i, nref) < 0.0f ? n : -n;
	}

	/* Compute angle of vector, a.k.a direction of vector
	 */
	[Inline]
 	public static float Angle(Vector2 v)
	{
	    return Atan2(v.y, v.x);
	}

	/* Compute angle of two vector
	 */
	[Inline]
 	public static float Angle(Vector2 a, Vector2 b)
	{
	    return Angle(b - a);
	}
}