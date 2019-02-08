#pragma once
#include "PhysicsObject.h"
#include <glm/ext.hpp>

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shape, glm::vec2 position, glm::vec2 velocity, float mass, float rotation);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Debug();

	void ApplyForce(glm::vec2 force);
	void ApplyForceToActor(Rigidbody* other, glm::vec2 force);

	virtual bool CheckCollision(PhysicsObject* other) = 0;
	virtual void ResolveCollision(Rigidbody* other) = 0;

	glm::vec2 GetPosition() { return m_position; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }
	float GetRotation() { return m_rotation; }

	void SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void SetPosition(glm::vec2 position) { m_position = position; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_rotation;
};

