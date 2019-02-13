#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include "Circle.h"
#include "Line.h"
#include "Square.h"
#include <math.h>

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

	Circle* ball = new Circle(glm::vec2(-49, -49), glm::vec2(0, 0), 2.0f, 2, glm::vec4(0, 1, 0, 1));
	//ball->SetElasticity(0.2f);
	//ball->SetLinearDrag(0.3f);
	ball->SetKinematic(true);
	Circle* ball2 = new Circle(glm::vec2(-48, -30), glm::vec2(0, 0), 2, 2, glm::vec4(1, 0, 0, 1));
	//ball2->SetElasticity(0.2f);
	//ball2->SetLinearDrag(0.3f);
	Line* line1 = new Line(glm::vec2(0, -1), -50);
	Line* line2 = new Line(glm::vec2(0, 1), -50);
	Line* line3 = new Line(glm::vec2(-1, 0), -90);
	Line* line4 = new Line(glm::vec2(1, 0), -90);
	Line* line5 = new Line(glm::vec2(-0.707, 0.707), -40);
	m_physicsScene->AddActor(ball);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(line1);
	m_physicsScene->AddActor(line2);
	m_physicsScene->AddActor(line3);
	m_physicsScene->AddActor(line4);
	m_physicsScene->AddActor(line5);

	const int ballCount = 20;

	for (int i = 0; i < ballCount; i++)
	{
		Circle* newBall = new Circle(glm::vec2(-40 + i * 3, i * 3), glm::vec2(30, 30), 2.0f, 2, glm::vec4(1, 0, 0, 1));
		m_physicsScene->AddActor(newBall);
	}

	Square* square = new Square(glm::vec2(-20, -30), glm::vec2(0, 0), 500, 10, 10, glm::vec4(0, 0, 1, 1));
	square->SetKinematic(true);
	//square->SetElasticity(0.5f);
	m_physicsScene->AddActor(square);

	return true;
}

void Application2D::shutdown() 
{
	delete m_2dRenderer;
	delete m_physicsScene;
}

void Application2D::update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();
	//ProjectileDemo(glm::vec2(-80, -10), 45, 40, -9.8f);

	m_timer += deltaTime;

	m_physicsScene->Update(deltaTime);
	m_physicsScene->UpdateGizmos();

	if (input->wasMouseButtonReleased(0) && m_m1Pressed)
	{
		glm::vec2 parallel(m_newLineNormal.y, -m_newLineNormal.x);
		
		glm::vec2 negNormal = -m_newLineNormal;
		float t = std::fmaxf(0, std::fminf(glm::dot(m_newLinePos, negNormal), 999));
		if (t == 0)
		{
			t = std::fmaxf(0, std::fminf(glm::dot(m_newLinePos, -negNormal), 999));
			t *= -1;
		}

		Line* newLine = new Line(m_newLineNormal, -t);
		m_physicsScene->AddActor(newLine);

		m_m1Pressed = false;
	}
	if (input->isMouseButtonDown(0) && m_m1Pressed)
	{
		glm::vec2 mousePos(input->getMouseX(), input->getMouseY());
		mousePos -= glm::vec2(640, 360);
		mousePos *= (100.0f / 640.0f);

		m_newLineNormal = glm::normalize(mousePos - m_newLinePos);
		glm::vec2 parallel(m_newLineNormal.y, -m_newLineNormal.x);

		float lineLength = 300;

		glm::vec2 start = m_newLinePos + (parallel * lineLength);
		glm::vec2 end = m_newLinePos - (parallel * lineLength);

		aie::Gizmos::add2DLine(start, end, glm::vec4(1, 1, 1, 1));

		
	}
	if (input->wasMouseButtonPressed(0))
	{
		m_m1Pressed = true;

		glm::vec2 mousePos(input->getMouseX(), input->getMouseY());
		mousePos -= glm::vec2(640, 360);
		mousePos *= (100.0f / 640.0f);

		m_newLinePos = mousePos;
	}

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

	aie::Input* input = aie::Input::getInstance();
	glm::vec2 mousePos(input->getMouseX(), input->getMouseY());

	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	mousePos -= glm::vec2(640, 360);
	mousePos *= (100.0f/640.0f);
	printf("%f, %f\n", mousePos.x, mousePos.y);
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

	inclination *= 0.0174533f;

	while (t <= 5)
	{
		float x = (startPos.x + (cosf(inclination) * speed) * t);
		float y = (startPos.y + ((sinf(inclination) * speed) * t) + (0.5f * gravity * t * t));

		aie::Gizmos::add2DCircle(glm::vec2(x, y), radius, 12, colour);

		t += tStep;
	}
}