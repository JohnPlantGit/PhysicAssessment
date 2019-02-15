#include "Square.h"
#include <Gizmos.h>
#include <glm/gtx/transform.hpp>

Square::Square(glm::vec2 position, glm::vec2 velocity, float mass, float height, float width, glm::vec4 colour) : Rigidbody(SQUARE, position, velocity, mass, 0)
{
	m_min = glm::vec2(-width / 2, -height / 2);
	m_max = glm::vec2(width / 2, height / 2);
	m_colour = colour;
	m_moment = 1.0f / 12.0f * m_mass * width * height;
}

Square::~Square()
{
}

void Square::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::FixedUpdate(gravity, timeStep);

	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);

	m_rotationMatrix[0] = glm::vec2(cs, sn);
	m_rotationMatrix[1] = glm::vec2(-sn, cs);
}

void Square::MakeGizmo()
{
	glm::mat4 transform = glm::rotate(m_rotation, glm::vec3(0, 0, 1));

	aie::Gizmos::add2DAABBFilled(m_position, m_max, m_colour, &transform);
}

bool Square::CheckCollision(PhysicsObject* other)
{
	return true;
}

void Square::ResolveCollision(Rigidbody* other, CollisionArgs cArgs)
{
	if (other->GetShape() == CIRCLE)
	{
		glm::vec2 normal = cArgs.m_collisionNormal;
		glm::vec2 relativeVelocity = other->GetVelocity() - m_velocity;

		float elasticity = (m_elasticity + other->GetElasticity()) / 2;
		float j = glm::dot(-(1 + elasticity) * relativeVelocity, normal) / glm::dot(normal, normal * ((m_kinematic ? 0 : (1 / m_mass)) + 1 / other->GetMass()));

		glm::vec2 force = normal * j;

		ApplyForceToActor(other, force, other->GetPosition());
	}
	else if (other->GetShape() == SQUARE)
	{
		glm::vec2 normal = cArgs.m_collisionNormal;
		glm::vec2 relativeVelocity = other->GetVelocity() - m_velocity;

		float elasticity = (m_elasticity + other->GetElasticity()) / 2;
		float j = glm::dot(-(1 + elasticity) * relativeVelocity, normal) / glm::dot(normal, normal * ((m_kinematic ? 0 : (1 / m_mass)) + 1 / other->GetMass()));

		glm::vec2 force = normal * j;

		ApplyForceToActor(other, force, other->GetPosition());
	}
}