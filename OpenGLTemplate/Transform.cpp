#include "Transform.h"


Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
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
void Transform::rotate(glm::quat rotation)
{
	this->rotation *= rotation;
	updateTransformMatrix();
}
void Transform::rotate(float angle, glm::vec3 axis)
{
	glm::quat rotation = glm::angleAxis(angle, axis);
	this->rotation *= rotation;
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
void Transform::setRotation(glm::quat newRotation)
{
	rotation = newRotation;
	updateTransformMatrix();
}
void Transform::setRotation(float angle, glm::vec3 axis)
{
	glm::quat newRotation = glm::angleAxis(angle, axis);
	this->rotation = newRotation;
	updateTransformMatrix();
}
void Transform::setScale(glm::vec3 newScale)
{
	scale = newScale;
	updateTransformMatrix();
}
void Transform::setScale(glm::mat4 newScale)
{
	//TODO//Find a way to implement multiplying by a scale matrix
	transformMatrix = newScale * transformMatrix;
}
glm::vec3 Transform::getPosition()
{
	return this->position;
}
glm::quat Transform::getRotation()
{
	return this->rotation;
}
glm::vec3 Transform::getScale()
{
	return this->scale;
}
glm::vec3 Transform::getForward()
{
	glm::vec4 forwardVector = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	forwardVector = rotationMatrix * forwardVector;
	return glm::vec3(forwardVector);
}
glm::vec3 Transform::getUp()
{
	glm::vec4 upVector = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	upVector = rotationMatrix * upVector;
	return glm::vec3(upVector);
}
glm::vec3 Transform::getRight()
{
	glm::vec4 rightVector = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	rightVector = rotationMatrix * rightVector;
	return glm::vec3(rightVector);
}

glm::mat4 Transform::getInverseMatrix() const
{
	return glm::inverse(transformMatrix);
}

glm::mat4 Transform::getInverseRotationMatrix() const
{
	return glm::inverse(rotationMatrix);
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
	rotationMatrix = glm::toMat4(rotation);
}

#pragma endregion