#pragma once
#include <glm/ext.hpp>

#define SHAPE_COUNT 4
enum ShapeType
{
	SPRING = -1,
	LINE = 0,
	CIRCLE,
	SQUARE,
	CUSTOM
};

class PhysicsObject
{
public:
	PhysicsObject(ShapeType shape);
	~PhysicsObject();

	// 
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;
	virtual void ResetPosition() {};

	ShapeType GetShape() { return m_shape; }

protected:
	ShapeType m_shape;
};

