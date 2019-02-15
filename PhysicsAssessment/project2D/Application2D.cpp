#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include "Circle.h"
#include "Line.h"
#include "Square.h"
#include <math.h>
#include "Spring.h"

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

	Circle* ball = new Circle(glm::vec2(40, 5), glm::vec2(0, 0), 2, 2, glm::vec4(0, 1, 0, 1));
	ball->SetElasticity(0.0f);
	ball->SetLinearDrag(0.3f);
	//ball->SetKinematic(true);
	Circle* ball2 = new Circle(glm::vec2(50, 5), glm::vec2(0, 0), 2, 2, glm::vec4(1, 0, 0, 1));
	ball2->SetElasticity(0.3f);
	ball2->SetLinearDrag(0.3f);

	Spring* spring1 = new Spring(ball, ball2, 15, 1);
	m_physicsScene->AddActor(spring1);

	mouseSpring = new Spring(ball, nullptr, 5, 0.5);
	aie::Input* input = aie::Input::getInstance();
	glm::vec2 mousePos(input->getMouseX(), input->getMouseY());
	mousePos -= glm::vec2(640, 360);
	mousePos *= (100.0f / 640.0f);
	mouseSpring->SetContact2(mousePos);
	//m_physicsScene->AddActor(mouseSpring);

	Circle* tl = new Circle(glm::vec2(-10, 10), glm::vec2(0, 0), 2, 2, glm::vec4(1, 1, 1, 1));
	Circle* tr = new Circle(glm::vec2(10, 10), glm::vec2(0, 0), 2, 2, glm::vec4(1, 1, 1, 1));
	Circle* br = new Circle(glm::vec2(10, -10), glm::vec2(0, 0), 2, 2, glm::vec4(1, 1, 1, 1));
	Circle* bl = new Circle(glm::vec2(-10, -10), glm::vec2(0, 0), 2, 2, glm::vec4(1, 1, 1, 1));

	// structural
	Spring* tl2tr = new Spring(tl, tr, 10, 50);
	Spring* tr2br = new Spring(tr, br, 10, 50);
	Spring* br2bl = new Spring(br, bl, 10, 50);
	Spring* bl2tl = new Spring(bl, tl, 10, 50);
	// shear							   
	Spring* tl2br = new Spring(tl, br, 14, 50);
	Spring* tr2bl = new Spring(tr, bl, 14, 50);

	m_physicsScene->AddActor(tl);
	m_physicsScene->AddActor(tr);
	m_physicsScene->AddActor(br);
	m_physicsScene->AddActor(bl);

	m_physicsScene->AddActor(tl2tr);
	m_physicsScene->AddActor(tr2br);
	m_physicsScene->AddActor(br2bl);
	m_physicsScene->AddActor(bl2tl);

	m_physicsScene->AddActor(tl2br);
	m_physicsScene->AddActor(tr2bl);

	
	Line* line1 = new Line(glm::vec2(0, -1), -50);
	Line* line2 = new Line(glm::vec2(0, 1), -50);
	Line* line3 = new Line(glm::vec2(-1, 0), -90);
	Line* line4 = new Line(glm::vec2(1, 0), -90);
	Line* line5 = new Line(glm::normalize(glm::vec2(-2, - 1)), 50);
	m_physicsScene->AddActor(ball);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(line1);
	m_physicsScene->AddActor(line2);
	m_physicsScene->AddActor(line3);
	m_physicsScene->AddActor(line4);
	m_physicsScene->AddActor(line5);

	//Square* square = new Square(glm::vec2(-50, 5), glm::vec2(0, 0), 2, 10, 10, glm::vec4(0, 0, 1, 1));
	//square->SetKinematic(true);
	//square->SetElasticity(0);
	//m_physicsScene->AddActor(square);
	/*Square* square2 = new Square(glm::vec2(50, -10), glm::vec2(0, 10), 2, 5, 5, glm::vec4(0, 0, 1, 1));
	square2->SetElasticity(0);
	m_physicsScene->AddActor(square2);
	Square* square3 = new Square(glm::vec2(50, 20), glm::vec2(0, -10), 2, 5, 5, glm::vec4(0, 0, 1, 1));
	square3->SetElasticity(0);
	m_physicsScene->AddActor(square3);
	Square* square4 = new Square(glm::vec2(35, 5), glm::vec2(10, 0), 2, 5, 5, glm::vec4(0, 0, 1, 1));
	square4->SetElasticity(0);
	m_physicsScene->AddActor(square4);
	Square* square5 = new Square(glm::vec2(65, 5), glm::vec2(-10, 0), 2, 5, 5, glm::vec4(0, 0, 1, 1));
	square5->SetElasticity(0);
	m_physicsScene->AddActor(square5);*/

	const int ballCount = 0;

	for (int i = 0; i < ballCount; i++)
	{
		Circle* newBall = new Circle(glm::vec2(-40 + i * 3, i * 3), glm::vec2(30, 30), 2.0f, 2, glm::vec4(1, 0, 0, 1));
		m_physicsScene->AddActor(newBall);
	}

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

	glm::vec2 mousePos(input->getMouseX(), input->getMouseY());
	mousePos -= glm::vec2(640, 360);
	mousePos *= (100.0f / 640.0f);
	mouseSpring->SetContact2(mousePos);

	if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
	{
		if (input->wasMouseButtonReleased(0))
		{
			std::vector<PhysicsObject*> objectsHit = m_physicsScene->CheckPoint(mousePos);
			for (int i = 0; i < objectsHit.size(); i++)
			{
				m_physicsScene->RemoveActor(objectsHit[i]);
			}
		}
	}
	else
	{
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

			m_newLinePos = mousePos;
		}
	}
	if (input->wasMouseButtonReleased(1) && m_m2Pressed)
	{
		Circle* newball = new Circle(m_newBallPos, m_newBallDirection, 2, 2, glm::vec4(1, 1, 1, 1));
		newball->SetElasticity(0.0f);
		m_physicsScene->AddActor(newball);
	}
	if (input->isMouseButtonDown(1) && m_m2Pressed)
	{
		m_newBallDirection = mousePos - m_newBallPos;

		glm::vec2 parallel(m_newBallDirection.y, -m_newBallDirection.x);

		aie::Gizmos::add2DLine(m_newBallPos, m_newBallPos + m_newBallDirection, glm::vec4(1, 1, 1, 1));
	}
	if (input->wasMouseButtonPressed(1))
	{
		m_newBallPos = mousePos;
		m_newBallDirection = glm::vec2(0, 0);
		m_m2Pressed = true;
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
	static glm::vec2 prevMousePos;

	if (mousePos != prevMousePos)
		//printf("%f, %f\n", mousePos.x, mousePos.y);

	prevMousePos = mousePos;
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