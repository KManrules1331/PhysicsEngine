#pragma once
#include "OpenGL.h"
#include "Constants.h"
class Transform
{
public:
	//Attributes
	glm::mat4 transformMatrix;
	glm::mat4 rotationMatrix;

	//Constructor/Destructor
	Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
	~Transform();

	//Methods
	void move(glm::vec3 displacement);
	void rotate(glm::vec3 rotation);
	void changeScale(glm::vec3 scale);
	void setPosition(glm::vec3 newPosition);
	void setRotation(glm::vec3 newRotation);
	void setScale(glm::vec3 newScale);
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	glm::vec3 getForward();
	glm::vec3 getUp();
	glm::vec3 getRight();

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	//Methods
	void updateTransformMatrix();
	void updateRotationMatrix();
};

