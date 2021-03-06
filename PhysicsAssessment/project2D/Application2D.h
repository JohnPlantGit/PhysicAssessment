#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include <vector>

class Line;
class Spring;

class Application2D : public aie::Application 
{
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void ProjectileDemo(glm::vec2 startPos, float inclination, float speed, float gravity);
protected:

	aie::Renderer2D*	m_2dRenderer;

	float m_cameraX, m_cameraY;
	float m_timer;

	PhysicsScene* m_physicsScene;
	std::vector<Line*> m_lines;

	int m_selectedShape = 0;
	float m_scrollBefore;

	bool m_m1Pressed;
	glm::vec2 m_newLinePos;
	glm::vec2 m_newLineNormal;

	bool m_m2Pressed;
	glm::vec2 m_newBallPos;
	glm::vec2 m_newBallDirection;

	Spring* mouseSpring1;
	Spring* mouseSpring2;
};