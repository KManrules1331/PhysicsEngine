//System includes
#include <iostream>
#include <iomanip>

//Custom includes
#include "OpenGL.h"
#include "Window.h"
#include "Scene.h"
#include "Mesh.h"
#include "GJKCollisionDetection.h"
#include "CommandTypes.h"
#include "Input.h"
#include "SoftBodySphere.h"

Window window;
Scene scene1;
Camera cam;
GameObject a;
SoftBodySphere* s;
float framesPerSecond;
float dt;

using namespace std;
void calculateFPS()
{
	static int previousTime = 0;

	int currentTime = glutGet(GLUT_ELAPSED_TIME);

	float milliSecondsPerFrame = currentTime - previousTime;

	framesPerSecond = 1000.0f / milliSecondsPerFrame;
	dt = milliSecondsPerFrame / 1000.0f;

	//For debug purposes, dt should never be
	//above .1
	dt = glm::clamp(dt, 0.0f, 0.1f);

	previousTime = currentTime;
}
void mouseclick(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			{
				break;
			}
		case GLUT_RIGHT_BUTTON:
			{
				break;
			}
		}
	}
	glutPostRedisplay();
}
void mouselook(int x, int y)
{}

void init(void)
{
	a.setTransform(Transform(glm::vec3(0.0f, 4.0f, 0.0f), glm::quat(), glm::vec3(3.0f)));
	a.setMesh(Mesh::sphereMesh);
	a.setColor(255, 0, 0, 255);
	a.addCollisionDetector(CollisionDetector::DetectorType::Sphere);
	a.addPhysicsComponent(4.0f, 1.0f);
	a.physicsComponent->setVelocity(glm::vec3(-0.05f, -0.8f, 0.0f));
	scene1.addObject(&a);
	s = new SoftBodySphere(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(3.0f));
	scene1.addObject(s);
	glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, -3.0f);
	cam.m_transform.setPosition(camPosition);
}

void update(void)
{
	calculateFPS();
	scene1.updateScene(dt);
	if (Input::KeyPressed('w'))
	{
		cam.m_transform.move(cam.m_transform.getForward() * 0.25f);
		//obj->transform->move(glm::vec3(0.0f, 0.0f, 0.01f));
	}
	if (Input::KeyPressed('a'))
	{
		cam.m_transform.move(cam.m_transform.getRight() * -0.25f);
		//obj->transform->move(glm::vec3(-0.01f, 0.0f, 0.0f));
	}
	if (Input::KeyPressed('s'))
	{
		cam.m_transform.move(cam.m_transform.getForward() * -0.25f);
		//obj->transform->move(glm::vec3(0.0f, 0.0f, -0.01f));
	}
	if (Input::KeyPressed('d'))
	{
		cam.m_transform.move(cam.m_transform.getRight() * 0.25f);
		//obj->transform->move(glm::vec3(0.01f, 0.0f, 0.0f));
	}
	if (Input::KeyPressed('q'))
	{
		cam.m_transform.move(cam.m_transform.getUp() * 0.25f);
	}
	if (Input::KeyPressed('e'))
	{
		cam.m_transform.move(cam.m_transform.getUp() * -0.25f);
	}
	if (Input::KeyPressed('t'))
	{
		//obj->physicsComponent->addForce(glm::vec3(500.0f, 0.0f, 0.0f), obj->transform->getPosition());
	}
	glutPostRedisplay();
}

void draw(void)
{
	scene1.drawScene();
	std::cout << std::fixed << "FPS: " << std::setprecision(4) << framesPerSecond << endl;
}

void quit(void)
{
	glutSetKeyRepeat(GLUT_KEY_REPEAT_DEFAULT);
	exit(0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	window.Init();
	glewInit();
	Light light(glm::vec3(0.0f, 3.0f, 0.0f));
	scene1.setCamera(&cam);
	scene1.setLight(&light);
	scene1.Init();
	glutMouseFunc(mouseclick);
	glutPassiveMotionFunc(mouselook);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutKeyboardFunc(Input::recordKeyboardPress);
	glutKeyboardUpFunc(Input::recordKeyboardRelease);
	glutIdleFunc(update);
	glutDisplayFunc(draw);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	//Initializers
	Mesh::init(scene1.shader);
	CollisionDetector::init();
	init();

	glutMainLoop();
	return 0;
}
