#pragma once
#include <glm/ext.hpp>
#include <vector>
class CollisionArgs;
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

	std::vector<PhysicsObject*> CheckPoint(glm::vec2 pos);
	bool Point2Circle(glm::vec2 pos, PhysicsObject* obj);
	bool Point2Square(glm::vec2 pos, PhysicsObject* obj);
	
	void CheckCollision();

	static CollisionArgs Line2Line(PhysicsObject* a, PhysicsObject* b);
	static CollisionArgs Line2Circle(PhysicsObject* a, PhysicsObject* b);
	static CollisionArgs Line2Square(PhysicsObject* a, PhysicsObject* b);
	static CollisionArgs Circle2Line(PhysicsObject* a, PhysicsObject* b);
	static CollisionArgs Circle2Circle(PhysicsObject* a, PhysicsObject* b);
	static CollisionArgs Circle2Square(PhysicsObject* a, PhysicsObject* b);
	static CollisionArgs Square2Line(PhysicsObject* a, PhysicsObject* b);
	static CollisionArgs Square2Circle(PhysicsObject* a, PhysicsObject* b);
	static CollisionArgs Square2Square(PhysicsObject* a, PhysicsObject* b);

protected:

	static glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
};

