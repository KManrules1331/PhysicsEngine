#include "Camera.h"

#pragma region Constructors

Camera::Camera(glm::vec3 position, glm::vec3 rotation)
{

#pragma region Initialize Projection Matrix

	projectionMatrix = glm::mat4(0.0f);
	projectionMatrix[3][2] = -1.0f;

#pragma endregion

#pragma region Initialize View Matrix

	viewMatrix = glm::mat4(0.0f);
	viewMatrix[3][3] = 1.0f;

#pragma endregion

	this->transform = new Transform(position, rotation);
	//Called to initialize matrices
	projectionData = glm::vec4(1.0f, 10.0f, 1.0f, 1.0f);
}

#pragma endregion

#pragma region Destructors

Camera::~Camera(void)
{
	delete transform;
}

#pragma endregion

#pragma region Methods

#pragma region Public

void Camera::setProjectionData(glm::vec4 projectionData)
{
	this->projectionData = projectionData;
}

void Camera::bindUniforms(const GLuint program, const GLuint cameraPosition, const GLuint projectionMatrix, const GLuint viewMatrix)
{
	updateViewMatrix();
	updateProjectionMatrix();
	glProgramUniform3fv(program, cameraPosition, 1, &(transform->getPosition()[0]));
	glProgramUniformMatrix4fv(program, projectionMatrix, 1, GL_FALSE, &(this->projectionMatrix[0][0]));
	glProgramUniformMatrix4fv(program, viewMatrix, 1, GL_FALSE, &(this->viewMatrix[0][0]));
}

glm::vec3 Camera::getLookPosition()
{
	return transform->getPosition() + transform->getForward();
}

#pragma endregion

#pragma region Private

void Camera::updateViewMatrix()
{
	glm::vec3 forward = transform->getForward();
	glm::vec3 right = transform->getRight();
	glm::vec3 up = transform->getUp();
	glm::vec3 position = transform->getPosition();
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