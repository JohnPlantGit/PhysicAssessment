#pragma once
#include "Rigidbody.h"
#include <vector>
class CustomShape : public Rigidbody
{
public:
	CustomShape(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec4 colour);
	CustomShape(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec4 colour, std::vector<glm::vec2> points);
	CustomShape(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec4 colour, glm::vec2 points[], int pointCount);
	~CustomShape();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void MakeGizmo();
	virtual void ResolveCollision(Rigidbody* other, CollisionArgs cArgs);

	glm::vec2 GetMin() { return m_min; }
	glm::vec2 GetMax() { return m_max; }

	glm::mat3x3 GetRotationMaxtrix() { return m_rotationMatrix; }

	std::vector<glm::vec2> GetPoints();
	std::vector<glm::vec2> GetRelativePoints() { return m_points; }

protected:

	void UpdateMinMax();

	glm::vec4 m_colour;

	glm::vec2 m_min;
	glm::vec2 m_max;

	std::vector<glm::vec2> m_points;

	glm::mat3x3 m_rotationMatrix;
};

