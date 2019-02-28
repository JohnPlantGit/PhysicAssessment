#pragma once
#include "PhysicsObject.h"

class Rigidbody;

/*
	Connects two rigidbodies to each other with a spring
*/
class Spring : public PhysicsObject
{
public:
	// Constructor
	// Params:
	//		rb1 : the first rigid body
	//		rb2 : the second rigid body
	//		restLength : the desired length of the spring
	//		springCpefficient : the spring coefficient of the spring used in hookes law
	//		damping : how quickly the two rigidboies will lose velocity
	//		contact1 : the location that the spring connects to the first rigid body
	//		contact2 : the location that the spring connects to the second rigid body
	Spring(Rigidbody* rb1, Rigidbody* rb2, float restLength, float springCoefficient, float damping = -1,
		glm::vec2 contact1 = glm::vec2(0, 0), glm::vec2 contact2 = glm::vec2(0, 0));
	~Spring();

	// Applies forces on the rigidbodies acording to hookes law
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Debug() {}
	// Draws a line between the two rigidbodies
	virtual void MakeGizmo();

	// Sets the position of contact2
	void SetContact2(glm::vec2 pos) { m_contact2 = pos; }

	bool m_draw = true;
protected:
	Rigidbody* m_body1;
	Rigidbody* m_body2;

	glm::vec2 m_contact1;
	glm::vec2 m_contact2;

	float m_damping;
	float m_restLength;
	float m_springCoefficient;
};

