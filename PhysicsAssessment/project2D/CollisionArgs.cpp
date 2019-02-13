#include "CollisionArgs.h"

CollisionArgs::CollisionArgs()
{
	m_collided = false;
	m_collisionNormal = glm::vec2(0, 0);
}

CollisionArgs::~CollisionArgs()
{
}

CollisionArgs::CollisionArgs(bool collided)
{
	m_collided = collided;
	m_collisionNormal = glm::vec2(0, 0);
}
