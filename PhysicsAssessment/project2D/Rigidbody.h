#pragma once
#include "PhysicsObject.h"
#include <glm/ext.hpp>
#include "CollisionArgs.h"

#define MIN_LINEAR_THRESHOLD 0.1
#define MIN_ROTATION_THRESHOLD 0.01

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
	virtual void ResolveCollision(Rigidbody* other, CollisionArgs cArgs) = 0;

	glm::vec2 GetPosition() { return m_position; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }
	float GetRotation() { return m_rotation; }

	void SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void SetPosition(glm::vec2 position) { m_position = position; }

	float GetLinearDrag() { return m_linearDrag; }
	void SetLinearDrag(float linearDrag) { m_linearDrag = linearDrag; }
	float GetAngularDrag() { return m_angularDrag; }
	void SetAngularDrag(float angularDrag) { m_angularDrag = angularDrag; }
	float GetElasticity() { return m_elasticity; }
	void SetElasticity(float elasticity) { m_elasticity = elasticity; }
	bool GetKinematic() { return m_kinematic; }
	void SetKinematic(bool kinematic) { m_kinematic = kinematic; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_rotation;
	float m_angularVelocity;
	float m_mass;
	float m_linearDrag = 0.0f;
	float m_angularDrag = 0.0f;
	float m_elasticity = 1.0f;
	bool m_kinematic = false;
};

