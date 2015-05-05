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

Window* window;
Scene* scene1;
Camera* cam;
GameObject* obj, *obj1;
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

	previousTime = currentTime;
}

void init(void)
{
	obj = new GameObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	obj->setMesh(Mesh::cubeMesh);
	obj->addCollisionDetector(CollisionDetector::DetectorType::Cube);
	obj->addPhysicsComponent(1.0f, 0.1f);
	scene1->addObject(obj);
	obj1 = new GameObject(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	obj1->setMesh(Mesh::cubeMesh);
	obj1->addCollisionDetector(CollisionDetector::DetectorType::Cube);
	obj1->addPhysicsComponent(0.0f, 0.1f);
	scene1->addObject(obj1);
	glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, -3.0f);
	cam->transform->setPosition(camPosition);
}

void update(void)
{
	calculateFPS();
	scene1->updateScene(dt);
	if (Input::KeyPressed('w'))
	{
		obj->transform->move(glm::vec3(0.0f, 0.0f, 0.01f));
	}
	if (Input::KeyPressed('a'))
	{
		obj->transform->move(glm::vec3(-0.01f, 0.0f, 0.0f));
	}
	if (Input::KeyPressed('s'))
	{
		obj->transform->move(glm::vec3(0.0f, 0.0f, -0.01f));
	}
	if (Input::KeyPressed('d'))
	{
		obj->transform->move(glm::vec3(0.01f, 0.0f, 0.0f));
	}
	if (Input::KeyPressed('t'))
	{
		obj->physicsComponent->addForce(glm::vec3(500.0f, 0.0f, 0.0f), obj->transform->getPosition());
	}
	obj->physicsComponent->addForce(glm::normalize(obj1->transform->getPosition() - obj->transform->getPosition()) * 0.1f, obj->transform->getPosition());
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
	glutMouseFunc(Input::recordMouseClick);
	glutPassiveMotionFunc(Input::recordMousePos);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutKeyboardFunc(Input::recordKeyboardPress);
	glutKeyboardUpFunc(Input::recordKeyboardRelease);
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