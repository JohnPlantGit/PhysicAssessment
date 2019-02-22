#include "Rigidbody.h"
#include <stdio.h>

// Initializes the rigidbody variables to the constructor parameters
// passes the shape into the physicsObject constructor
Rigidbody::Rigidbody(ShapeType shape, glm::vec2 position, glm::vec2 velocity, float mass, float rotation) : PhysicsObject(shape)
{
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_rotation = rotation;
}

Rigidbody::~Rigidbody()
{
}

// Updates the angular and linear velocity, the position and rotation and applies gravity to the rigid body 
void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (!m_kinematic)
	{
		ApplyForce(gravity * m_mass * timeStep, glm::vec2(0, 0)); // apply the gravity to the center of mass

		// slows down the velocity of the object acording to a portion of the current velocity defined by the drag variables
		m_velocity -= m_velocity * m_linearDrag * timeStep;
		m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

		// when the magnitude of the velocity reaches the defined minimum set the velocity to 0
		if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		{
			m_velocity = glm::vec2(0, 0);
			m_stationary = true;
		}
		else
		{
			m_stationary = false;
		}

		if (fabsf(m_angularVelocity) < MIN_ROTATION_THRESHOLD)
		{
			m_angularVelocity = 0;
		}

		// move and rotate the rigidbody according to the current angular and linear velocity
		m_rotation += m_angularVelocity * timeStep;
		m_position += m_velocity * timeStep;

		//printf("(%f, %f)\n", m_velocity.x, m_velocity.y);
	}
}

void Rigidbody::Debug()
{

}

// Adds the instantanious acceleration to the velocity according to the equation F = ma
void Rigidbody::ApplyForce(glm::vec2 force, glm::vec2 pos)
{
	if (!m_kinematic)
	{
		m_velocity += (force / m_mass);

		m_angularVelocity += (((force.y * pos.x) - (force.x * pos.y)) / m_moment);
	}
}

// calls the apply force function on the other actor using the force param
// calls the apply force function on the current actor using the opposite force
void Rigidbody::ApplyForceToActor(Rigidbody* other, glm::vec2 force, glm::vec2 pos)
{
	other->ApplyForce(force, pos);
	ApplyForce(-force, pos);
}

void Rigidbody::ResolveCollision(Rigidbody* other, CollisionArgs cArgs)
{
	glm::vec2 normal = cArgs.m_collisionNormal;
	glm::vec2 perp(normal.y, -normal.x);
	glm::vec2 relativeVelocity = other->GetVelocity() - m_velocity;

	float r1 = glm::dot(cArgs.m_contactPoint - m_position, -perp);
	float r2 = glm::dot(cArgs.m_contactPoint - other->GetPosition(), perp);
	
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = glm::dot(other->GetVelocity(), normal) + r2 * other->GetAngularVelocity();

	if (true)//v1 > v2)
	{
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / other->GetMass() + (r2 * r2) / other->GetMoment());
	
		float elasticity = (m_elasticity + other->GetElasticity()) / 2.0f;
		//float j = glm::dot(-(1 + elasticity) * relativeVelocity, normal) / glm::dot(normal, normal * ((m_kinematic ? 0 : (1 / m_mass)) + 1 / other->GetMass()));

		//glm::vec2 force = normal * j;
		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		//ApplyForceToActor(other, force);
		ApplyForce(-force, cArgs.m_contactPoint - m_position);
		other->ApplyForce(force, cArgs.m_contactPoint - other->GetPosition());
	}

}
