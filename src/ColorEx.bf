namespace Raylib;

using System;

extension Color
{
	public this(float r, float g, float b, float a = 1.0f)
		: this((uint8)(r * 255), (uint8)(g * 255), (uint8)(b * 255), (uint8)(a * 255))
	{
	}

	public this(Color origin, float newAlpha)
	{
		this.r = origin.r;
		this.g = origin.g;
		this.b = origin.b;
		this.a = (uint8)(newAlpha * 255);
	}

	public static Color HSV(float h, float s, float v, float a = 1.0f)
	{
		if (h == 0 && s == 0)
			return Color(v, v, v, a);

		float c = s * v;

		float x = c * (1 - Math.Abs(h % 2 - 1));

		float m = v - c;

		if (h < 1) return Color(c + m, x + m, m);

		else if (h < 2) return Color(x + m, c + m, m);

		else if (h < 3) return Color(m, c + m, x + m);

		else if (h < 4) return Color(m, x + m, c + m);

		else if (h < 5) return Color(x + m, m, c + m);

		else return Color(c + m, m, x + m);
	}

	public static Color Lerp(Color color1, Color color2, float t)
	{
		return Color(
			(uint8)Math.Lerp(color1.r, color2.r, t),
			(uint8)Math.Lerp(color1.g, color2.g, t),
			(uint8)Math.Lerp(color1.b, color2.b, t),
			(uint8)Math.Lerp(color1.a, color2.a, t)
		);
	}
}