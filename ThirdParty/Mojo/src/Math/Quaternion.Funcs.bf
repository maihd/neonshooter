namespace System;

using Mojo;

extension Math
{
	/* Quaternion multiplication
	 */
	[Inline]
	public static Quaterion Mul(Quaterion a, Quaterion b)
	{
	    let a3 = Vector3(a.x, a.y, a.z);
	    let b3 = Vector3(b.x, b.y, b.z);

		let v = a3 * b.w + b3 * a.w + Cross(a3, b3);
	    let w = a.w * b.w - Dot(a3, b3);
	    return Quaterion(v.x, v.y, v.z, w);
	}

	[Inline]
	public static Quaterion Inverse(Quaterion q)
	{
	    return Quaterion(q.x, q.y, q.z, -q.w);
	}

	[Inline]
	public static Quaterion Conjugate(Quaterion q)
	{
	    return Quaterion(-q.x, -q.y, -q.z, q.w);
	}
}