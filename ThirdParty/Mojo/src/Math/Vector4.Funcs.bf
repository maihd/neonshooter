namespace System;

using Mojo;

extension Math
{
    /* Computes sign of 'x'
     */
    [Inline]
 	public static Vector4 Sign(Vector4 v)
    {
        return Vector4(Sign(v.x),
            Sign(v.y),
            Sign(v.z),
            Sign(v.w));
    }

    /* Computes absolute value
     */
    [Inline]
 	public static Vector4 Abs(Vector4 v)
    {
        return Vector4(Abs(v.x),
            Abs(v.y),
            Abs(v.z),
            Abs(v.w));
    }

    /* Computes cosine
        */
    [Inline]
 	public static Vector4 Cos(Vector4 v)
    {
        return Vector4(Cos(v.x),
            Cos(v.y),
            Cos(v.z),
            Cos(v.w));
    }

    /* Computes sine
        */
    [Inline]
 	public static Vector4 Sin(Vector4 v)
    {
        return Vector4(Sin(v.x),
            Sin(v.y),
            Sin(v.z),
            Sin(v.w));
    }

    /* Computes tangent
        */
    [Inline]
 	public static Vector4 Tan(Vector4 v)
    {
        return Vector4(Tan(v.x),
            Tan(v.y),
            Tan(v.z),
            Tan(v.w));
    }

    /* Computes hyperbolic cosine
        */
    [Inline]
 	public static Vector4 Cosh(Vector4 v)
    {
        return Vector4(Cosh(v.x),
            Cosh(v.y),
            Cosh(v.z),
            Cosh(v.w));
    }

    /* Computes hyperbolic sine
        */
    [Inline]
 	public static Vector4 Sinh(Vector4 v)
    {
        return Vector4(Sinh(v.x),
            Sinh(v.y),
            Sinh(v.z),
            Sinh(v.w));
    }

    /* Computes hyperbolic tangent
        */
    [Inline]
 	public static Vector4 Tanh(Vector4 v)
    {
        return Vector4(Tanh(v.x),
            Tanh(v.y),
            Tanh(v.z),
            Tanh(v.w));
    }

    /* Computes inverse cosine
        */
    [Inline]
 	public static Vector4 Acos(Vector4 v)
    {
        return Vector4(Acos(v.x),
            Acos(v.y),
            Acos(v.z),
            Acos(v.w));
    }

    /* Computes inverse sine
        */
    [Inline]
 	public static Vector4 Asin(Vector4 v)
    {
        return Vector4(Asin(v.x),
            Asin(v.y),
            Asin(v.z),
            Asin(v.w));
    }

    /* Computes inverse tangent
        */
    [Inline]
 	public static Vector4 Atan(Vector4 v)
    {
        return Vector4(Atan(v.x),
            Atan(v.y),
            Atan(v.z),
            Atan(v.w));
    }

    /* Computes inverse tangent with 2 args
        */
    [Inline]
 	public static Vector4 Atan2(Vector4 a, Vector4 b)
    {
        return Vector4(Atan2(a.x, b.x), Atan2(a.y, b.y), Atan2(a.z, b.z), Atan2(a.w, b.w));
    }

    /* Computes Euler number raised to the power 'x'
        */
    [Inline]
 	public static Vector4 Exp(Vector4 v)
    {
        return Vector4(Exp(v.x), Exp(v.y), Exp(v.z), Exp(v.w));
    }

    /* Computes 2 raised to the power 'x'
        */
    [Inline]
 	public static Vector4 Exp2(Vector4 v)
    {
        return Vector4(Exp2(v.x), Exp2(v.y), Exp2(v.z), Exp2(v.w));
    }

    /* Computes the base Euler number logarithm
        */
    [Inline]
 	public static Vector4 Log(Vector4 v)
    {
        return Vector4(Log(v.x), Log(v.y), Log(v.z), Log(v.w));
    }

    /* Computes the base 2 logarithm
        */
    [Inline]
 	public static Vector4 Log2(Vector4 v)
    {
        return Vector4(Log2(v.x), Log2(v.y), Log2(v.z), Log2(v.w));
    }

    /* Computes the base 10 logarithm
        */
    [Inline]
 	public static Vector4 Log10(Vector4 v)
    {
        return Vector4(Log10(v.x), Log10(v.y), Log10(v.z), Log10(v.w));
    }

    /* Computes the value of base raised to the power exponent
        */
    [Inline]
 	public static Vector4 Pow(Vector4 a, Vector4 b)
    {
        return Vector4(
            Pow(a.x, b.x),
            Pow(a.y, b.y),
            Pow(a.z, b.z),
            Pow(a.w, b.w));
    }

    /* Get the fractal part of floating point
        */
    [Inline]
 	public static Vector4 Frac(Vector4 v)
    {
        return Vector4(
            Frac(v.x),
            Frac(v.y),
            Frac(v.z),
            Frac(v.w));
    }

    /* Computes the floating-point remainder of the division operation x/y
        */
    [Inline]
 	public static Vector4 Mod(Vector4 a, Vector4 b)
    {
        return Vector4(
            Mod(a.x, b.x),
            Mod(a.y, b.y),
            Mod(a.z, b.z),
            Mod(a.w, b.w));
    }

    /* Computes the smallest integer value not less than 'x'
        */
    [Inline]
 	public static Vector4 Ceiling(Vector4 v)
    {
        return Vector4(
            Ceiling(v.x),
            Ceiling(v.y),
            Ceiling(v.z),
            Ceiling(v.w));
    }

    /* Computes the largest integer value not greater than 'x'
        */
    [Inline]
 	public static Vector4 Floor(Vector4 v)
    {
        return Vector4(
            Floor(v.x),
            Floor(v.y),
            Floor(v.z),
            Floor(v.w));
    }

    /* Computes the nearest integer value
        */
    [Inline]
 	public static Vector4 Round(Vector4 v)
    {
        return Vector4(
            Round(v.x),
            Round(v.y),
            Round(v.z),
            Round(v.w));
    }

    /* Computes the nearest integer not greater in magnitude than 'x'
        */
    [Inline]
 	public static Vector4 Truncate(Vector4 v)
    {
        return Vector4(
            Truncate(v.x),
            Truncate(v.y),
            Truncate(v.z),
            Truncate(v.w));
    }

    /* Get the smaller value
        */
    [Inline]
 	public static Vector4 Min(Vector4 a, Vector4 b)
    {
        return Vector4(
            Min(a.x, b.x),
            Min(a.y, b.y),
            Min(a.z, b.z),
            Min(a.w, b.w));
    }

    /* Get the larger value
        */
    [Inline]
 	public static Vector4 Max(Vector4 a, Vector4 b)
    {
        return Vector4(
            Max(a.x, b.x),
            Max(a.y, b.y),
            Max(a.z, b.z),
            Max(a.w, b.w));
    }

    /* Clamps the 'x' value to the [min, max].
        */
    [Inline]
 	public static Vector4 Clamp(Vector4 v, Vector4 min, Vector4 max)
    {
        return Vector4(
            Clamp(v.x, min.x, max.x),
            Clamp(v.y, min.y, max.y),
            Clamp(v.z, min.z, max.z),
            Clamp(v.w, min.w, max.w));
    }

    /* Clamps the specified value within the range of 0 to 1
        */
    [Inline]
 	public static Vector4 Saturate(Vector4 v)
    {
        return Vector4(
            Saturate(v.x),
            Saturate(v.y),
            Saturate(v.z),
            Saturate(v.w));
    }

    /* Compares two values, returning 0 or 1 based on which value is greater.
        */
    [Inline]
 	public static Vector4 Step(Vector4 a, Vector4 b)
    {
        return Vector4(
            Step(a.x, b.x),
            Step(a.y, b.y),
            Step(a.z, b.z),
            Step(a.w, b.w));
    }

    /* Performs a linear interpolation.
        */
    [Inline]
 	public static Vector4 Lerp(Vector4 a, Vector4 b, Vector4 t)
    {
        return Vector4(
            Lerp(a.x, b.x, t.x),
            Lerp(a.y, b.y, t.y),
            Lerp(a.z, b.z, t.z),
            Lerp(a.w, b.w, t.w));
    }

    /* Performs a linear interpolation.
        */
    [Inline]
 	public static Vector4 Lerp(Vector4 a, Vector4 b, float t)
    {
        return Vector4(
            Lerp(a.x, b.x, t),
            Lerp(a.y, b.y, t),
            Lerp(a.z, b.z, t),
            Lerp(a.w, b.w, t));
    }

    /* Compute a smooth Hermite interpolation
        */
    [Inline]
 	public static Vector4 Smoothstep(Vector4 a, Vector4 b, Vector4 t)
    {
        return Vector4(
            Smoothstep(a.x, b.x, t.x),
            Smoothstep(a.y, b.y, t.y),
            Smoothstep(a.z, b.z, t.z),
            Smoothstep(a.w, b.w, t.w));
    }

    /* Computes square root of 'x'.
        */
    [Inline]
 	public static Vector4 Sqrt(Vector4 v)
    {
        return Vector4(Sqrt(v.x), Sqrt(v.y), Sqrt(v.z), Sqrt(v.w));
    }

    /* Computes inverse square root of 'x'.
        */
    [Inline]
 	public static Vector4 InvSqrt(Vector4 v)
    {
        return Vector4(InvSqrt(v.x), InvSqrt(v.y), InvSqrt(v.z), InvSqrt(v.w));
    }

    /* Computes fast square root of 'x'.
        */
    [Inline]
 	public static Vector4 FastSqrt(Vector4 v)
    {
        return Vector4(FastSqrt(v.x), FastSqrt(v.y), FastSqrt(v.z), FastSqrt(v.w));
    }

    /* Computes fast inverse square root of 'x'.
        */
    [Inline]
 	public static Vector4 FastInvSqrt(Vector4 v)
    {
        return Vector4(FastInvSqrt(v.x), FastInvSqrt(v.y), FastInvSqrt(v.z), FastInvSqrt(v.w));
    }

    //
    // @region: Graphics functions
    //

    /* Compute Dot product of two vectors
        */
    [Inline]
 	public static float Dot(Vector4 a, Vector4 b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    /* Compute squared Length of vector
        */
    [Inline]
 	public static float LengthSq(Vector4 v)
    {
        return Dot(v, v);
    }

    /* Compute Length of vector
        */
    [Inline]
 	public static float Length(Vector4 v)
    {
        return Sqrt(LengthSq(v));
    }

    /* Compute distance from 'a' to b
        */
    [Inline]
 	public static float Distance(Vector4 a, Vector4 b)
    {
        return Length(a - b);
    }

    /* Compute squared distance from 'a' to b
        */
    [Inline]
 	public static float DistanceSq(Vector4 a, Vector4 b)
    {
        return LengthSq(a - b);
    }

    /* Compute normalized vector
        */
    [Inline]
 	public static Vector4 Normalize(Vector4 v)
    {
        let lsqr = LengthSq(v);
        if (lsqr > 0.0f)
        {
            let f = InvSqrt(lsqr);
            return Vector4(v.x * f, v.y * f, v.z * f, v.w * f);
        }
        else
        {
            return v;
        }
    }

    /* Compute reflection vector
        */
    [Inline]
 	public static Vector4 Reflect(Vector4 v, Vector4 n)
    {
        return v - 2.0f * Dot(v, n) * n;
    }

    /* Compute refraction vector
        */
    [Inline]
 	public static Vector4 Refract(Vector4 v, Vector4 n, float eta)
    {
        let k = 1.0f - eta * eta * (1.0f - Dot(v, n) * Dot(v, n));
        return k < 0.0f
            ? Vector4(0.0f)
            : eta * v - (eta * Dot(v, n) + Sqrt(k)) * n;
    }

    /* Compute faceforward vector
        */
    [Inline]
 	public static Vector4 Faceforward(Vector4 n, Vector4 i, Vector4 nref)
    {
        return Dot(i, nref) < 0.0f ? n : -n;
    }
}