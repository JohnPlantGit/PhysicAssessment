#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include <algorithm>
#include <list>
#include "Rigidbody.h"
#include "Circle.h"
#include "Line.h"
#include "Square.h"
#include "CollisionArgs.h"
#include <math.h>
#include "LengthConstraint.h"
#include "CustomShape.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>


glm::vec2 PhysicsScene::m_gravity = (glm::vec2(0, 0));

PhysicsScene::PhysicsScene() : m_timeStep(0.02f) 
{
}

PhysicsScene::~PhysicsScene()
{
	for (auto actor : m_actors)
	{
		delete actor;
	}
	for (auto constraint : m_constraints)
	{
		delete constraint;
	}
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::AddConstraint(LengthConstraint* constraint)
{
	m_constraints.push_back(constraint);
}

bool PhysicsScene::RemoveActor(PhysicsObject* actor)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i] == actor)
		{
			delete m_actors[i]; 
			m_actors.erase(m_actors.begin() + i);
			return true;
		}
	}

	return false;
}

void PhysicsScene::Update(float deltaTime)
{
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep)
	{
		clock_t start = clock();
		for (auto actor : m_actors)
		{
			actor->FixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;

		for (int i = 0; i < m_constraintIterations; i++)
		{
			for (auto constraint : m_constraints)
			{
				constraint->SatisfyConstraint();
			}
		}

		CheckCollision();

		printf("Time taken: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
	}
}

void PhysicsScene::UpdateGizmos()
{
	for (auto actor : m_actors)
	{
		actor->MakeGizmo();
	}
}

std::vector<PhysicsObject*> PhysicsScene::CheckPoint(glm::vec2 pos)
{
	std::vector<PhysicsObject*> objectsHit;
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i]->GetShape() == CIRCLE)
		{
			if (Point2Circle(pos, m_actors[i]))
				objectsHit.push_back(m_actors[i]);
		}
		if (m_actors[i]->GetShape() == SQUARE)
		{
			if (Point2Square(pos, m_actors[i]))
				objectsHit.push_back(m_actors[i]);
		}
	}

	return objectsHit;
}

bool PhysicsScene::Point2Circle(glm::vec2 pos, PhysicsObject* obj)
{
	Circle* circle = dynamic_cast<Circle*>(obj);
	return glm::length2(pos - circle->GetPosition()) < circle->GetRadius() * circle->GetRadius();
}

bool PhysicsScene::Point2Square(glm::vec2 pos, PhysicsObject* obj)
{
	Square* square = dynamic_cast<Square*>(obj);
	return glm::all(glm::greaterThan(pos, square->GetMin())) && glm::all(glm::lessThan(pos, square->GetMax()));
}

typedef CollisionArgs(*fn)(PhysicsObject* a, PhysicsObject* b);

static fn CollisionFunctionArray[] =
{
	PhysicsScene::Line2Line,
	PhysicsScene::Line2Circle,
	PhysicsScene::Line2Square,
	PhysicsScene::Line2Custom,
	PhysicsScene::Circle2Line,
	PhysicsScene::Circle2Circle,
	PhysicsScene::Circle2Square,
	PhysicsScene::Circle2Custom,
	PhysicsScene::Square2Line,
	PhysicsScene::Square2Circle,
	PhysicsScene::Square2Square,
	PhysicsScene::Square2Custom,
	PhysicsScene::Custom2Line,
	PhysicsScene::Custom2Circle,
	PhysicsScene::Custom2Square,
	PhysicsScene::Custom2Custom
};

void PhysicsScene::CheckCollision()
{
	int actorCount = m_actors.size();
	static std::vector<PhysicsObject*> dirtyA;
	static std::vector<PhysicsObject*> dirtyB;

	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			if (std::find(dirtyB.begin(), dirtyB.end(), m_actors[outer]) != dirtyB.end() &&
				std::find(dirtyA.begin(), dirtyA.end(), m_actors[inner]) != dirtyA.end())
			{
				if (std::find(dirtyB.begin(), dirtyB.end(), m_actors[outer]) - dirtyB.end() == std::find(dirtyA.begin(), dirtyA.end(), m_actors[inner]) - dirtyB.end())
					continue;
			}

			PhysicsObject* a = m_actors[outer];
			PhysicsObject* b = m_actors[inner];
			int shapeIdA = a->GetShape();
			int shapeIdB = b->GetShape();

			if (shapeIdA < 0 || shapeIdB < 0)
				continue;

			int functionId = (shapeIdA * SHAPE_COUNT) + shapeIdB;
			fn collisionFunctionPtr = CollisionFunctionArray[functionId];
			if (collisionFunctionPtr != nullptr)
			{
				CollisionArgs collision = collisionFunctionPtr(a, b);
				if (collision.m_collided)
				{
					dirtyA.push_back(m_actors[outer]);
					dirtyB.push_back(m_actors[inner]);
				}
			}
		}
	}
	dirtyA.clear();
	dirtyB.clear();
}

CollisionArgs PhysicsScene::Line2Line(PhysicsObject* a, PhysicsObject* b)
{
	return CollisionArgs(false);
}

CollisionArgs PhysicsScene::Line2Circle(PhysicsObject* a, PhysicsObject* b)
{
	return Circle2Line(b, a);
}

CollisionArgs PhysicsScene::Line2Square(PhysicsObject* a, PhysicsObject* b)
{
	Line* line = dynamic_cast<Line*>(a);
	Square* square = dynamic_cast<Square*>(b);
	CollisionArgs output;

	glm::vec2 squarePos = square->GetPosition();
	glm::vec2 squareMin = square->GetMinRelative();
	glm::vec2 squareMax = square->GetMaxRelative();

	glm::mat3x3 rotMatrix = square->GetRotationMaxtrix();

	glm::vec2 squareCorners[4] =
	{
		(rotMatrix * glm::vec3(squareMin.x, squareMin.y, 0)),
		(rotMatrix * glm::vec3(squareMin.x, squareMax.y, 0)),
		(rotMatrix * glm::vec3(squareMax.x, squareMax.y ,0)),
		(rotMatrix * glm::vec3(squareMax.x, squareMin.y, 0)),
	};
	for (int i = 0; i < 4; i++)
	{
		squareCorners[i] += square->GetPosition();
	}

	glm::vec2 collisionNormal = line->GetNormal();
	float cornersToLine[4];
	for (int i = 0; i < 4; i++)
	{
		cornersToLine[i] = glm::dot(squareCorners[i], line->GetNormal()) - line->GetDistance();
	}

	int inFront = 0;
	int behind = 0;

	for (int i = 0; i < 4; i++)
	{
		if (cornersToLine[i] < 0)
		{
			behind++;
		}
		else if (cornersToLine[i] > 0)
		{
			inFront++;
		}
	}

	float overlap = cornersToLine[0];
	for (int i = 1; i < 4; i++)
	{
		if (fabsf(overlap) > fabsf(cornersToLine[i]))
		{
			overlap = cornersToLine[i];
		}
	}

	if (inFront != 0 && behind != 0)
	{
		output.m_collided = true;
		if (glm::dot(square->GetPosition(), line->GetNormal()) - line->GetDistance() < 0)
			collisionNormal *= -1;
		output.m_collisionNormal = collisionNormal;
		glm::vec2 contact(0, 0);
		int count = 0;
		for (int i = 0; i < 4; i++)
		{
			if (overlap < 0 && cornersToLine[i] < 0)
			{
				contact += squareCorners[i];
				count++;
			}
			if (overlap > 0 && cornersToLine[i] > 0)
			{
				contact += squareCorners[i];
				count++;
			}
		}
		contact = contact / (float)count;
		output.m_contactPoint = (contact /*- (overlap * collisionNormal)*/) - square->GetPosition();

		square->SetPosition(square->GetPosition() + (fabsf(overlap) * collisionNormal));

		line->ResolveCollision(square, output);

		return output;
	}

	return output;
}

CollisionArgs PhysicsScene::Line2Custom(PhysicsObject* a, PhysicsObject* b)
{
	Line* line = dynamic_cast<Line*>(a);
	CustomShape* custom = dynamic_cast<CustomShape*>(b);
	CollisionArgs output;

	std::vector<glm::vec2> customPoints = custom->GetPoints();
	int pointCount = customPoints.size();

	glm::vec2 collisionNormal = line->GetNormal();
	std::vector<float> pointsToLine;
	for (int i = 0; i < pointCount; i++)
	{
		pointsToLine.push_back(glm::dot(customPoints[i], line->GetNormal()) - line->GetDistance());
	}

	int inFront = 0;
	int behind = 0;

	for (int i = 0; i < pointCount; i++)
	{
		if (pointsToLine[i] < 0)
		{
			behind++;
		}
		else if (pointsToLine[i] > 0)
		{
			inFront++;
		}
	}

	float overlap = pointsToLine[0];
	for (int i = 1; i < pointCount; i++)
	{
		if (fabsf(overlap) > fabsf(pointsToLine[i]))
		{
			overlap = pointsToLine[i];
		}
	}

	if (inFront != 0 && behind != 0)
	{
		output.m_collided = true;
		if (glm::dot(custom->GetPosition(), line->GetNormal()) - line->GetDistance() < 0)
			collisionNormal *= -1;
		output.m_collisionNormal = collisionNormal;
		glm::vec2 contact(0, 0);
		int count = 0;
		for (int i = 0; i < 4; i++)
		{
			if (overlap < 0 && pointsToLine[i] < 0)
			{
				contact += customPoints[i];
				count++;
			}
			if (overlap > 0 && pointsToLine[i] > 0)
			{
				contact += customPoints[i];
				count++;
			}
		}
		contact = contact / (float)count;
		output.m_contactPoint = (contact /*- (overlap * collisionNormal)*/)-custom->GetPosition();

		custom->SetPosition(custom->GetPosition() + (fabsf(overlap) * collisionNormal));

		line->ResolveCollision(custom, output);

		return output;
	}

	return output;
	
}

CollisionArgs PhysicsScene::Circle2Line(PhysicsObject* a, PhysicsObject* b)
{
	Circle* circle = dynamic_cast<Circle*>(a);
	Line* line = dynamic_cast<Line*>(b);
	CollisionArgs output;

	if (circle != nullptr && line != nullptr)
	{
		glm::vec2 collisionNormal = line->GetNormal();
		float circleToLine = glm::dot(circle->GetPosition(), line->GetNormal()) - line->GetDistance();

		/*if (circleToLine < 0)
		{
			collisionNormal *= -1;
			circleToLine *= -1;
		}*/

		float intersection = circle->GetRadius() - circleToLine;
		if (intersection > 0)
		{
   			circle->SetPosition(circle->GetPosition() + (collisionNormal * intersection));

			output.m_collided = true;
			output.m_collisionNormal = collisionNormal;

			if (collisionNormal.y == -1)
				int tmp = 1;

			glm::vec2 normalForce = (collisionNormal * fabsf(glm::dot(collisionNormal, m_gravity)));
			output.m_normalForce = normalForce;
			output.m_contactPoint = circle->GetPosition() + (collisionNormal * -circle->GetRadius());

			line->ResolveCollision(circle, output);
			return output;
		}
	}

	return output;
}

CollisionArgs PhysicsScene::Circle2Circle(PhysicsObject* a, PhysicsObject* b)
{
	Circle* circleA = dynamic_cast<Circle*>(a);
	Circle* circleB = dynamic_cast<Circle*>(b);
	CollisionArgs output;

	if (circleA != nullptr && circleB != nullptr)
	{
		float length = glm::length2(circleA->GetPosition() - circleB->GetPosition());
		glm::vec2 normal;
		if (length > 0)
			normal = glm::normalize(circleA->GetPosition() - circleB->GetPosition());
		else
			normal = glm::vec2(0, 1);

		if (length < (circleA->GetRadius() + circleB->GetRadius()) * (circleA->GetRadius() + circleB->GetRadius()))
		{
			float overlap = (circleA->GetRadius() + circleB->GetRadius()) - sqrt(length);

			float massRatio = circleA->GetMass() / (circleA->GetMass() + circleB->GetMass());

			if (circleA->GetKinematic() && !circleB->GetKinematic())
			{
				circleB->SetPosition(circleB->GetPosition() + overlap * -normal);
			}
			else if (!circleA->GetKinematic() && circleB->GetKinematic())
			{
				circleA->SetPosition(circleA->GetPosition() + overlap * normal);
			}
			else
			{
				circleA->SetPosition(circleA->GetPosition() + overlap * normal * massRatio);
				circleB->SetPosition(circleB->GetPosition() + overlap * -normal * (1 - massRatio));
			}

			output.m_collided = true;
			output.m_collisionNormal = normal;
			output.m_contactPoint = 0.5f * (circleA->GetPosition() + circleB->GetPosition());

			circleA->ResolveCollision(circleB, output);

			return output;
		}
	}
	
	return output;
}

CollisionArgs PhysicsScene::Circle2Square(PhysicsObject* a, PhysicsObject* b)
{
	Circle* circle = dynamic_cast<Circle*>(a);
	Square* square = dynamic_cast<Square*>(b);
	CollisionArgs output;

	glm::vec2 cPos = circle->GetPosition();

	glm::vec2 closestPoint = glm::clamp(circle->GetPosition(), square->GetMin(), square->GetMax());
	glm::vec2 distance = circle->GetPosition() - closestPoint;

	if (glm::length2(distance) < circle->GetRadius() * circle->GetRadius())
	{		 
		// SAT
		glm::vec2 squareMin = square->GetMinRelative();
		glm::vec2 squareMax = square->GetMaxRelative();

		glm::mat3x3 rotMatrix = square->GetRotationMaxtrix();

		glm::vec2 squareCorners[4] =
		{
			(rotMatrix * glm::vec3(squareMin.x, squareMin.y, 0)),
			(rotMatrix * glm::vec3(squareMin.x, squareMax.y, 0)),
			(rotMatrix * glm::vec3(squareMax.x, squareMax.y ,0)),
			(rotMatrix * glm::vec3(squareMax.x, squareMin.y, 0)),
		};
		for (int i = 0; i < 4; i++)
		{
			squareCorners[i] += square->GetPosition();
		}

		glm::vec2 normals[4];

		// get the 4 normals of the square
		for (int i = 0; i < 4; i++)
		{
			glm::vec2 side = squareCorners[i] - squareCorners[i + 1 < 4 ? i + 1 : 0];
			side = glm::normalize(side);
			normals[i] = glm::vec2(side.y, -side.x);
		}

		float overlap[4]; // the overlap on each of the normals

		for (int i = 0; i < 4; i++)
		{
			float sqrExtents[4];

			for (int j = 0; j < 4; j++)
			{
				sqrExtents[j] = glm::dot(squareCorners[j], normals[i]);
			}

			float sqrExtentsMax = sqrExtents[0];
			float sqrExtentsMin = sqrExtents[0];

			for (int j = 1; j < 4; j++)
			{
				if (sqrExtents[j] > sqrExtentsMax)
					sqrExtentsMax = sqrExtents[j];
				if (sqrExtents[j] < sqrExtentsMin)
					sqrExtentsMin = sqrExtents[j];
			}

			float circleExtent = glm::dot(circle->GetPosition(), normals[i]);

			float circleExtentMax = circleExtent + circle->GetRadius();
			float circleExtentMin = circleExtent - circle->GetRadius();

			// if there is no overlap
			if (fmin(sqrExtentsMax - circleExtentMin, circleExtentMax - sqrExtentsMin) < 0)
				return output;

			overlap[i] = fmin(sqrExtentsMax - circleExtentMin, circleExtentMax - sqrExtentsMin);
		}

		int overlapMin = 0; 
		for (int i = 1; i < 4; i++)
		{
			if (overlap[i] < overlap[overlapMin])
				overlapMin = i;
		}

		glm::vec2 direction = circle->GetPosition() - square->GetPosition();
		direction = glm::normalize(direction);

		if (glm::dot(normals[overlapMin], direction) < 0)
			normals[overlapMin] *= -1;

		output.m_collided = true;
		output.m_collisionNormal = normals[overlapMin];

		//

		float massRatio = circle->GetMass() / (circle->GetMass() + square->GetMass());

		if (!circle->GetKinematic() && !square->GetKinematic())
		{
			circle->SetPosition(circle->GetPosition() + (normals[overlapMin] * overlap[overlapMin] * massRatio));
			square->SetPosition(square->GetPosition() - (normals[overlapMin] * overlap[overlapMin] * (1 - massRatio)));
		}
		else if (!circle->GetKinematic() && square->GetKinematic())
		{
			circle->SetPosition(circle->GetPosition() + (normals[overlapMin] * overlap[overlapMin]));
		}
		else if (circle->GetKinematic() && !square->GetKinematic())
		{
			square->SetPosition(square->GetPosition() - (normals[overlapMin] * overlap[overlapMin]));
		}

		square->ResolveCollision(circle, output);

		return output;
	}
	return output;
}

CollisionArgs PhysicsScene::Circle2Custom(PhysicsObject* a, PhysicsObject* b)
{
	Circle* circle = dynamic_cast<Circle*>(a);
	CustomShape* custom = dynamic_cast<CustomShape*>(b);
	CollisionArgs output;

	glm::vec2 cPos = circle->GetPosition();

	glm::vec2 closestPoint = glm::clamp(circle->GetPosition(), custom->GetMin(), custom->GetMax());
	glm::vec2 distance = circle->GetPosition() - closestPoint;

	if (glm::length2(distance) < circle->GetRadius() * circle->GetRadius())
	{
		std::vector<glm::vec2> customPoints = custom->GetPoints();

		std::vector<glm::vec2> normals;

		for (int i = 0; i < customPoints.size(); i++)
		{
			glm::vec2 side = customPoints[i] - customPoints[i + 1 < customPoints.size() ? i + 1 : 0];
			side = glm::normalize(side);
			normals.push_back(glm::vec2(side.y, -side.x));
		}

		std::vector<float> overlap;
		for (int i = 0; i < customPoints.size(); i++)
		{
			std::vector<float> customExtents;

			for (int j = 0; j < customPoints.size(); j++)
			{
				customExtents.push_back(glm::dot(customPoints[j], normals[i]));
			}

			float customExtentsMax = customExtents[0];
			float customExtentsMin = customExtents[0];

			for (int j = 1; j < customPoints.size(); j++)
			{
				if (customExtents[j] > customExtentsMax)
					customExtentsMax = customExtents[j];
				if (customExtents[j] < customExtentsMin)
					customExtentsMin = customExtents[j];
			}

			float circleExtent = glm::dot(circle->GetPosition(), normals[i]);
			float circleExtentMax = circleExtent + circle->GetRadius();
			float circleExtentMin = circleExtent - circle->GetRadius();

			// no overlap
			if (fmin(customExtentsMax - circleExtentMin, circleExtentMax - customExtentsMin) < 0)
				return output;

			overlap.push_back(fmin(customExtentsMax - circleExtentMin, circleExtentMax - customExtentsMin));
		}

		int overlapMin = 0;
		for (int i = 1; i < overlap.size(); i++)
		{
			if (overlap[i] < overlap[overlapMin])
				overlapMin = i;
		}

		glm::vec2 direction = circle->GetPosition() - custom->GetPosition();
		direction = glm::normalize(direction);

		if (glm::dot(normals[overlapMin], direction) < 0)
			normals[overlapMin] *= -1;

		output.m_collided = true;
		output.m_collisionNormal = normals[overlapMin];
		//

		float massRatio = circle->GetMass() / (circle->GetMass() + custom->GetMass());

		if (!circle->GetKinematic() && !custom->GetKinematic())
		{
			circle->SetPosition(circle->GetPosition() + (normals[overlapMin] * overlap[overlapMin] * massRatio));
			custom->SetPosition(custom->GetPosition() - (normals[overlapMin] * overlap[overlapMin] * (1 - massRatio)));
		}
		else if (!circle->GetKinematic() && custom->GetKinematic())
		{
			circle->SetPosition(circle->GetPosition() + (normals[overlapMin] * overlap[overlapMin]));
		}
		else if (circle->GetKinematic() && !custom->GetKinematic())
		{
			custom->SetPosition(custom->GetPosition() - (normals[overlapMin] * overlap[overlapMin]));
		}

		custom->ResolveCollision(circle, output);
		
		return output;
	}
	return output;
}

CollisionArgs PhysicsScene::Square2Line(PhysicsObject* a, PhysicsObject* b)
{
	return Line2Square(b, a);
}

CollisionArgs PhysicsScene::Square2Circle(PhysicsObject* a, PhysicsObject* b)
{
	return Circle2Square(b, a);
}

CollisionArgs PhysicsScene::Square2Square(PhysicsObject* a, PhysicsObject* b)
{
	Square* squareA = dynamic_cast<Square*>(a);
	Square* squareB = dynamic_cast<Square*>(b);
	CollisionArgs output;

	if (glm::all(glm::lessThan(squareA->GetMin(), squareB->GetMax())) && glm::all(glm::greaterThan(squareA->GetMax(), squareB->GetMin())))
	{
		// Square A's corners
		glm::vec2 squareAMin = squareA->GetMinRelative();
		glm::vec2 squareAMax = squareA->GetMaxRelative();

		glm::mat3x3 rotMatrixA = squareA->GetRotationMaxtrix();

		glm::vec2 squareACorners[4] =
		{
			(rotMatrixA * glm::vec3(squareAMin.x, squareAMin.y, 0)),
			(rotMatrixA * glm::vec3(squareAMin.x, squareAMax.y, 0)),
			(rotMatrixA * glm::vec3(squareAMax.x, squareAMax.y ,0)),
			(rotMatrixA * glm::vec3(squareAMax.x, squareAMin.y, 0)),
		};
		for (int i = 0; i < 4; i++)
		{
			squareACorners[i] += squareA->GetPosition();
		}

		// Square B's corners
		glm::vec2 squareBMin = squareB->GetMinRelative();
		glm::vec2 squareBMax = squareB->GetMaxRelative();

		glm::mat3x3 rotMatrixB = squareB->GetRotationMaxtrix();

		glm::vec2 squareBCorners[4] =
		{
			(rotMatrixB * glm::vec3(squareBMin.x, squareBMin.y, 0)),
			(rotMatrixB * glm::vec3(squareBMin.x, squareBMax.y, 0)),
			(rotMatrixB * glm::vec3(squareBMax.x, squareBMax.y ,0)),
			(rotMatrixB * glm::vec3(squareBMax.x, squareBMin.y, 0)),
		};
		for (int i = 0; i < 4; i++)
		{
			squareBCorners[i] += squareB->GetPosition();
		}

		// SAT
		glm::vec2 normals[8];
		for (int i = 0; i < 4; i++)
		{
			glm::vec2 side = squareACorners[i] - squareACorners[i + 1 < 4 ? i + 1 : 0];
			side = glm::normalize(side);
			normals[i] = glm::vec2(side.y, -side.x);
		}
		for (int i = 0; i < 4; i++)
		{
			glm::vec2 side = squareBCorners[i] - squareBCorners[i + 1 < 4 ? i + 1 : 0];
			side = glm::normalize(side);
			normals[i + 4] = glm::vec2(side.y, -side.x);
		}

		float overlap[8];

		for (int i = 0; i < 8; i++)
		{
			float AExtents[4];
			float BExtents[4];

			for (int j = 0; j < 4; j++)
			{
				AExtents[j] = glm::dot(squareACorners[j], normals[i]);
				BExtents[j] = glm::dot(squareBCorners[j], normals[i]);
			}

			float AExtentsMax = AExtents[0];
			float AExtentsMin = AExtents[0];

			float BExtentsMax = BExtents[0];
			float BExtentsMin = BExtents[0];

			for (int j = 0; j < 4; j++)
			{
				if (AExtents[j] > AExtentsMax)
					AExtentsMax = AExtents[j];
				if (AExtents[j] < AExtentsMin)
					AExtentsMin = AExtents[j];

				if (BExtents[j] > BExtentsMax)
					BExtentsMax = BExtents[j];
				if (BExtents[j] < BExtentsMin)
					BExtentsMin = BExtents[j];
			}

			if (fmin(AExtentsMax - BExtentsMin, BExtentsMax - AExtentsMin) < 0)
				return output;

			overlap[i] = fmin(AExtentsMax - BExtentsMin, BExtentsMax - AExtentsMin);
		}

		int overlapMin = 0;
		for (int i = 1; i < 8; i++)
		{
			if (overlap[i] < overlap[overlapMin])
				overlapMin = i;
		}

		glm::vec2 direction = squareB->GetPosition() - squareA->GetPosition();
		direction = glm::normalize(direction);

		if (glm::dot(normals[overlapMin], direction) < 0)
			normals[overlapMin] *= -1;

		output.m_collided = true;
		output.m_collisionNormal = normals[overlapMin];
		//

		float massRatio = squareA->GetMass() / (squareB->GetMass() + squareA->GetMass());

		if (!squareB->GetKinematic() && !squareA->GetKinematic())
		{
			squareB->SetPosition(squareB->GetPosition() + (normals[overlapMin] * overlap[overlapMin] * massRatio));
			squareA->SetPosition(squareA->GetPosition() - (normals[overlapMin] * overlap[overlapMin] * (1 - massRatio)));
		}
		else if (!squareB->GetKinematic() && squareA->GetKinematic())
		{
			squareB->SetPosition(squareB->GetPosition() + (normals[overlapMin] * overlap[overlapMin]));
		}
		else if (squareB->GetKinematic() && !squareA->GetKinematic())
		{
			squareA->SetPosition(squareA->GetPosition() - (normals[overlapMin] * overlap[overlapMin]));
		}		

		squareA->ResolveCollision(squareB, output);

		return output;
	}

	return output;
}

CollisionArgs PhysicsScene::Square2Custom(PhysicsObject* a, PhysicsObject* b)
{
	Square* square = dynamic_cast<Square*>(a);
	CustomShape* custom = dynamic_cast<CustomShape*>(b);
	CollisionArgs output;

	if (glm::all(glm::lessThan(square->GetMin(), custom->GetMax())) && glm::all(glm::greaterThan(square->GetMax(), custom->GetMin())))
	{
		// Square's corners
		glm::vec2 squareMin = square->GetMinRelative();
		glm::vec2 squareMax = square->GetMaxRelative();

		glm::mat3x3 rotMatrix = square->GetRotationMaxtrix();

		glm::vec2 squareCorners[4] =
		{
			(rotMatrix * glm::vec3(squareMin.x, squareMin.y, 0)),
			(rotMatrix * glm::vec3(squareMin.x, squareMax.y, 0)),
			(rotMatrix * glm::vec3(squareMax.x, squareMax.y ,0)),
			(rotMatrix * glm::vec3(squareMax.x, squareMin.y, 0)),
		};
		for (int i = 0; i < 4; i++)
		{
			squareCorners[i] += square->GetPosition();
		}

		std::vector<glm::vec2> customPoints = custom->GetPoints();

		// SAT
		std::vector<glm::vec2> normals;
		for (int i = 0; i < 4; i++)
		{
			glm::vec2 side = squareCorners[i] - squareCorners[i + 1 < 4 ? i + 1 : 0];
			side = glm::normalize(side);
			normals.push_back(glm::vec2(side.y, -side.x));
		}
		for (int i = 0; i < customPoints.size(); i++)
		{
			glm::vec2 side = customPoints[i] - customPoints[i + 1 < 4 ? i + 1 : 0];
			side = glm::normalize(side);
			normals.push_back(glm::vec2(side.y, -side.x));
		}

		std::vector<float> overlap;
		for (int i = 0; i < normals.size(); i++)
		{
			float squareExtents[4];
			std::vector<float> customExtents;

			for (int j = 0; j < 4; j++)
			{
				squareExtents[j] = glm::dot(squareCorners[j], normals[i]);
			}
			for (int j = 0; j < customPoints.size(); j++)
			{
				customExtents.push_back(glm::dot(customPoints[j], normals[i]));
			}

			float squareExtentsMax = squareExtents[0];
			float squareExtentsMin = squareExtents[0];
			for (int j = 0; j < 4; j++)
			{
				if (squareExtents[j] > squareExtentsMax)
					squareExtentsMax = squareExtents[j];
				if (squareExtents[j] < squareExtentsMin)
					squareExtentsMin = squareExtents[j];
			}			

			float customExtentsMax = customExtents[0];
			float customExtentsMin = customExtents[0];
			for (int j = 0; j < customExtents.size(); j++)
			{
				if (customExtents[j] > customExtentsMax)
					customExtentsMax = customExtents[j];
				if (customExtents[j] < customExtentsMin)
					customExtentsMin = customExtents[j];
			}

			if (fmin(squareExtentsMax - customExtentsMin, customExtentsMax - squareExtentsMin) < 0)
				return output;

			overlap.push_back(fmin(squareExtentsMax - customExtentsMin, customExtentsMax - squareExtentsMin));
		}

		int overlapMin = 0;
		for (int i = 1; i < overlap.size(); i++)
		{
			if (overlap[i] < overlap[overlapMin])
				overlapMin = i;
		}

		glm::vec2 direction = custom->GetPosition() - square->GetPosition();
		direction = glm::normalize(direction);

		if (glm::dot(normals[overlapMin], direction) < 0)
			normals[overlapMin] *= -1;

		output.m_collided = true;
		output.m_collisionNormal = normals[overlapMin];
		//

		float massRatio = square->GetMass() / (custom->GetMass() + square->GetMass());

		if (!custom->GetKinematic() && !square->GetKinematic())
		{
			custom->SetPosition(custom->GetPosition() + (normals[overlapMin] * overlap[overlapMin] * massRatio));
			square->SetPosition(square->GetPosition() - (normals[overlapMin] * overlap[overlapMin] * (1 - massRatio)));
		}
		else if (!custom->GetKinematic() && square->GetKinematic())
		{
			custom->SetPosition(custom->GetPosition() + (normals[overlapMin] * overlap[overlapMin]));
		}
		else if (custom->GetKinematic() && !square->GetKinematic())
		{
			square->SetPosition(square->GetPosition() - (normals[overlapMin] * overlap[overlapMin]));
		}

		square->ResolveCollision(custom, output);

		return output;
	}

	return output;
}

CollisionArgs PhysicsScene::Custom2Line(PhysicsObject* a, PhysicsObject* b)
{
	return Line2Custom(b, a);
}

CollisionArgs PhysicsScene::Custom2Circle(PhysicsObject* a, PhysicsObject* b)
{
	return Circle2Custom(b, a);
}

CollisionArgs PhysicsScene::Custom2Square(PhysicsObject* a, PhysicsObject* b)
{
	return Square2Custom(b, a);
}

CollisionArgs PhysicsScene::Custom2Custom(PhysicsObject* a, PhysicsObject* b)
{
	CustomShape* customA = dynamic_cast<CustomShape*>(a);
	CustomShape* customB = dynamic_cast<CustomShape*>(b);
	CollisionArgs output;

	if (glm::all(glm::lessThan(customA->GetMin(), customB->GetMax())) && glm::all(glm::greaterThan(customA->GetMax(), customB->GetMin())))
	{
		std::vector<glm::vec2> customAPoints = customA->GetPoints();
		std::vector<glm::vec2> customBPoints = customB->GetPoints();

		std::vector<glm::vec2> normals;
		for (int i = 0; i < customAPoints.size(); i++)
		{
			glm::vec2 side = customAPoints[i] - customAPoints[i + 1 < customAPoints.size() ? i + 1 : 0];
			side = glm::normalize(side);
			normals.push_back(glm::vec2(side.y, -side.x));
		}
		for (int i = 0; i < customBPoints.size(); i++)
		{
			glm::vec2 side = customBPoints[i] - customBPoints[i + 1 < customBPoints.size() ? i + 1 : 0];
			side = glm::normalize(side);
			normals.push_back(glm::vec2(side.y, -side.x));
		}

		std::vector<float> overlap;
		for (int i = 0; i < normals.size(); i++)
		{
			std::vector<float> AExtents;
			std::vector<float> BExtents;

			for (int j = 0; j < customAPoints.size(); j++)
			{
				AExtents.push_back(glm::dot(customAPoints[j], normals[i]));
			}
			for (int j = 0; j < customBPoints.size(); j++)
			{
				BExtents.push_back(glm::dot(customBPoints[j], normals[i]));
			}

			float AExtentsMax = AExtents[0];
			float AExtentsMin = AExtents[0];

			float BExtentsMax = BExtents[0];
			float BExtentsMin = BExtents[0];

			for (int j = 0; j < 4; j++)
			{
				if (AExtents[j] > AExtentsMax)
					AExtentsMax = AExtents[j];
				if (AExtents[j] < AExtentsMin)
					AExtentsMin = AExtents[j];

				if (BExtents[j] > BExtentsMax)
					BExtentsMax = BExtents[j];
				if (BExtents[j] < BExtentsMin)
					BExtentsMin = BExtents[j];
			}

			if (fmin(AExtentsMax - BExtentsMin, BExtentsMax - AExtentsMin) < 0)
				return output;

			overlap.push_back(fmin(AExtentsMax - BExtentsMin, BExtentsMax - AExtentsMin));

		}

		int overlapMin = 0;
		for (int i = 1; i < 4; i++)
		{
			if (overlap[i] < overlap[overlapMin])
				overlapMin = i;
		}

		glm::vec2 direction = customB->GetPosition() - customA->GetPosition();
		direction = glm::normalize(direction);

		if (glm::dot(normals[overlapMin], direction) < 0)
			normals[overlapMin] *= -1;

		output.m_collided = true;
		output.m_collisionNormal = normals[overlapMin];
		//

		float massRatio = customA->GetMass() / (customB->GetMass() + customA->GetMass());

		if (!customB->GetKinematic() && !customA->GetKinematic())
		{
			customB->SetPosition(customB->GetPosition() + (normals[overlapMin] * overlap[overlapMin] * massRatio));
			customA->SetPosition(customA->GetPosition() - (normals[overlapMin] * overlap[overlapMin] * (1 - massRatio)));
		}
		else if (!customB->GetKinematic() && customA->GetKinematic())
		{
			customB->SetPosition(customB->GetPosition() + (normals[overlapMin] * overlap[overlapMin]));
		}
		else if (customB->GetKinematic() && !customA->GetKinematic())
		{
			customA->SetPosition(customA->GetPosition() - (normals[overlapMin] * overlap[overlapMin]));
		}

		customA->ResolveCollision(customB, output);

		return output;
	}

	return output;
}
