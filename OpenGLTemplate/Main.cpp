//System includes
#include <iostream>
#include <iomanip>

//Custom includes
#include "OpenGL.h"
#include "Window.h"
#include "Scene.h"
#include "Mesh.h"
#include "GJKCollisionDetection.h"

Window* window;
Scene* scene1;
Camera* cam;
GameObject* obj1;
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
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'w': obj1->transform->move(glm::vec3(0.0f, 0.0f, 0.001f));	break;
		case 'a': obj1->transform->move(glm::vec3(-0.001f, 0.0f,  0.0f));	break;
		case 's': obj1->transform->move(glm::vec3(0.0f, 0.0f, -0.001f));	break;
		case 'd': obj1->transform->move(glm::vec3(0.001f, 0.0f, 0.0f));	break;
		case 'q': obj1->transform->move(glm::vec3(0.0f, -0.001f, 0.0f));	break;
		case 'e': obj1->transform->move(glm::vec3(0.0f, 0.001f, 0.0f));	break;
		case 033:	exit(0);	break;
	}
	glutPostRedisplay();
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
	//obj1->transform->rotate(glm::vec3(0.05f, 0.05f, 0.05f));
	calculateFPS();
	glutPostRedisplay();
}

void draw(void)
{
	scene1->drawScene();
	std::cout << std::fixed << "FPS: " << std::setprecision(4) << framesPerSecond << endl;
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
	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);
	glutDisplayFunc(draw);

	//Test Vars
	Mesh::init(scene1->shader);
	Collider::init();
	init();

	glutMainLoop();
	return 0;
}