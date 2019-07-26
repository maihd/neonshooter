#pragma once

#include "Mojo/Physics/Common/Math.h"

class b2Draw;

/// 
struct RopeDef
{
	///
	float2* vertices;

	///
	int 	count;

	///
	float* 	masses;

	///
	float2 	gravity;

	///
	float 	damping;

	/// Stretching stiffness
	float 	k2;

	/// Bending stiffness. Values above 0.5 can make the simulation blow up.
	float 	k3;

	RopeDef()
	{
		vertices 	= 0;
		count 		= 0;
		masses 		= 0;
		gravity 	= float2(0.0f);
		damping 	= 0.1f;
		k2 			= 0.9f;
		k3 			= 0.1f;
	}
};

/// 
class Rope
{
public:
	Rope();
	~Rope();

	///
	void Initialize(const RopeDef* def);

	///
	void Step(float timeStep, int iterations);

	///
	int GetVertexCount() const
	{
		return m_count;
	}

	///
	const float2* GetVertices() const
	{
		return m_ps;
	}

	///
	void Draw(b2Draw* draw) const;

	///
	void SetAngle(float angle);

private:
	void SolveC2();
	void SolveC3();

	int	 	m_count;
	float2* m_ps;
	float2* m_p0s;
	float2* m_vs;

	float* 	m_ims;

	float* 	m_Ls;
	float* 	m_as;

	float2 	m_gravity;
	float 	m_damping;

	float 	m_k2;
	float 	m_k3;
};
