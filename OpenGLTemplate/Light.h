#pragma once
//System includes

//Custom includes
#include "OpenGL.h"

class Light
{
public:
	//Attributes
	glm::vec3 position;

	//Constructor / Destructor
	Light(glm::vec3 position = glm::vec3(0.0f, 3.0f, 0.0f));
	~Light(void);

	//Methods
	void bindUniform(const GLuint program, const GLuint lightPosition);
};

