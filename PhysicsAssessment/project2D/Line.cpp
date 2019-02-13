#include "Line.h"
#include <Gizmos.h>
#include "Rigidbody.h"

Line::Line() : PhysicsObject(LINE)
{
	m_distanceToOrigin = 0;
	m_normal = glm::vec2(0, 1);
}

Line::Line(glm::vec2 normal, float distance) : PhysicsObject(LINE)
{
	m_normal = normal;
	m_distanceToOrigin = distance;
}

Line::~Line()
{
}

void Line::FixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void Line::Debug()
{

}

void Line::MakeGizmo()
{
	float lineLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;	
	glm::vec2 parallel(m_normal.y, -m_normal.x);

	glm::vec2 start = centerPoint + (parallel * lineLength);
	glm::vec2 end = centerPoint - (parallel * lineLength);

	glm::vec4 colour(1, 1, 1, 1);

	aie::Gizmos::add2DLine(start, end, colour);
}

void Line::ResolveCollision(Rigidbody* other, CollisionArgs cArgs)
{
	if (other->GetShape() == CIRCLE)
	{
		float elasticity = other->GetElasticity();
		float j = glm::dot(-(1 + elasticity) * other->GetVelocity(), m_normal) / (1 / other->GetMass());

		glm::vec2 force = m_normal * j;

		other->ApplyForce(force);
	}
	else if (other->GetShape() == SQUARE)
	{
		float elasticity = other->GetElasticity();
		float j = glm::dot(-(1 + elasticity) * other->GetVelocity(), m_normal) / (1 / other->GetMass());

		glm::vec2 force = m_normal * j;

		other->ApplyForce(force);
	}
}
