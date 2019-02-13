#include "Rigidbody.h"

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

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (!m_kinematic)
	{
		ApplyForce(gravity * m_mass * timeStep);

		m_velocity -= m_velocity * m_linearDrag * timeStep;
		m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

		if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		{
			m_velocity = glm::vec2(0, 0);
		}

		//m_rotation += m_angularVelocity * timeStep;
		m_position += m_velocity * timeStep;
	}
}

void Rigidbody::Debug()
{

}

void Rigidbody::ApplyForce(glm::vec2 force)
{
	if (!m_kinematic)
		m_velocity += (force / m_mass);
}

void Rigidbody::ApplyForceToActor(Rigidbody* other, glm::vec2 force)
{
	other->ApplyForce(force);
	ApplyForce(-force);
}
