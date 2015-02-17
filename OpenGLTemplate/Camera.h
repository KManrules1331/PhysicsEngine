#pragma once
//System includes
#include <cmath>

//Custom includes
#include "OpenGL.h"
#include "Constants.h"

class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	~Camera(void);

	void setPosition(float x, float y, float z);
	void move(float dX, float dY, float dZ);
	void look(float dPitch, float dYaw);
	void setProjectionData(float* data);
	void setViewData(float* position, float* direction, float* up);
	void bindUniforms(const GLuint program, const GLuint cameraPosition, const GLuint projectionMatrix, const GLuint viewMatrix);

	glm::vec3 getLookPosition();

private:
	glm::vec3 position;
	glm::vec3 forward, up, right;
	glm::vec4 projectionData;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	void updateViewMatrix();
	void updateProjectionMatrix();
};

