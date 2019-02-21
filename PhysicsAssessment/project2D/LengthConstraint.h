#pragma once
#include "PhysicsObject.h"

class Rigidbody;

class LengthConstraint
{
public:
	LengthConstraint();
	LengthConstraint(Rigidbody* body1, Rigidbody* body2, float length);
	~LengthConstraint();

	void SatisfyConstraint();

protected:
	Rigidbody* m_body1;
	Rigidbody* m_body2;
	float m_length;
	float m_stiffness = 0.01;
};

