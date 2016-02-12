#pragma once
#include "OpenGL.h"
#include "Constants.h"
class Transform
{
public:

	//Constructor/Destructor
	Transform(glm::vec3 position = glm::vec3(0.0f), glm::quat rotation = glm::angleAxis(1.0f, glm::vec3(0.0f)), glm::vec3 scale = glm::vec3(1.0f));
	~Transform();

	//Methods
	void move(glm::vec3 displacement);
	void rotate(glm::quat rotation);
	void rotate(float angle, glm::vec3 axis);
	void changeScale(glm::vec3 scale);
	void setPosition(glm::vec3 newPosition);
	void setRotation(glm::quat newRotation);
	void setRotation(float angle, glm::vec3 axis);
	void setScale(glm::vec3 newScale);

	const glm::vec3& getPosition() const;
	glm::vec3& getPosition();
	const glm::quat& getRotation() const;
	glm::quat& getRotation();

	const glm::vec3& getScale() const;
	glm::vec3& getScale();

	glm::vec3 getForward() const;
	glm::vec3 getUp() const;
	glm::vec3 getRight() const;

	glm::mat4 getTransformMatrix() const;
	glm::mat3 getRotationMatrix() const;

	//I need this for something so I added it
	glm::mat4 getInverseMatrix() const;

private:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
};

