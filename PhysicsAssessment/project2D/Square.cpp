#include "Square.h"
#include <Gizmos.h>

Square::Square(glm::vec2 position, glm::vec2 velocity, float mass, float height, float width, glm::vec4 colour) : Rigidbody(SQUARE, position, velocity, mass, 0)
{
	m_min = glm::vec2(-width / 2, -height / 2);
	m_max = glm::vec2(width / 2, height / 2);
	m_colour = colour;
}

Square::~Square()
{
}

void Square::MakeGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_position, m_max, m_colour);
}

bool Square::CheckCollision(PhysicsObject* other)
{
	return true;
}

void Square::ResolveCollision(Rigidbody* other)
{
	if (other->GetShape() == CIRCLE)
	{
		glm::vec2 normal = glm::normalize(other->GetPosition() - m_position);
		glm::vec2 relativeVelocity = other->GetVelocity() - m_velocity;

		float elasticity = 1;
		float j = glm::dot(-(1 + elasticity) * relativeVelocity, normal) / glm::dot(normal, normal * (1 / m_mass + 1 / other->GetMass()));

		glm::vec2 force = normal * j;

		ApplyForceToActor(other, force);
	}
}