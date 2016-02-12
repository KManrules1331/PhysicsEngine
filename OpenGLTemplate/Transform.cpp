#include "Transform.h"


Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}


Transform::~Transform()
{
}

void Transform::move(glm::vec3 displacement)
{
	position += displacement;
}
void Transform::rotate(glm::quat rotation)
{
	this->rotation *= rotation;
}
void Transform::rotate(float angle, glm::vec3 axis)
{
	glm::quat rotation = glm::angleAxis(angle, axis);
	this->rotation *= rotation;
}
void Transform::changeScale(glm::vec3 scale)
{
	this->scale += scale;
}
void Transform::setPosition(glm::vec3 newPosition)
{
	position = newPosition;
}
void Transform::setRotation(glm::quat newRotation)
{
	rotation = newRotation;
}
void Transform::setRotation(float angle, glm::vec3 axis)
{
	glm::quat newRotation = glm::angleAxis(angle, axis);
	this->rotation = newRotation;
}
void Transform::setScale(glm::vec3 newScale)
{
	scale = newScale;
}
const glm::vec3& Transform::getPosition() const
{
	return this->position;
}
glm::vec3& Transform::getPosition()
{
	return this->position;
}
const glm::quat& Transform::getRotation() const
{
	return this->rotation;
}
glm::quat& Transform::getRotation()
{
	return this->rotation;
}
const glm::vec3& Transform::getScale() const
{
	return this->scale;
}
glm::vec3& Transform::getScale()
{
	return this->scale;
}
glm::vec3 Transform::getForward() const
{
	return getRotationMatrix()[2];
}
glm::vec3 Transform::getUp() const
{
	return getRotationMatrix()[1];
}
glm::vec3 Transform::getRight() const
{
	return getRotationMatrix()[0];
}

glm::mat4 Transform::getInverseMatrix() const
{
	return glm::inverse(getTransformMatrix());
}

#pragma region MatrixUpdaters

glm::mat3 Transform::getRotationMatrix() const
{
	return glm::toMat3(rotation);
}

glm::mat4 Transform::getTransformMatrix() const
{
	glm::mat4 scaleMatrix(glm::scale(scale));
	glm::mat4 rotationMatrix(glm::toMat4(rotation));
	glm::mat4 translationMatrix(glm::translate(position));

	return translationMatrix * rotationMatrix * scaleMatrix;
}

#pragma endregion