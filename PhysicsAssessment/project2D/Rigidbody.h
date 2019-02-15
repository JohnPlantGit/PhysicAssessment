#pragma once
#include "PhysicsObject.h"
#include <glm/ext.hpp>
#include "CollisionArgs.h"

#define MIN_LINEAR_THRESHOLD 0.01
#define MIN_ROTATION_THRESHOLD 0.01

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shape, glm::vec2 position, glm::vec2 velocity, float mass, float rotation);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Debug();

	void ApplyForce(glm::vec2 force, glm::vec2 pos);
	void ApplyForceToActor(Rigidbody* other, glm::vec2 force, glm::vec2 pos);

	virtual bool CheckCollision(PhysicsObject* other) = 0;
	virtual void ResolveCollision(Rigidbody* other, CollisionArgs cArgs);

	glm::vec2 GetPosition() { return m_position; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetAngularVelocity() { return m_angularVelocity; }
	float GetMass() { return m_mass; }
	float GetRotation() { return m_rotation; }

	void SetPosition(glm::vec2 position) { m_position = position; }
	void SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void SetAngularVelocity(float angularVelocity) { m_angularVelocity = angularVelocity; }

	float GetLinearDrag() { return m_linearDrag; }
	void SetLinearDrag(float linearDrag) { m_linearDrag = linearDrag; }
	float GetAngularDrag() { return m_angularDrag; }
	void SetAngularDrag(float angularDrag) { m_angularDrag = angularDrag; }
	float GetElasticity() { return m_elasticity; }
	void SetElasticity(float elasticity) { m_elasticity = elasticity; }
	bool GetKinematic() { return m_kinematic; }
	void SetKinematic(bool kinematic) { m_kinematic = kinematic; }
	bool GetStationary() { return m_stationary; }
	float GetStaticFriction() { return m_staticFriction; }
	void SetStaticFriction(float staticFriction) { m_staticFriction = staticFriction; }
	float GetDynamicFriction() { return m_dynamicFriction; }
	void SetDynamicFriction(float dynamicFriction) { m_dynamicFriction = dynamicFriction; }

	float GetMoment() { return m_moment; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_rotation = 0.0f;
	float m_angularVelocity = 0.0f;
	float m_mass;
	float m_linearDrag = 0;
	float m_angularDrag = 1;
	float m_elasticity = 0.5;
	bool m_kinematic = false;
	bool m_stationary;
	float m_staticFriction = 1.0f;
	float m_dynamicFriction = 0.01;
	float m_moment;
};

