#include "PhysicsObject.h"

// Stores the shape parameter in the physics object
PhysicsObject::PhysicsObject(ShapeType shape)
{
	m_shape = shape;
}

PhysicsObject::~PhysicsObject()
{
}
