#include "Spring.h"
#include "Rigidbody.h"
#include <Gizmos.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <time.h>
#include <iostream>

// Initializes spring variables to the parameters and passes the "SPRING" shapetype enum into the physicsobject constructor
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

// Uses the equation (F = -kX - bv) to calculate the force the spring applies on the rigidbodies
// if rigidbody2 doesn't exist the equation just uses the contact2 position, this can be used to allow a spring to be connected to a position other than a rigidbody
void Spring::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	//clock_t start = clock();

	// Gets the distance between the two rigidbodies
	glm::vec2 point1 = m_body1->GetPosition() + m_contact1;
	glm::vec2 point2 = m_body2 != nullptr ? m_body2->GetPosition() : glm::vec2(0, 0) + m_contact2;
	glm::vec2 distance = point2 - point1;
	float length = glm::length2(distance);

	glm::vec2 velRel = m_body2 != nullptr ? m_body2->GetVelocity() : glm::vec2(0, 0) - m_body1->GetVelocity();

	// F = -kX - bv
	glm::vec2 force = distance * -m_springCoefficient * (m_restLength * m_restLength - length) - m_damping * velRel;

	// applies force to the rigid bodies
	m_body1->ApplyForce(force * timeStep, glm::vec2(0, 0));
	if (m_body2 != nullptr)
		m_body2->ApplyForce(-force * timeStep, glm::vec2(0, 0));

	//printf("Time taken: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
}

// Draws a line between the rigidbodies if the spring is set to draw 
void Spring::MakeGizmo()
{
	if (m_draw)
		aie::Gizmos::add2DLine(m_body1->GetPosition() + m_contact1, m_body2 != nullptr ? m_body2->GetPosition() : glm::vec2(0, 0) + m_contact2, glm::vec4(1, 1, 1, 1));
}
