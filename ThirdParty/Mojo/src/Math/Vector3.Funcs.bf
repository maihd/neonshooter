namespace System;

using Mojo;

extension Math
{
	/* Computes sign of 'x'
	*/
	[Inline]
 	public static Vector3 Sign(Vector3 v)
	{
	    return Vector3(Sign(v.x), Sign(v.y), Sign(v.z));
	}

	/* Computes absolute value
	 */
	[Inline]
 	public static Vector3 Abs(Vector3 v)
	{
	    return Vector3(Abs(v.x), Abs(v.y), Abs(v.z));
	}

	/* Computes cosine
	 */
	[Inline]
 	public static Vector3 Cos(Vector3 v)
	{
	    return Vector3(Cos(v.x), Cos(v.y), Cos(v.z));
	}

	/* Computes sine
	 */
	[Inline]
 	public static Vector3 Sin(Vector3 v)
	{
	    return Vector3(Sin(v.x), Sin(v.y), Sin(v.z));
	}

	/* Computes tangent
	 */
	[Inline]
 	public static Vector3 Tan(Vector3 v)
	{
	    return Vector3(Tan(v.x), Tan(v.y), Tan(v.z));
	}

	/* Computes hyperbolic cosine
	 */
	[Inline]
 	public static Vector3 Cosh(Vector3 v)
	{
	    return Vector3(Cosh(v.x), Cosh(v.y), Cosh(v.z));
	}

	/* Computes hyperbolic sine
	 */
	[Inline]
 	public static Vector3 Sinh(Vector3 v)
	{
	    return Vector3(Sinh(v.x), Sinh(v.y), Sinh(v.z));
	}

	/* Computes hyperbolic tangent
	 */
	[Inline]
 	public static Vector3 Tanh(Vector3 v)
	{
	    return Vector3(Tanh(v.x), Tanh(v.y), Tanh(v.z));
	}

	/* Computes inverse cosine
	 */
	[Inline]
 	public static Vector3 Acos(Vector3 v)
	{
	    return Vector3(Acos(v.x), Acos(v.y), Acos(v.z));
	}

	/* Computes inverse sine
	 */
	[Inline]
 	public static Vector3 Asin(Vector3 v)
	{
	    return Vector3(Asin(v.x), Asin(v.y), Asin(v.z));
	}

	/* Computes inverse tangent
	 */
	[Inline]
 	public static Vector3 Atan(Vector3 v)
	{
	    return Vector3(Atan(v.x),
	        Atan(v.y),
	        Asin(v.z));
	}

	/* Computes inverse tangent with 2 args
	 */
	[Inline]
 	public static Vector3 Atan2(Vector3 a, Vector3 b)
	{
	    return Vector3(Atan2(a.x, b.x),
	        Atan2(a.y, b.y),
	        Atan2(a.z, b.z));
	}

	/* Computes Euler number raised to the power 'x'
	 */
	[Inline]
 	public static Vector3 Exp(Vector3 v)
	{
	    return Vector3(Exp(v.x),
	        Exp(v.y),
	        Exp(v.z));
	}

	/* Computes 2 raised to the power 'x'
	 */
	[Inline]
 	public static Vector3 Exp2(Vector3 v)
	{
	    return Vector3(Exp2(v.x),
	        Exp2(v.y),
	        Exp2(v.z));
	}

	/* Computes the base Euler number logarithm
	 */
	[Inline]
 	public static Vector3 Log(Vector3 v)
	{
	    return Vector3(Log(v.x),
	        Log(v.y),
	        Log(v.z));
	}

	/* Computes the base 2 logarithm
	 */
	[Inline]
 	public static Vector3 Log2(Vector3 v)
	{
	    return Vector3(Log2(v.x),
	        Log2(v.y),
	        Log2(v.z));
	}

	/* Computes the base 10 logarithm
	 */
	[Inline]
 	public static Vector3 Log10(Vector3 v)
	{
	    return Vector3(Log10(v.x),
	        Log10(v.y),
	        Log10(v.z));
	}

	/* Computes the value of base raised to the power exponent
	 */
	[Inline]
 	public static Vector3 Pow(Vector3 a, Vector3 b)
	{
	    return Vector3(Pow(a.x, b.x),
	        Pow(a.y, b.y),
	        Pow(a.z, b.z));
	}

	/* Get the fractal part of floating point
	 */
	[Inline]
 	public static Vector3 Frac(Vector3 v)
	{
	    return Vector3(Frac(v.x),
	        Frac(v.y),
	        Frac(v.z));
	}

	/* Computes the floating-point remainder of the division operation x/y
	 */
	[Inline]
 	public static Vector3 Mod(Vector3 a, Vector3 b)
	{
	    return Vector3(Mod(a.x, b.x),
	        Mod(a.y, b.y),
	        Mod(a.z, b.z));
	}

	/* Computes the smallest integer value not less than 'x'
	 */
	[Inline]
 	public static Vector3 Ceiling(Vector3 v)
	{
	    return Vector3(Ceiling(v.x),
	        Ceiling(v.y),
	        Ceiling(v.z));
	}

	/* Computes the largest integer value not greater than 'x'
	 */
	[Inline]
 	public static Vector3 Floor(Vector3 v)
	{
	    return Vector3(Floor(v.x),
	        Floor(v.y),
	        Floor(v.z));
	}

	/* Computes the nearest integer value
	 */
	[Inline]
 	public static Vector3 Round(Vector3 v)
	{
	    return Vector3(Round(v.x),
	        Round(v.y),
	        Round(v.z));
	}

	/* Computes the nearest integer not greater in magnitude than 'x'
	 */
	[Inline]
 	public static Vector3 Truncate(Vector3 v)
	{
	    return Vector3(Truncate(v.x),
	        Truncate(v.y),
	        Truncate(v.z));
	}

	/* Get the smaller value
	 */
	[Inline]
 	public static Vector3 Min(Vector3 a, Vector3 b)
	{
	    return Vector3(Min(a.x, b.x),
	        Min(a.y, b.y),
	        Min(a.z, b.z));
	}

	/* Get the larger value
	 */
	[Inline]
 	public static Vector3 Max(Vector3 a, Vector3 b)
	{
	    return Vector3(Max(a.x, b.x),
	        Max(a.y, b.y),
	        Max(a.z, b.z));
	}

	/* Clamps the 'x' value to the [min, max].
	 */
	[Inline]
 	public static Vector3 Clamp(Vector3 v, Vector3 min, Vector3 max)
	{
	    return Vector3(Clamp(v.x, min.x, max.x),
	        Clamp(v.y, min.y, max.y),
	        Clamp(v.z, min.z, max.z));
	}

	/* Clamps the specified value within the range of 0 to 1
	 */
	[Inline]
 	public static Vector3 Saturate(Vector3 v)
	{
	    return Vector3(Saturate(v.x),
	        Saturate(v.y),
	        Saturate(v.z));
	}

	/* Compares two values, returning 0 or 1 based on which value is greater.
	 */
	[Inline]
 	public static Vector3 Step(Vector3 a, Vector3 b)
	{
	    return Vector3(
	        Step(a.x, b.x),
	        Step(a.y, b.y),
	        Step(a.z, b.z)
	    );
	}

	/* Performs a linear interpolation.
	 */
	[Inline]
 	public static Vector3 Lerp(Vector3 a, Vector3 b, Vector3 t)
	{
	    return Vector3(Lerp(a.x, b.x, t.x),
	        Lerp(a.y, b.y, t.y),
	        Lerp(a.z, b.z, t.z));
	}

	/* Performs a linear interpolation.
	 */
	[Inline]
 	public static Vector3 Lerp(Vector3 a, Vector3 b, float t)
	{
	    return Vector3(Lerp(a.x, b.x, t),
	        Lerp(a.y, b.y, t),
	        Lerp(a.z, b.z, t));
	}

	/* Compute a smooth Hermite interpolation
	 */
	[Inline]
 	public static Vector3 Smoothstep(Vector3 a, Vector3 b, Vector3 t)
	{
	    return Vector3(Smoothstep(a.x, b.x, t.x),
	        Smoothstep(a.y, b.y, t.y),
	        Smoothstep(a.z, b.z, t.z));
	}

	/* Computes square root of 'x'.
	 */
	[Inline]
 	public static Vector3 Sqrt(Vector3 v)
	{
	    return Vector3(Sqrt(v.x),
	        Sqrt(v.y),
	        Sqrt(v.z));
	}

	/* Computes inverse square root of 'x'.
	 */
	[Inline]
 	public static Vector3 InvSqrt(Vector3 v)
	{
	    return Vector3(InvSqrt(v.x),
	        InvSqrt(v.y),
	        InvSqrt(v.z));
	}

	/* Computes fast square root of 'x'.
	 */
	[Inline]
 	public static Vector3 FastSqrt(Vector3 v)
	{
	    return Vector3(FastSqrt(v.x),
	        FastSqrt(v.y),
	        FastSqrt(v.z));
	}

	/* Computes fast inverse square root of 'x'.
	 */
	[Inline]
 	public static Vector3 FastInvSqrt(Vector3 v)
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
	[Inline]
 	public static Vector3 Cross(Vector3 a, Vector3 b)
	{
	    return Vector3(
	        a.y * b.z - a.z * b.y,
	        a.z * b.x - a.x * b.z,
	        a.x * b.y - a.y * b.x
	    );
	}

	/* Compute Dot product of two vectors
	 */
	[Inline]
 	public static float Dot(Vector3 a, Vector3 b)
	{
	    return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	/* Compute squared Length of vector
	 */
	[Inline]
 	public static float LengthSq(Vector3 v)
	{
	    return Dot(v, v);
	}

	/* Compute Length of vector
	 */
	[Inline]
 	public static float Length(Vector3 v)
	{
	    return Sqrt(LengthSq(v));
	}

	/* Compute distance from 'a' to b
	 */
	[Inline]
 	public static float Distance(Vector3 a, Vector3 b)
	{
	    return Length(a - b);
	}

	/* Compute squared distance from 'a' to b
	 */
	[Inline]
 	public static float DistanceSq(Vector3 a, Vector3 b)
	{
	    return LengthSq(a - b);
	}

	/* Compute normalized vector
	 */
	[Inline]
 	public static Vector3 Normalize(Vector3 v)
	{
	    let lsqr = LengthSq(v);
	    if (lsqr > 0.0f)
	    {
	        let f = InvSqrt(lsqr);
	        return Vector3(v.x * f, v.y * f, v.z * f);
	    }
	    else
	    {
	        return v;
	    }
	}

	/* Compute reflection vector
	 */
	[Inline]
 	public static Vector3 Reflect(Vector3 v, Vector3 n)
	{
	    return v - 2.0f * Dot(v, n) * n;
	}

	/* Compute refraction vector
	 */
	[Inline]
 	public static Vector3 Refract(Vector3 v, Vector3 n, float eta)
	{
	    let k = 1.0f - eta * eta * (1.0f - Dot(v, n) * Dot(v, n));
	    return k < 0.0f
	        ? Vector3(0.0f)
	        : eta * v - (eta * Dot(v, n) + Sqrt(k)) * n;
	}

	/* Compute faceforward vector
	 */
	[Inline]
 	public static Vector3 Faceforward(Vector3 n, Vector3 i, Vector3 nref)
	{
	    return Dot(i, nref) < 0.0f ? n : -n;
	}
}