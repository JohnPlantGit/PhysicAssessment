#pragma once
#include "Rigidbody.h"
class Circle : public Rigidbody
{
public:
	Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
	~Circle();

	virtual void MakeGizmo();
	virtual bool CheckCollision(PhysicsObject* other);

	float GetRadius() { return m_radius; }
	glm::vec4 GetColour() { return m_colour; }

protected:
	float m_radius;
	glm::vec4 m_colour;
};

