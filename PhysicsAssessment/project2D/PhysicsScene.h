#pragma once
#include <glm/ext.hpp>
#include <vector>
class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* actor);
	bool RemoveActor(PhysicsObject* actor);
	void Update(float deltaTime);
	void UpdateGizmos();

	void SetGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 GetGravity() const { return m_gravity; }

	void SetTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float GetTimeStep() const { return m_timeStep; }

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
};

