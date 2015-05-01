#include "Spring.h"


Spring::Spring(PhysicsComponent& nodeA, PhysicsComponent& nodeB, glm::vec3 nodeAAnchorPoint, glm::vec3 nodeBAnchorPoint) : GameObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)), a{ nodeA }, b{ nodeB }
{
	//aLoc and bLoc are relative to the object's 0 transform.  To get world coordinates, multiply by node's transform matrices
	aLoc = glm::vec4(nodeAAnchorPoint, 1.0f);
	bLoc = glm::vec4(nodeBAnchorPoint, 1.0f);

	restingLength = glm::length((b.GOTransform.transformMatrix * bLoc) - (a.GOTransform.transformMatrix * aLoc));
	k = 2000.0f;
}


Spring::~Spring()
{
}

void Spring::update(float dt) {
	glm::vec3 aPosition = glm::vec3(a.GOTransform.transformMatrix * aLoc);
	glm::vec3 bPosition = glm::vec3(b.GOTransform.transformMatrix * bLoc);

	glm::vec3 diff = bPosition - aPosition;
	float displacement = glm::length(diff) - restingLength;
	if (!std::isfinite(displacement))
	{
		displacement = std::numeric_limits<float>::max();
		diff *= 1.0f / diff.x;
	}

	diff = glm::normalize(diff);
	diff *= k * displacement * 0.5f;
	a.addForce(diff, aPosition);
	diff *= -1;
	b.addForce(diff, bPosition);
}

void Spring::draw() {
	glm::vec3 aPosition = glm::vec3(a.GOTransform.transformMatrix * aLoc);
	glm::vec3 bPosition = glm::vec3(b.GOTransform.transformMatrix * bLoc);
	glm::vec3 diff = bPosition - aPosition;

	this->transform->setPosition((aPosition + bPosition) * 0.5f);
	this->transform->setScale(glm::vec3(0.01f, glm::length(diff), 0.01f));
	//If angle is 0 or PI or 2PI, produces error
	glm::vec3 axis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), bPosition - aPosition);
	if (glm::length(axis) <= 0)
		axis = glm::vec3(0.0f, 0.0f, 1.0f);
	else
		axis = glm::normalize(axis);
	this->transform->setRotation(glm::angle(glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(bPosition - aPosition)), axis);

	GameObject::draw();
}