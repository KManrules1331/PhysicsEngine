#include "Camera.h"

#pragma region Constructors

Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up)
{

#pragma region Initialize Projection Matrix

	projectionMatrix = glm::mat4(0.0f);
	projectionMatrix[3][2] = -1.0f;

#pragma endregion

#pragma region Initialize View Matrix

	viewMatrix = glm::mat4(0.0f);
	viewMatrix[3][3] = 1.0f;

#pragma endregion

	this->position = position;
	this->forward = glm::normalize(forward);
	this->up = glm::normalize(up);
	this->right = glm::cross(this->forward, this->up);
	//Called to initialize matrices
	setViewData(&position[0], &forward[0], &up[0]);
	projectionData = glm::vec4(1.0f, 10.0f, 1.0f, 1.0f);
	setProjectionData(&projectionData[0]);
}

#pragma endregion

#pragma region Destructors

Camera::~Camera(void)
{
}

#pragma endregion

#pragma region Methods

#pragma region Public

void Camera::move(float dX, float dY, float dZ)
{
	this->position += dX * this->right;
	this->position += dY * this->up;
	this->position += dZ * this->forward;
	updateViewMatrix();
}

void Camera::look(float dPitch, float dYaw)
{
	//Broken until further notice.
	updateViewMatrix();
}

void Camera::setViewData(float* position, float* forward, float* up)
{
	this->position = glm::vec3(position[0], position[1], position[2]);
	this->forward = glm::vec3(forward[0], forward[1], forward[2]);
	this->up = glm::vec3(up[0], up[1], up[2]);
	this->right = glm::cross(this->forward, this->up);
	updateViewMatrix();
}

void Camera::setProjectionData(float* projectionData)
{
	this->projectionData.x = projectionData[0];
	this->projectionData.y = projectionData[1];
	this->projectionData.z = projectionData[2];
	this->projectionData.w = projectionData[3];
	updateProjectionMatrix();
}

void Camera::bindUniforms(const GLuint program, const GLuint cameraPosition, const GLuint projectionMatrix, const GLuint viewMatrix)
{
	glProgramUniform3fv(program, cameraPosition, 1, &(this->position[0]));
	glProgramUniformMatrix4fv(program, projectionMatrix, 1, GL_FALSE, &(this->projectionMatrix[0][0]));
	glProgramUniformMatrix4fv(program, viewMatrix, 1, GL_FALSE, &(this->viewMatrix[0][0]));
}

glm::vec3 Camera::getLookPosition()
{
	return this->position + this->forward;
}

#pragma endregion

#pragma region Private

void Camera::updateViewMatrix()
{
	forward = glm::normalize(forward);
	right = glm::normalize(right);
	up = glm::normalize(up);
	viewMatrix[0][0] = right.x;
	viewMatrix[1][0] = right.y;
	viewMatrix[2][0] = right.z;
	viewMatrix[3][0] = glm::dot(-right, position);
	viewMatrix[0][1] = up.x;
	viewMatrix[1][1] = up.y;
	viewMatrix[2][1] = up.z;
	viewMatrix[3][1] = glm::dot(-up, position);
	viewMatrix[0][2] = -forward.x;
	viewMatrix[1][2] = -forward.y;
	viewMatrix[2][2] = -forward.z;
	viewMatrix[3][2] = glm::dot(forward, position);
}

void Camera::updateProjectionMatrix()
{
	projectionMatrix[0][0] = 2 * projectionData.x / projectionData.z;
	projectionMatrix[1][1] = 2 * projectionData.x / projectionData.w;
	projectionMatrix[2][2] = -(projectionData.y + projectionData.x) / (projectionData.y - projectionData.x);
	projectionMatrix[2][3] = -2 * projectionData.y * projectionData.x / (projectionData.y - projectionData.x);
}

#pragma endregion

#pragma endregion