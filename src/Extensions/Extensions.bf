namespace System;

using Raylib;

public extension Random
{
	public float NextFloat(float min, float max)
	{
		return min + (.)(NextDouble() * (max - min));
	}

	public Vector2 NextVector2(float min, float max)
	{
		let angle = NextFloat(0, 2 * Math.PI_f);
		let length = NextFloat(min, max);
		return .FromAngle(angle, length);
	}
}