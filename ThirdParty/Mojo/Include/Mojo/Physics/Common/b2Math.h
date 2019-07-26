/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef B2_MATH_H
#define B2_MATH_H

#include "Mojo/Physics/Common/b2Settings.h"
#include <math.h>

/// This function is used to ensure that a floating point number is not a NaN or infinity.
inline bool b2IsValid(float x)
{
	return isfinite(x);
}

#define	b2Sqrt(x)	sqrtf(x)
#define	b2Atan2(y, x)	atan2f(y, x)

/// A 2D column vector.
struct float2
{
	/// Default constructor does nothing (for performance).
	float2() {}

	/// Construct using coordinates.
	float2(float xIn, float yIn) : x(xIn), y(yIn) {}

	/// Set this vector to all zeros.
	void SetZero() { x = 0.0f; y = 0.0f; }

	/// Set this vector to some specified coordinates.
	void Set(float x_, float y_) { x = x_; y = y_; }

	/// Negate this vector.
	float2 operator -() const { float2 v; v.Set(-x, -y); return v; }
	
	/// Read from and indexed element.
	float operator () (int i) const
	{
		return (&x)[i];
	}

	/// Write to an indexed element.
	float& operator () (int i)
	{
		return (&x)[i];
	}

	/// Add a vector to this vector.
	void operator += (const float2& v)
	{
		x += v.x; y += v.y;
	}
	
	/// Subtract a vector from this vector.
	void operator -= (const float2& v)
	{
		x -= v.x; y -= v.y;
	}

	/// Multiply this vector by a scalar.
	void operator *= (float a)
	{
		x *= a; y *= a;
	}

	/// Get the length of this vector (the norm).
	float Length() const
	{
		return b2Sqrt(x * x + y * y);
	}

	/// Get the length squared. For performance, use this instead of
	/// float2::Length (if possible).
	float LengthSquared() const
	{
		return x * x + y * y;
	}

	/// Convert this vector into a unit vector. Returns the length.
	float Normalize()
	{
		float length = Length();
		if (length < b2_epsilon)
		{
			return 0.0f;
		}
		float invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;

		return length;
	}

	/// Does this vector contain finite coordinates?
	bool IsValid() const
	{
		return b2IsValid(x) && b2IsValid(y);
	}

	/// Get the skew vector such that dot(skew_vec, other) == cross(vec, other)
	float2 Skew() const
	{
		return float2(-y, x);
	}

	float x, y;
};

/// A 2D column vector with 3 elements.
struct b2Vec3
{
	/// Default constructor does nothing (for performance).
	b2Vec3() {}

	/// Construct using coordinates.
	b2Vec3(float xIn, float yIn, float zIn) : x(xIn), y(yIn), z(zIn) {}

	/// Set this vector to all zeros.
	void SetZero() { x = 0.0f; y = 0.0f; z = 0.0f; }

	/// Set this vector to some specified coordinates.
	void Set(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }

	/// Negate this vector.
	b2Vec3 operator -() const { b2Vec3 v; v.Set(-x, -y, -z); return v; }

	/// Add a vector to this vector.
	void operator += (const b2Vec3& v)
	{
		x += v.x; y += v.y; z += v.z;
	}

	/// Subtract a vector from this vector.
	void operator -= (const b2Vec3& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
	}

	/// Multiply this vector by a scalar.
	void operator *= (float s)
	{
		x *= s; y *= s; z *= s;
	}

	float x, y, z;
};

/// A 2-by-2 matrix. Stored in column-major order.
struct b2Mat22
{
	/// The default constructor does nothing (for performance).
	b2Mat22() {}

	/// Construct this matrix using columns.
	b2Mat22(const float2& c1, const float2& c2)
	{
		ex = c1;
		ey = c2;
	}

	/// Construct this matrix using scalars.
	b2Mat22(float a11, float a12, float a21, float a22)
	{
		ex.x = a11; ex.y = a21;
		ey.x = a12; ey.y = a22;
	}

	/// Initialize this matrix using columns.
	void Set(const float2& c1, const float2& c2)
	{
		ex = c1;
		ey = c2;
	}

	/// Set this to the identity matrix.
	void SetIdentity()
	{
		ex.x = 1.0f; ey.x = 0.0f;
		ex.y = 0.0f; ey.y = 1.0f;
	}

	/// Set this matrix to all zeros.
	void SetZero()
	{
		ex.x = 0.0f; ey.x = 0.0f;
		ex.y = 0.0f; ey.y = 0.0f;
	}

	b2Mat22 GetInverse() const
	{
		float a = ex.x, b = ey.x, c = ex.y, d = ey.y;
		b2Mat22 B;
		float det = a * d - b * c;
		if (det != 0.0f)
		{
			det = 1.0f / det;
		}
		B.ex.x =  det * d;	B.ey.x = -det * b;
		B.ex.y = -det * c;	B.ey.y =  det * a;
		return B;
	}

	/// Solve A * x = b, where b is a column vector. This is more efficient
	/// than computing the inverse in one-shot cases.
	float2 Solve(const float2& b) const
	{
		float a11 = ex.x, a12 = ey.x, a21 = ex.y, a22 = ey.y;
		float det = a11 * a22 - a12 * a21;
		if (det != 0.0f)
		{
			det = 1.0f / det;
		}
		float2 x;
		x.x = det * (a22 * b.x - a12 * b.y);
		x.y = det * (a11 * b.y - a21 * b.x);
		return x;
	}

	float2 ex, ey;
};

/// A 3-by-3 matrix. Stored in column-major order.
struct b2Mat33
{
	/// The default constructor does nothing (for performance).
	b2Mat33() {}

	/// Construct this matrix using columns.
	b2Mat33(const b2Vec3& c1, const b2Vec3& c2, const b2Vec3& c3)
	{
		ex = c1;
		ey = c2;
		ez = c3;
	}

	/// Set this matrix to all zeros.
	void SetZero()
	{
		ex.SetZero();
		ey.SetZero();
		ez.SetZero();
	}

	/// Solve A * x = b, where b is a column vector. This is more efficient
	/// than computing the inverse in one-shot cases.
	b2Vec3 Solve33(const b2Vec3& b) const;

	/// Solve A * x = b, where b is a column vector. This is more efficient
	/// than computing the inverse in one-shot cases. Solve only the upper
	/// 2-by-2 matrix equation.
	float2 Solve22(const float2& b) const;

	/// Get the inverse of this matrix as a 2-by-2.
	/// Returns the zero matrix if singular.
	void GetInverse22(b2Mat33* M) const;

	/// Get the symmetric inverse of this matrix as a 3-by-3.
	/// Returns the zero matrix if singular.
	void GetSymInverse33(b2Mat33* M) const;

	b2Vec3 ex, ey, ez;
};

/// Rotation
struct b2Rot
{
	b2Rot() {}

	/// Initialize from an angle in radians
	explicit b2Rot(float angle)
	{
		/// TODO_ERIN optimize
		s = sinf(angle);
		c = cosf(angle);
	}

	/// Set using an angle in radians.
	void Set(float angle)
	{
		/// TODO_ERIN optimize
		s = sinf(angle);
		c = cosf(angle);
	}

	/// Set to the identity rotation
	void SetIdentity()
	{
		s = 0.0f;
		c = 1.0f;
	}

	/// Get the angle in radians
	float GetAngle() const
	{
		return b2Atan2(s, c);
	}

	/// Get the x-axis
	float2 GetXAxis() const
	{
		return float2(c, s);
	}

	/// Get the u-axis
	float2 GetYAxis() const
	{
		return float2(-s, c);
	}

	/// Sine and cosine
	float s, c;
};

/// A transform contains translation and rotation. It is used to represent
/// the position and orientation of rigid frames.
struct b2Transform
{
	/// The default constructor does nothing.
	b2Transform() {}

	/// Initialize using a position vector and a rotation.
	b2Transform(const float2& position, const b2Rot& rotation) : p(position), q(rotation) {}

	/// Set this to the identity transform.
	void SetIdentity()
	{
		p.SetZero();
		q.SetIdentity();
	}

	/// Set this based on the position and angle.
	void Set(const float2& position, float angle)
	{
		p = position;
		q.Set(angle);
	}

	float2 p;
	b2Rot q;
};

/// This describes the motion of a body/shape for TOI computation.
/// Shapes are defined with respect to the body origin, which may
/// no coincide with the center of mass. However, to support dynamics
/// we must interpolate the center of mass position.
struct b2Sweep
{
	/// Get the interpolated transform at a specific time.
	/// @param beta is a factor in [0,1], where 0 indicates alpha0.
	void GetTransform(b2Transform* xfb, float beta) const;

	/// Advance the sweep forward, yielding a new initial state.
	/// @param alpha the new initial time.
	void Advance(float alpha);

	/// Normalize the angles.
	void Normalize();

	float2 localCenter;	///< local center of mass position
	float2 c0, c;		///< center world positions
	float a0, a;		///< world angles

	/// Fraction of the current time step in the range [0,1]
	/// c0 and a0 are the positions at alpha0.
	float alpha0;
};

/// Useful constant
extern const float2 b2Vec2_zero;

/// Perform the dot product on two vectors.
inline float b2Dot(const float2& a, const float2& b)
{
	return a.x * b.x + a.y * b.y;
}

/// Perform the cross product on two vectors. In 2D this produces a scalar.
inline float b2Cross(const float2& a, const float2& b)
{
	return a.x * b.y - a.y * b.x;
}

/// Perform the cross product on a vector and a scalar. In 2D this produces
/// a vector.
inline float2 b2Cross(const float2& a, float s)
{
	return float2(s * a.y, -s * a.x);
}

/// Perform the cross product on a scalar and a vector. In 2D this produces
/// a vector.
inline float2 b2Cross(float s, const float2& a)
{
	return float2(-s * a.y, s * a.x);
}

/// Multiply a matrix times a vector. If a rotation matrix is provided,
/// then this transforms the vector from one frame to another.
inline float2 b2Mul(const b2Mat22& A, const float2& v)
{
	return float2(A.ex.x * v.x + A.ey.x * v.y, A.ex.y * v.x + A.ey.y * v.y);
}

/// Multiply a matrix transpose times a vector. If a rotation matrix is provided,
/// then this transforms the vector from one frame to another (inverse transform).
inline float2 b2MulT(const b2Mat22& A, const float2& v)
{
	return float2(b2Dot(v, A.ex), b2Dot(v, A.ey));
}

/// Add two vectors component-wise.
inline float2 operator + (const float2& a, const float2& b)
{
	return float2(a.x + b.x, a.y + b.y);
}

/// Subtract two vectors component-wise.
inline float2 operator - (const float2& a, const float2& b)
{
	return float2(a.x - b.x, a.y - b.y);
}

inline float2 operator * (float s, const float2& a)
{
	return float2(s * a.x, s * a.y);
}

inline bool operator == (const float2& a, const float2& b)
{
	return a.x == b.x && a.y == b.y;
}

inline bool operator != (const float2& a, const float2& b)
{
	return a.x != b.x || a.y != b.y;
}

inline float b2Distance(const float2& a, const float2& b)
{
	float2 c = a - b;
	return c.Length();
}

inline float b2DistanceSquared(const float2& a, const float2& b)
{
	float2 c = a - b;
	return b2Dot(c, c);
}

inline b2Vec3 operator * (float s, const b2Vec3& a)
{
	return b2Vec3(s * a.x, s * a.y, s * a.z);
}

/// Add two vectors component-wise.
inline b2Vec3 operator + (const b2Vec3& a, const b2Vec3& b)
{
	return b2Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

/// Subtract two vectors component-wise.
inline b2Vec3 operator - (const b2Vec3& a, const b2Vec3& b)
{
	return b2Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

/// Perform the dot product on two vectors.
inline float b2Dot(const b2Vec3& a, const b2Vec3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

/// Perform the cross product on two vectors.
inline b2Vec3 b2Cross(const b2Vec3& a, const b2Vec3& b)
{
	return b2Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline b2Mat22 operator + (const b2Mat22& A, const b2Mat22& B)
{
	return b2Mat22(A.ex + B.ex, A.ey + B.ey);
}

// A * B
inline b2Mat22 b2Mul(const b2Mat22& A, const b2Mat22& B)
{
	return b2Mat22(b2Mul(A, B.ex), b2Mul(A, B.ey));
}

// A^T * B
inline b2Mat22 b2MulT(const b2Mat22& A, const b2Mat22& B)
{
	float2 c1(b2Dot(A.ex, B.ex), b2Dot(A.ey, B.ex));
	float2 c2(b2Dot(A.ex, B.ey), b2Dot(A.ey, B.ey));
	return b2Mat22(c1, c2);
}

/// Multiply a matrix times a vector.
inline b2Vec3 b2Mul(const b2Mat33& A, const b2Vec3& v)
{
	return v.x * A.ex + v.y * A.ey + v.z * A.ez;
}

/// Multiply a matrix times a vector.
inline float2 b2Mul22(const b2Mat33& A, const float2& v)
{
	return float2(A.ex.x * v.x + A.ey.x * v.y, A.ex.y * v.x + A.ey.y * v.y);
}

/// Multiply two rotations: q * r
inline b2Rot b2Mul(const b2Rot& q, const b2Rot& r)
{
	// [qc -qs] * [rc -rs] = [qc*rc-qs*rs -qc*rs-qs*rc]
	// [qs  qc]   [rs  rc]   [qs*rc+qc*rs -qs*rs+qc*rc]
	// s = qs * rc + qc * rs
	// c = qc * rc - qs * rs
	b2Rot qr;
	qr.s = q.s * r.c + q.c * r.s;
	qr.c = q.c * r.c - q.s * r.s;
	return qr;
}

/// Transpose multiply two rotations: qT * r
inline b2Rot b2MulT(const b2Rot& q, const b2Rot& r)
{
	// [ qc qs] * [rc -rs] = [qc*rc+qs*rs -qc*rs+qs*rc]
	// [-qs qc]   [rs  rc]   [-qs*rc+qc*rs qs*rs+qc*rc]
	// s = qc * rs - qs * rc
	// c = qc * rc + qs * rs
	b2Rot qr;
	qr.s = q.c * r.s - q.s * r.c;
	qr.c = q.c * r.c + q.s * r.s;
	return qr;
}

/// Rotate a vector
inline float2 b2Mul(const b2Rot& q, const float2& v)
{
	return float2(q.c * v.x - q.s * v.y, q.s * v.x + q.c * v.y);
}

/// Inverse rotate a vector
inline float2 b2MulT(const b2Rot& q, const float2& v)
{
	return float2(q.c * v.x + q.s * v.y, -q.s * v.x + q.c * v.y);
}

inline float2 b2Mul(const b2Transform& T, const float2& v)
{
	float x = (T.q.c * v.x - T.q.s * v.y) + T.p.x;
	float y = (T.q.s * v.x + T.q.c * v.y) + T.p.y;

	return float2(x, y);
}

inline float2 b2MulT(const b2Transform& T, const float2& v)
{
	float px = v.x - T.p.x;
	float py = v.y - T.p.y;
	float x = (T.q.c * px + T.q.s * py);
	float y = (-T.q.s * px + T.q.c * py);

	return float2(x, y);
}

// v2 = A.q.Rot(B.q.Rot(v1) + B.p) + A.p
//    = (A.q * B.q).Rot(v1) + A.q.Rot(B.p) + A.p
inline b2Transform b2Mul(const b2Transform& A, const b2Transform& B)
{
	b2Transform C;
	C.q = b2Mul(A.q, B.q);
	C.p = b2Mul(A.q, B.p) + A.p;
	return C;
}

// v2 = A.q' * (B.q * v1 + B.p - A.p)
//    = A.q' * B.q * v1 + A.q' * (B.p - A.p)
inline b2Transform b2MulT(const b2Transform& A, const b2Transform& B)
{
	b2Transform C;
	C.q = b2MulT(A.q, B.q);
	C.p = b2MulT(A.q, B.p - A.p);
	return C;
}

template <typename T>
inline T b2Abs(T a)
{
	return a > T(0) ? a : -a;
}

inline float2 b2Abs(const float2& a)
{
	return float2(b2Abs(a.x), b2Abs(a.y));
}

inline b2Mat22 b2Abs(const b2Mat22& A)
{
	return b2Mat22(b2Abs(A.ex), b2Abs(A.ey));
}

template <typename T>
inline T b2Min(T a, T b)
{
	return a < b ? a : b;
}

inline float2 b2Min(const float2& a, const float2& b)
{
	return float2(b2Min(a.x, b.x), b2Min(a.y, b.y));
}

template <typename T>
inline T b2Max(T a, T b)
{
	return a > b ? a : b;
}

inline float2 b2Max(const float2& a, const float2& b)
{
	return float2(b2Max(a.x, b.x), b2Max(a.y, b.y));
}

template <typename T>
inline T b2Clamp(T a, T low, T high)
{
	return b2Max(low, b2Min(a, high));
}

inline float2 b2Clamp(const float2& a, const float2& low, const float2& high)
{
	return b2Max(low, b2Min(a, high));
}

template<typename T> inline void b2Swap(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

/// "Next Largest Power of 2
/// Given a binary integer value x, the next largest power of 2 can be computed by a SWAR algorithm
/// that recursively "folds" the upper bits into the lower bits. This process yields a bit vector with
/// the same most significant 1 as x, but all 1's below it. Adding 1 to that value yields the next
/// largest power of 2. For a 32-bit value:"
inline uint32 b2NextPowerOfTwo(uint32 x)
{
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	return x + 1;
}

inline bool b2IsPowerOfTwo(uint32 x)
{
	bool result = x > 0 && (x & (x - 1)) == 0;
	return result;
}

inline void b2Sweep::GetTransform(b2Transform* xf, float beta) const
{
	xf->p = (1.0f - beta) * c0 + beta * c;
	float angle = (1.0f - beta) * a0 + beta * a;
	xf->q.Set(angle);

	// Shift to origin
	xf->p -= b2Mul(xf->q, localCenter);
}

inline void b2Sweep::Advance(float alpha)
{
	b2Assert(alpha0 < 1.0f);
	float beta = (alpha - alpha0) / (1.0f - alpha0);
	c0 += beta * (c - c0);
	a0 += beta * (a - a0);
	alpha0 = alpha;
}

/// Normalize an angle in radians to be between -pi and pi
inline void b2Sweep::Normalize()
{
	float twoPi = 2.0f * b2_pi;
	float d =  twoPi * floorf(a0 / twoPi);
	a0 -= d;
	a -= d;
}

#endif
