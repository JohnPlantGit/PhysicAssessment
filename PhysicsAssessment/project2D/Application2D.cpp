#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include "Circle.h"
#include "Line.h"
#include "Square.h"

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
	m_physicsScene->SetGravity(glm::vec2(0, -100));

	Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(30, 30), 2.0f, 2, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(-10, 30), glm::vec2(-20, -20), 8.0f, 2, glm::vec4(0, 1, 0, 1));
	Line* line1 = new Line(glm::vec2(0, 1), 50);
	Line* line2 = new Line(glm::vec2(0, -1), 50);
	Line* line3 = new Line(glm::vec2(1, 0), 90);
	Line* line4 = new Line(glm::vec2(-1, 0), 90);
	m_physicsScene->AddActor(ball);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(line1);
	m_physicsScene->AddActor(line2);
	m_physicsScene->AddActor(line3);
	m_physicsScene->AddActor(line4);

	const int ballCount = 10;

	for (int i = 0; i < ballCount; i++)
	{
		Circle* newBall = new Circle(glm::vec2(-40 + i * 3, i * 3), glm::vec2(30, 30), 2.0f, 2, glm::vec4(1, 0, 0, 1));
		m_physicsScene->AddActor(newBall);
	}

	Square* square = new Square(glm::vec2(40, 10), glm::vec2(-20, -20), 1.0f, 5, 5, glm::vec4(0, 0, 1, 1));
	m_physicsScene->AddActor(square);

	return true;
}

void Application2D::shutdown() 
{
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime)
{
	aie::Gizmos::clear();
	//ProjectileDemo(glm::vec2(-80, -10), 45, 40, -9.8f);

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

void Application2D::ProjectileDemo(glm::vec2 startPos, float inclination, float speed, float gravity)
{
	float t = 0;
	float tStep = 0.5f;
	float radius = 1.0f;
	int segments = 12;
	glm::vec4 colour = glm::vec4(1, 1, 0, 1);

	while (t <= 5)
	{
		float x = (startPos.x + (cosf(inclination) * speed) * t);
		float y = (startPos.y + ((sinf(inclination) * speed) * t) + (0.5 * gravity * t * t));

		aie::Gizmos::add2DCircle(glm::vec2(x, y), radius, 12, colour);

		t += tStep;
	}
}