#pragma once
#include "PhysicsObject.h"

class Rigidbody;

class Spring : public PhysicsObject
{
public:
	Spring(Rigidbody* rb1, Rigidbody* rb2, float restLength, float springCoefficient, float damping = 0,
		glm::vec2 contact1 = glm::vec2(0, 0), glm::vec2 contact2 = glm::vec2(0, 0));
	~Spring();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Debug() {}
	virtual void MakeGizmo();

	void SetContact2(glm::vec2 pos) { m_contact2 = pos; }

protected:
	Rigidbody* m_body1;
	Rigidbody* m_body2;

	glm::vec2 m_contact1;
	glm::vec2 m_contact2;

	float m_damping;
	float m_restLength;
	float m_springCoefficient;
};

