#include "Window.h"


Window::Window(char* name, int width, int height, glm::vec4 color)
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	this->width = width; this->height = height;
	glutCreateWindow(name);
	glClearColor(color.r, color.g, color.b, color.a);
}


Window::~Window(void)
{
}
