#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include <algorithm>
#include <list>
#include "Rigidbody.h"
#include "Circle.h"
#include "Line.h"
#include "Square.h"

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

		/*for (auto actor : m_actors)
		{
			for (auto otherActor : m_actors)
			{
				if (actor == otherActor)
					continue;
				if (std::find(dirty.begin(), dirty.end(), actor) != dirty.end() &&
					std::find(dirty.begin(), dirty.end(), otherActor) != dirty.end())
					continue;

				Rigidbody* rigid = dynamic_cast<Rigidbody*>(actor);
				if (rigid != nullptr && rigid->CheckCollision(otherActor) == true)
				{
					rigid->ApplyForceToActor(dynamic_cast<Rigidbody*>(otherActor), rigid->GetVelocity() * rigid->GetMass());
					dirty.push_back(actor);
					dirty.push_back(otherActor);
				}
			}
		}
		dirty.clear();*/

		CheckCollision();
	}
}

void PhysicsScene::UpdateGizmos()
{
	for (auto actor : m_actors)
	{
		actor->MakeGizmo();
	}
}

typedef bool(*fn)(PhysicsObject* a, PhysicsObject* b);

static fn CollisionFunctionArray[] =
{
	PhysicsScene::Line2Line,
	PhysicsScene::Line2Circle,
	PhysicsScene::Line2Square,
	PhysicsScene::Circle2Line,
	PhysicsScene::Circle2Circle,
	PhysicsScene::Circle2Square,
	PhysicsScene::Square2Line,
	PhysicsScene::Square2Circle,
	PhysicsScene::Square2Square,
};

void PhysicsScene::CheckCollision()
{
	int actorCount = m_actors.size();
	static std::list<PhysicsObject*> dirty;

	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			if (std::find(dirty.begin(), dirty.end(), m_actors[outer]) != dirty.end() &&
				std::find(dirty.begin(), dirty.end(), m_actors[inner]) != dirty.end())
				continue;

			PhysicsObject* a = m_actors[outer];
			PhysicsObject* b = m_actors[inner];
			int shapeIdA = a->GetShape();
			int shapeIdB = b->GetShape();

			int functionId = (shapeIdA * SHAPE_COUNT) + shapeIdB;
			fn collisionFunctionPtr = CollisionFunctionArray[functionId];
			if (collisionFunctionPtr != nullptr)
			{
				if (collisionFunctionPtr(a, b))
				{
					dirty.push_back(m_actors[outer]);
					dirty.push_back(m_actors[inner]);
				}
			}
		}
	}
	dirty.clear();
}

bool PhysicsScene::Line2Line(PhysicsObject* a, PhysicsObject* b)
{
	return false;
}

bool PhysicsScene::Line2Circle(PhysicsObject* a, PhysicsObject* b)
{
	return Circle2Line(b, a);
}

bool PhysicsScene::Line2Square(PhysicsObject* a, PhysicsObject* b)
{
	Line* line = dynamic_cast<Line*>(a);
	Square* square = dynamic_cast<Square*>(b);

	glm::vec2 squarePos = square->GetPosition();
	glm::vec2 squareMin = square->GetMin();
	glm::vec2 squareMax = square->GetMax();

	glm::vec2 corners[4] =
	{
		glm::vec2(squarePos + glm::vec2(squareMin.x, squareMax.y)),
		glm::vec2(squarePos + squareMax),
		glm::vec2(squarePos + glm::vec2(squareMax.x, squareMin.y)),
		glm::vec2(squarePos + squareMin),
	};

	glm::vec2 collisionNormal = line->GetNormal();
	float cornersToLine[4];
	for (int i = 0; i < 4; i++)
	{
		cornersToLine[i] = glm::dot(corners[i], line->GetNormal()) - line->GetDistance();
	}

	int inFront = 0;
	int behind = 0;

	for (int i = 0; i < 4; i++)
	{
		if (cornersToLine[i] < 0)
			behind++;
		else if (cornersToLine[i] > 0)
			inFront++;
	}

	if (inFront != 0 && behind != 0)
	{
		line->ResolveCollision(square);
		return true;
	}
	return false;
}

bool PhysicsScene::Circle2Line(PhysicsObject* a, PhysicsObject* b)
{
	Circle* circle = dynamic_cast<Circle*>(a);
	Line* line = dynamic_cast<Line*>(b);

	if (circle != nullptr && line != nullptr)
	{
		glm::vec2 collisionNormal = line->GetNormal();
		float circleToLine = glm::dot(circle->GetPosition(), line->GetNormal()) - line->GetDistance();

		if (circleToLine < 0)
		{
			collisionNormal *= -1;
			circleToLine *= -1;
		}

		float intersection = circle->GetRadius() - circleToLine;
		if (intersection > 0)
		{
			line->ResolveCollision(circle);
			circle->SetPosition(circle->GetPosition() - (line->GetNormal() * intersection));

			return true;
		}
	}
	return false;
}

bool PhysicsScene::Circle2Circle(PhysicsObject* a, PhysicsObject* b)
{
	Circle* circleA = dynamic_cast<Circle*>(a);
	Circle* circleB = dynamic_cast<Circle*>(b);

	if (circleA != nullptr && circleB != nullptr)
	{
		float length = glm::length(circleA->GetPosition() - circleB->GetPosition());

		if (length < circleA->GetRadius() + circleB->GetRadius())
		{
			circleA->ResolveCollision(circleB);

			return true;
		}
	}

	return false;
}

bool PhysicsScene::Circle2Square(PhysicsObject* a, PhysicsObject* b)
{
	Circle* circle = dynamic_cast<Circle*>(a);
	Square* square = dynamic_cast<Square*>(b);

	glm::vec2 closestPoint = glm::clamp(circle->GetPosition(), square->GetMin(), square->GetMax());
	glm::vec2 distance = circle->GetPosition() - closestPoint;

	if (glm::length(distance) < circle->GetRadius())
	{
		square->ResolveCollision(circle);
		 
		return true;
	}
	return false;
}

bool PhysicsScene::Square2Line(PhysicsObject* a, PhysicsObject* b)
{
	return Line2Square(b, a);
}

bool PhysicsScene::Square2Circle(PhysicsObject* a, PhysicsObject* b)
{
	return Circle2Square(b, a);
}

bool PhysicsScene::Square2Square(PhysicsObject* a, PhysicsObject* b)
{
	return false;
}
