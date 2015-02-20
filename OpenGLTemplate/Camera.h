#pragma once
//System includes
#include <cmath>

//Custom includes
#include "OpenGL.h"
#include "Transform.h"
#include "Constants.h"

class Camera
{
public:
	//Attributes
	Transform* transform;

	//Constructors/Destructors
	Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f));
	~Camera(void);

	//Methods
	void setProjectionData(glm::vec4 projectionData);
	void bindUniforms(const GLuint program, const GLuint cameraPosition, const GLuint projectionMatrix, const GLuint viewMatrix);
	glm::vec3 getLookPosition();

private:
	//Attributes
	glm::vec4 projectionData;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	//Methods
	void updateViewMatrix();
	void updateProjectionMatrix();
};

