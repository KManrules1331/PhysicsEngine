#include "Light.h"


Light::Light(glm::vec3 position)
{
	this->position = position;
}


Light::~Light(void)
{
}

#pragma region Methods

#pragma region Public

void Light::bindUniform(const GLuint program, const GLuint lightPosition)
{
	glProgramUniform3fv(program, lightPosition, 1, &position[0]);
}

#pragma endregion

#pragma region Private



#pragma endregion

#pragma endregion
