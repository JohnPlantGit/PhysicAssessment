#pragma once
#include "PhysicsObject.h"

class Rigidbody;

class Line : public PhysicsObject
{
public:
	Line();
	Line(glm::vec2 normal, float distance);
	~Line();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Debug();
	virtual void MakeGizmo();

	void ResolveCollision(Rigidbody* other);

	glm::vec2 GetNormal() { return m_normal; }
	float GetDistance() { return m_distanceToOrigin; }

protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
};

