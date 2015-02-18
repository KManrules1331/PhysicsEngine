#include "Transform.h"


Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	updateTransformMatrix();
}


Transform::~Transform()
{
}

void Transform::move(glm::vec3 displacement)
{
	position += displacement;
	updateTransformMatrix();
}
void Transform::rotate(glm::vec3 rotation)
{
	this->rotation += rotation;
	updateTransformMatrix();
}
void Transform::changeScale(glm::vec3 scale)
{
	this->scale += scale;
	updateTransformMatrix();
}
void Transform::setPosition(glm::vec3 newPosition)
{
	position = newPosition;
	updateTransformMatrix();
}
void Transform::setRotation(glm::vec3 newRotation)
{
	rotation = newRotation;
	updateTransformMatrix();
}
void Transform::setScale(glm::vec3 newScale)
{
	scale = newScale;
	updateTransformMatrix();
}

#pragma region MatrixUpdaters

void Transform::updateTransformMatrix()
{
	updateRotationMatrix();
	glm::mat4 positionMatrix = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		position.x, position.y, position.z, 1.0f);
	glm::mat4 scaleMatrix = glm::mat4(scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	transformMatrix = positionMatrix * rotationMatrix * scaleMatrix;
}

void Transform::updateRotationMatrix()
{
	glm::vec3 angles = rotation * (PI / 180);
	glm::vec3 cosAngles = glm::vec3(cos(angles.x), cos(angles.y), cos(angles.z));
	glm::vec3 sinAngles = glm::vec3(sin(angles.x), sin(angles.y), sin(angles.z));

	glm::mat4 rotationX = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosAngles.x, sinAngles.x, 0.0f,
		0.0f, -sinAngles.x, cosAngles.x, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 rotationY = glm::mat4(cosAngles.y, 0.0f, -sinAngles.y, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinAngles.y, 0.0f, cosAngles.y, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 rotationZ = glm::mat4(cosAngles.z, -sinAngles.z, 0.0f, 0.0f,
		sinAngles.z, cosAngles.z, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	rotationMatrix = rotationZ * rotationY * rotationX;
}

#pragma endregion