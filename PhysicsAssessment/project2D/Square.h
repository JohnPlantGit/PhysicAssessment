#pragma once
#include "Rigidbody.h"
class Square : public Rigidbody
{
public:
	Square(glm::vec2 position, glm::vec2 velocity, float mass, float height, float width, glm::vec4 colour);
	~Square();

	virtual void MakeGizmo();
	virtual bool CheckCollision(PhysicsObject* other);
	virtual void ResolveCollision(Rigidbody* other);

	glm::vec2 GetMin() { return m_min; }
	glm::vec2 GetMax() { return m_max; }

protected:
	glm::vec2 m_min;
	glm::vec2 m_max;

	glm::vec4 m_colour;
};
