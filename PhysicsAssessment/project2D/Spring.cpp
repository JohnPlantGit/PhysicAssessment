#include "Spring.h"
#include "Rigidbody.h"
#include <Gizmos.h>

Spring::Spring(Rigidbody* rb1, Rigidbody* rb2, float restLength, float springCoefficient, float damping, glm::vec2 contact1, glm::vec2 contact2) : PhysicsObject(SPRING)
{
	m_body1 = rb1;
	m_body2 = rb2;
	m_restLength = restLength;
	m_springCoefficient = springCoefficient;
	m_damping = damping;
	m_contact1 = contact1;
	m_contact2 = contact2;
}

Spring::~Spring()
{
}

void Spring::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	glm::vec2 point1 = m_body1->GetPosition() + m_contact1;
	glm::vec2 point2 = m_body2 != nullptr ? m_body2->GetPosition() : glm::vec2(0, 0) + m_contact2;
	glm::vec2 distance = point2 - point1;
	float length = glm::length(distance);

	glm::vec2 velRel = m_body2 != nullptr ? m_body2->GetVelocity() : glm::vec2(0, 0) - m_body1->GetVelocity();

	// F = -kX - bv
	glm::vec2 force = distance * -m_springCoefficient * (m_restLength - length) - m_damping * velRel;

	m_body1->ApplyForce(force * timeStep, glm::vec2(0, 0));
	if (m_body2 != nullptr)
		m_body2->ApplyForce(-force * timeStep, glm::vec2(0, 0));
}

void Spring::MakeGizmo()
{
	aie::Gizmos::add2DLine(m_body1->GetPosition() + m_contact1, m_body2 != nullptr ? m_body2->GetPosition() : glm::vec2(0, 0) + m_contact2, glm::vec4(1, 1, 1, 1));
}
