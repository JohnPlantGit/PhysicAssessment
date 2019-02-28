#include "Circle.h"
#include <Gizmos.h>
#include <iostream>

Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : Rigidbody(CIRCLE, position, velocity, mass, 0)
{
	m_radius = radius;
	m_colour = colour;
	m_moment = 0.5f * m_mass * m_radius * m_radius;
}

Circle::~Circle()
{
}

void Circle::MakeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 20, m_colour);

	glm::vec2 edge = glm::vec2(cosf(m_rotation), sinf(m_rotation)) * m_radius;
	aie::Gizmos::add2DLine(m_position, m_position + edge, glm::vec4(0, 0, 0, 1));
}

void Circle::ResolveCollision(Rigidbody* other, CollisionArgs cArgs)
{
	if (other->GetShape() == CIRCLE)
	{
		//if (glm::dot(m_velocity, other->GetVelocity()) <= 0 || !glm::all(glm::equal(m_velocity, glm::vec2(0,0))) || !glm::all(glm::equal(other->GetVelocity(), glm::vec2(0, 0))))
		{
			glm::vec2 normal = -cArgs.m_collisionNormal;
			glm::vec2 relativeVelocity = other->GetVelocity() - m_velocity;

			float elasticity = (m_elasticity + other->GetElasticity()) / 2;
			float j = glm::dot(-(1 + elasticity) * relativeVelocity, normal) / glm::dot(normal, normal * ((m_kinematic ? 0 : (1 / m_mass)) + (1 / other->GetMass())));

			glm::vec2 force = normal * j;

			ApplyForceToActor(other, force, glm::vec2(0, 0));
		}

	}
}
