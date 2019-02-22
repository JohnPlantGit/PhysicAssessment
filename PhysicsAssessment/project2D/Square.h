#pragma once
#include "Rigidbody.h"
class Square : public Rigidbody
{
public:
	Square(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, float width, float height, glm::vec4 colour);
	~Square();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void MakeGizmo();
	virtual void ResolveCollision(Rigidbody* other, CollisionArgs cArgs);

	glm::vec2 GetMin() { return m_min; }
	glm::vec2 GetMax() { return m_max; }
	glm::vec2 GetMinRelative() { return -m_extents; }
	glm::vec2 GetMaxRelative() { return m_extents; }

	glm::mat3x3 GetRotationMaxtrix() { return m_rotationMatrix; }

protected:

	void UpdateMinMax();

	glm::vec2 m_min;
	glm::vec2 m_max;

	glm::vec2 m_extents;
	glm::vec4 m_colour;

	glm::mat3x3 m_rotationMatrix;
};

