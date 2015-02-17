#pragma once
//System includes

//Custom includes
#include "OpenGL.h"

class Window
{
public:
	//Attributes
	int width, height;

	//Constructors / Destructors
	Window(char* name = "OpenGL Project", int width = 800, int height = 600, glm::vec4 color = glm::vec4(.392, .584, .929, 1.0));
	~Window(void);
};

