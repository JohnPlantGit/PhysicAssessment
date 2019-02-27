#include "Hexagon.h"
#define _USE_MATH_DEFINES
#include <math.h>

Hexagon::Hexagon(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec4 colour, float radius) : 
	CustomShape(position, velocity, mass, rotation, colour)
{
	float angle = 60;
	m_points.push_back(glm::vec2(cosf(angle * (M_PI / 180)), sinf(angle * (M_PI / 180))) * radius);
	m_points.push_back(glm::vec2(1, 0) * radius);
	m_points.push_back(glm::vec2(cosf(-angle * (M_PI / 180)), sinf(-angle * (M_PI / 180))) * radius);
	m_points.push_back(glm::vec2(cosf((-180 + angle) * (M_PI / 180)), sinf((-180 + angle) * (M_PI / 180))) * radius);
	m_points.push_back(glm::vec2(-1, 0) * radius);
	m_points.push_back(glm::vec2(cosf((180 - angle) * (M_PI / 180)), sinf((180 - angle) * (M_PI / 180))) * radius);
}

Hexagon::Hexagon(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec4 colour, std::vector<glm::vec2> points) : 
	CustomShape(position, velocity, mass, rotation, colour, points)
{

}

Hexagon::Hexagon(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec4 colour, glm::vec2 points[], int pointCount) :
	CustomShape(position, velocity, mass, rotation, colour, points, pointCount)
{

}

Hexagon::~Hexagon()
{
}
