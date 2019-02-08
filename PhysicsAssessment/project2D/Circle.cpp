#include "Circle.h"
#include <Gizmos.h>

Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : Rigidbody(CIRCLE, position, velocity, mass, 0)
{
	m_radius = radius;
	m_colour = colour;
}

Circle::~Circle()
{
}

void Circle::MakeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 20, m_colour);
}

bool Circle::CheckCollision(PhysicsObject* other)
{
	if (other->GetShape() == CIRCLE)
	{
		Circle* castOther = dynamic_cast<Circle*>(other);
		int length = glm::length(castOther->GetPosition() - GetPosition());
		if (length < (castOther->GetRadius() + m_radius))
		{
			return true;
		}
	}

	return false;
}

void Circle::ResolveCollision(Rigidbody* other)
{
	if (other->GetShape())
	{
		glm::vec2 normal = glm::normalize(other->GetPosition() - m_position);
		glm::vec2 relativeVelocity = other->GetVelocity() - m_velocity;

		float elasticity = 1;
		float j = glm::dot(-(1 + elasticity) * relativeVelocity, normal) / glm::dot(normal, normal * (1 / m_mass + 1 / other->GetMass()));

		glm::vec2 force = normal * j;

		ApplyForceToActor(other, force);
	}
}
