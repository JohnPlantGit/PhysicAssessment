#pragma once
#include "CustomShape.h"
class Hexagon : public CustomShape
{
public:
	Hexagon(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec4 colour, float radius);
	Hexagon(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec4 colour, std::vector<glm::vec2> points);
	Hexagon(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec4 colour, glm::vec2 points[], int pointCount);
	~Hexagon();
};

