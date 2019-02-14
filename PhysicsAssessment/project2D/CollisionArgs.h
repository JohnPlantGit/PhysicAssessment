#pragma once
#include <glm\ext.hpp>
class CollisionArgs
{
public:
	CollisionArgs();
	CollisionArgs(bool collided);
	~CollisionArgs();

	bool m_collided;
	glm::vec2 m_collisionNormal;
	glm::vec2 m_normalForce;

};

