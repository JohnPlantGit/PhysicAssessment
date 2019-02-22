#include "Square.h"
#include <Gizmos.h>
#include <glm/gtx/transform.hpp>

Square::Square(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, float height, float width, glm::vec4 colour) : Rigidbody(SQUARE, position, velocity, mass, rotation)
{
	m_min = glm::vec2(-width / 2, -height / 2);
	m_max = glm::vec2(width / 2, height / 2);
	m_extents = glm::vec2(width / 2, height / 2);
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

	m_rotationMatrix[0] = glm::vec3(cs, sn, 0);
	m_rotationMatrix[1] = glm::vec3(-sn, cs, 0);
	m_rotationMatrix[2] = glm::vec3(0, 0, 1);

	UpdateMinMax();
}

void Square::MakeGizmo()
{
	glm::mat4 transform = glm::rotate(m_rotation, glm::vec3(0, 0, 1));

	aie::Gizmos::add2DAABBFilled(m_position, m_extents, m_colour, &transform);

	glm::vec2 xAxis = glm::vec2(cosf(m_rotation), sinf(m_rotation)) * (m_extents.x);
	aie::Gizmos::add2DLine(m_position, m_position + xAxis, glm::vec4(1, 0, 0, 1));
	glm::vec2 yAxis = glm::vec2(-sinf(m_rotation), cosf(m_rotation)) * (m_extents.y);
	aie::Gizmos::add2DLine(m_position, m_position + yAxis, glm::vec4(0, 1, 0, 1));
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

		ApplyForceToActor(other, force, glm::vec2(0,0));
	}
}

void Square::UpdateMinMax()
{
	glm::vec2 squareMin = GetMinRelative();
	glm::vec2 squareMax = GetMaxRelative();

	glm::mat3x3 rotMatrix = GetRotationMaxtrix();

	glm::vec2 squareCorners[4] =
	{
		(rotMatrix * glm::vec3(squareMin.x, squareMin.y, 0)),
		(rotMatrix * glm::vec3(squareMin.x, squareMax.y, 0)),
		(rotMatrix * glm::vec3(squareMax.x, squareMax.y ,0)),
		(rotMatrix * glm::vec3(squareMax.x, squareMin.y, 0)),
	};
	for (int i = 0; i < 4; i++)
	{
		squareCorners[i] += m_position;
	}

	m_min = squareCorners[0];
	m_max = squareCorners[0];

	for (int i = 1; i < 4; i++)
	{
		if (squareCorners[i].x < m_min.x)
		{
			m_min.x = squareCorners[i].x;
		}
		if (squareCorners[i].y < m_min.y)
		{
			m_min.y = squareCorners[i].y;
		}
		if (squareCorners[i].x > m_max.x)
		{
			m_max.x = squareCorners[i].x;
		}
		if (squareCorners[i].y > m_max.y)
		{
			m_max.y = squareCorners[i].y;
		}
	}
}