#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include "Circle.h"

Application2D::Application2D() 
{

}

Application2D::~Application2D()
{

}

bool Application2D::startup()
{	
	m_2dRenderer = new aie::Renderer2D();
	
	m_cameraX = 0;
	m_cameraY = 0;
	m_timer = 0;

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(30, 30), 2.0f, 2, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(-10, 30), glm::vec2(-15, -15), 1.0f, 2, glm::vec4(0, 1, 0, 1));
	m_physicsScene->AddActor(ball);
	m_physicsScene->AddActor(ball2);

	return true;
}

void Application2D::shutdown() 
{
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime)
{
	aie::Gizmos::clear();

	m_timer += deltaTime;

	m_physicsScene->Update(deltaTime);
	m_physicsScene->UpdateGizmos();

	// input example
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

}

void Application2D::draw()
{

	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// done drawing sprites
	m_2dRenderer->end();
}