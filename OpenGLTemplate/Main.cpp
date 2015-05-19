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
#include "Cloth.h"

Window* window;
Scene* scene1;
Camera* cam;
GameObject* a;
Cloth* t;
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
	a = new GameObject(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.1f));
	a->setMesh(Mesh::sphereMesh);
	a->addCollisionDetector(CollisionDetector::DetectorType::Sphere);
	a->addPhysicsComponent(1.0f, 1.0f);
	scene1->addObject(a);
	GameObject* b = new GameObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.1f));
	b->setMesh(Mesh::sphereMesh);
	b->addCollisionDetector(CollisionDetector::DetectorType::Sphere);
	b->addPhysicsComponent(0.0f, 0.0f);
	scene1->addObject(b);
	Spring* s = new Spring(*(a->physicsComponent), *(b->physicsComponent), glm::vec3(0.0f), glm::vec3(0.0f), 0.1f);
	s->setMesh(Mesh::cubeMesh);
	scene1->addObject(s);
	glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, -3.0f);
	cam->transform->setPosition(camPosition);
	Input::OnKeyboardPress('y', new MoveCommand(glm::vec3(2.0f, 0.0f, 0.0f), *a));
}

void update(void)
{
	calculateFPS();
	scene1->updateScene(dt);
	if (Input::KeyPressed('w'))
	{
		//obj->transform->move(glm::vec3(0.0f, 0.0f, 0.01f));
	}
	if (Input::KeyPressed('a'))
	{
		//obj->transform->move(glm::vec3(-0.01f, 0.0f, 0.0f));
	}
	if (Input::KeyPressed('s'))
	{
		//obj->transform->move(glm::vec3(0.0f, 0.0f, -0.01f));
	}
	if (Input::KeyPressed('d'))
	{
		//obj->transform->move(glm::vec3(0.01f, 0.0f, 0.0f));
	}
	if (Input::KeyPressed('t'))
	{
		//obj->physicsComponent->addForce(glm::vec3(500.0f, 0.0f, 0.0f), obj->transform->getPosition());
	}
	glutPostRedisplay();
}

void draw(void)
{
	scene1->drawScene();
	std::cout << std::fixed << "FPS: " << std::setprecision(4) << framesPerSecond << endl;
}

void quit(void)
{
	delete window;
	delete scene1;
	glutSetKeyRepeat(GLUT_KEY_REPEAT_DEFAULT);
	exit(0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	window = new Window();
	glewInit();
	cam = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));
	Light* light = new Light(glm::vec3(0.0f, 3.0f, 0.0f));
	scene1 = new Scene(cam, light);
	glutMouseFunc(mouseclick);
	glutPassiveMotionFunc(mouselook);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutKeyboardFunc(Input::getKeyboardPress);
	glutKeyboardUpFunc(Input::getKeyboardRelease);
	glutIdleFunc(update);
	glutDisplayFunc(draw);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	//Initializers
	Mesh::init(scene1->shader);
	CollisionDetector::init();
	init();

	glutMainLoop();
	return 0;
}