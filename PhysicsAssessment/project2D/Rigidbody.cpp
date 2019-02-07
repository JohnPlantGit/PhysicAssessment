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
	ApplyForce(gravity * m_mass * timeStep);
	m_position += m_velocity * timeStep;
}

void Rigidbody::Debug()
{

}

void Rigidbody::ApplyForce(glm::vec2 force)
{
	m_velocity += (force / m_mass);
}

void Rigidbody::ApplyForceToActor(Rigidbody* other, glm::vec2 force)
{
	other->ApplyForce(force);
	ApplyForce(-force);
}
