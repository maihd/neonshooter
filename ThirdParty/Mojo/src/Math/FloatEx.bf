namespace System;

extension Math
{
	[Union, Align(alignof(float))]
	private struct Converter
	{
	    public float 	f;
	    public int32   	i;
	}

	/* Get the fractional part of floating point
	*/
	[Inline]
    public static float Frac(float x)
	{
		float intPart;
	    return modff(x, out intPart);
	}

	/* Computes 2 raised to the power 'x'
	 */
	[Inline]
    public static float Exp2(float v)
	{
	    return Exp(v * 0.69314718f);
	}

	/* Computes the base 2 logarithm
	 */
	[Inline]
    public static float Log2(float v)
	{
	    return Log(v, 2.0f);
	}

	/* Computes the floating-point remainder of the division operation x/y
	 */
	[Inline]
    public static float Mod(float a, float b)
	{
	    return a % b;
	}

    /* Clamps the specified value within the range of 0 to 1
     */
    [Inline]
    public static float Saturate(float x)
    {
        return Clamp(x, 0.0f, 1.0f);
    }

    /* Compares two values, returning 0 or 1 based on which value is greater.
     */
    [Inline]
    public static float Step(float y, float x)
    {
        return x >= y ? 1.0f : 0.0f;
    }

	/* Compute a smooth Hermite interpolation
	 * @return: 0 if x <= min
	 *          1 if x >= max
	 *          (0, 1) otherwise
	 */
	[Inline]
    public static float Smoothstep(float min, float max, float x)
	{
	    return (Clamp(x, min, max) - min) / (max - min);
	}

    /* Computes inverse square root of 'x'.
     */
    [Inline]
	public static float InvSqrt(float x)
    {
        return 1.0f / Sqrt(x);
    }

	/* Computes fast inverse square root of 'x'.
	 */
	[Inline]
	public static float FastInvSqrt(float x)
	{
		Converter cvt;

	    cvt.f = x;
	    cvt.i = 0x5F3759DF - (cvt.i >> 1);
	    cvt.f = cvt.f * (1.5f - (0.5f * x * cvt.f * cvt.f));

	    return cvt.f;
	}

	/* Computes fast inverse square root of 'x'.
	 */
	[Inline]
	public static float FastSqrt(float x)
	{
	    return x == 0.0f ? 0.0f : 1.0f / FastInvSqrt(x);
	}
}