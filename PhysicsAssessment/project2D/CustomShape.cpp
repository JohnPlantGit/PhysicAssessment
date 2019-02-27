#include "CustomShape.h"
#include <Gizmos.h>
#include <glm/gtx/transform.hpp>

CustomShape::CustomShape(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec4 colour) : Rigidbody(CUSTOM, position, velocity, mass, rotation)
{
	m_colour = colour;
}

CustomShape::CustomShape(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec4 colour, std::vector<glm::vec2> points) : Rigidbody(CUSTOM, position, velocity, mass, rotation)
{
	m_points = points;
	m_colour = colour;
}

CustomShape::CustomShape(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec4 colour, glm::vec2 points[], int pointCount) : Rigidbody(CUSTOM, position, velocity, mass, rotation)
{
	for (int i = 0; i < pointCount; i++)
	{
		m_points.push_back(points[i]);
	}
	m_colour = colour;
}

CustomShape::~CustomShape()
{
}

void CustomShape::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::FixedUpdate(gravity, timeStep);

	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);

	m_rotationMatrix[0] = glm::vec3(cs, sn, 0);
	m_rotationMatrix[1] = glm::vec3(-sn, cs, 0);
	m_rotationMatrix[2] = glm::vec3(0, 0, 1);

	UpdateMinMax();
}

void CustomShape::MakeGizmo()
{
	std::vector<glm::vec2> points = GetPoints();

	for (int i = 0; i < points.size(); i++)
	{
		aie::Gizmos::add2DCircle(points[i], 1, 12, m_colour);
		aie::Gizmos::add2DLine(points[i], points[(i + 1 < points.size() ? i + 1 : 0)], m_colour);
	}
}

void CustomShape::ResolveCollision(Rigidbody* other, CollisionArgs cArgs)
{
	glm::vec2 normal = cArgs.m_collisionNormal;
	glm::vec2 relativeVelocity = other->GetVelocity() - m_velocity;

	float elasticity = (m_elasticity + other->GetElasticity()) / 2;
	float j = glm::dot(-(1 + elasticity) * relativeVelocity, normal) / glm::dot(normal, normal * ((m_kinematic ? 0 : (1 / m_mass)) + 1 / other->GetMass()));

	glm::vec2 force = normal * j;

	ApplyForceToActor(other, force, glm::vec2(0, 0));
}

void CustomShape::UpdateMinMax()
{
	glm::mat3x3 rotMatrix = GetRotationMaxtrix();

	std::vector <glm::vec2> points = m_points;
	for (int i = 0; i < points.size(); i++)
	{
		points[i] = (m_rotationMatrix * glm::vec3(points[i].x, points[i].y, 0));
		points[i] += m_position;
	}

	m_min = points[0];
	m_max = points[0];

	for (int i = 1; i < points.size(); i++)
	{
		if (points[i].x < m_min.x)
		{
			m_min.x = points[i].x;
		}
		if (points[i].y < m_min.y)
		{
			m_min.y = points[i].y;
		}
		if (points[i].x > m_max.x)
		{
			m_max.x = points[i].x;
		}
		if (points[i].y > m_max.y)
		{
			m_max.y = points[i].y;
		}
	}
}

std::vector<glm::vec2> CustomShape::GetPoints()
{
	std::vector<glm::vec2> points = m_points;
	for (int i = 0; i < points.size(); i++)
	{
		points[i] = (m_rotationMatrix * glm::vec3(points[i].x, points[i].y, 0));
		points[i] += m_position;
	}

	return points;
}

