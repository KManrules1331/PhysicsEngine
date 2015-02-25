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
GameObject* obj1;
std::vector<MoveCommand> commands;
float framesPerSecond;

using namespace std;
void calculateFPS()
{
	static int previousTime = 0;

	int currentTime = glutGet(GLUT_ELAPSED_TIME);

	float secondsPerFrame = currentTime - previousTime;

	framesPerSecond = 1000.0f / secondsPerFrame;

	previousTime = currentTime;
}
//TODO: Find out best place to put this method
void spawnSphere()
{
	//GameObject* obj = new GameObject(GameObject::Primitive::Sphere, cam->getLookPosition(), glm::vec3(0.0f), glm::vec3(1.0f));
	//scene1->addObject(obj);
}
void mouseclick(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			{
				spawnSphere();
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
	scene1->addObject(new GameObject(GameObject::Primitive::Cube, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
	obj1 = new GameObject(GameObject::Primitive::Cube, glm::vec3(1.5f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	obj1->transform->rotate(glm::vec3(45.0f, 45.0f, 45.0f));
	scene1->addObject(obj1);
	glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, -3.0f);
	cam->transform->setPosition(camPosition);
}

void update(void)
{
	scene1->updateScene();
	if (Input::KeyPressed('w'))
	{
		obj1->transform->move(glm::vec3(0.0f, 0.0f, 0.01f));
	}
	for (int i = 0; i < commands.size(); i++)
	{
		commands[i].execute();
	}
	commands.clear();
	//obj1->transform->rotate(glm::vec3(0.05f, 0.05f, 0.05f));
	calculateFPS();
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

	//Test Vars
	Mesh::init(scene1->shader);
	Collider::init();
	init();

	glutMainLoop();
	return 0;
}