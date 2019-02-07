#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include <algorithm>
#include <list>
#include "Rigidbody.h"

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0,0))
{
}

PhysicsScene::~PhysicsScene()
{
	for (auto actor : m_actors)
	{
		delete actor;
	}
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

bool PhysicsScene::RemoveActor(PhysicsObject* actor)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i] == actor)
		{
			m_actors.erase(m_actors.begin() + i);
			return true;
		}
	}

	return false;
}

void PhysicsScene::Update(float deltaTime)
{
	static std::list<PhysicsObject*> dirty;

	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto actor : m_actors)
		{
			actor->FixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;

		for (auto actor : m_actors)
		{
			for (auto otherActor : m_actors)
			{
				if (actor == otherActor)
					continue;
				if (std::find(dirty.begin(), dirty.end(), actor) != dirty.end() &&
					std::find(dirty.begin(), dirty.end(), otherActor) != dirty.end())
					continue;

				Rigidbody* rigid = dynamic_cast<Rigidbody*>(actor);
				if (rigid->CheckCollision(otherActor) == true)
				{
					rigid->ApplyForceToActor(dynamic_cast<Rigidbody*>(otherActor), rigid->GetVelocity() * rigid->GetMass());
					dirty.push_back(actor);
					dirty.push_back(otherActor);
				}
			}
		}
		dirty.clear();
	}
}

void PhysicsScene::UpdateGizmos()
{
	for (auto actor : m_actors)
	{
		actor->MakeGizmo();
	}
}
