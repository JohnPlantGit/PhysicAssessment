#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

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
};