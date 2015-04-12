#include "Spring.h"


Spring::Spring(PhysicsComponent& nodeA, PhysicsComponent& nodeB, glm::vec3 nodeAAnchorPoint, glm::vec3 nodeBAnchorPoint) : GameObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)), a{ nodeA }, b{ nodeB }
{
	//aLoc and bLoc are relative to the object's 0 transform.  To get world coordinates, multiply by node's transform matrices
	aLoc = glm::vec4(nodeAAnchorPoint, 1.0f);
	bLoc = glm::vec4(nodeBAnchorPoint, 1.0f);

	restingLength = glm::length((b.GOTransform.transformMatrix * bLoc) - (a.GOTransform.transformMatrix * aLoc));
}


Spring::~Spring()
{
}

void Spring::update() {
	glm::vec3 aPosition = glm::vec3(a.GOTransform.transformMatrix * aLoc);
	glm::vec3 bPosition = glm::vec3(b.GOTransform.transformMatrix * bLoc);

	glm::vec3 diff = bPosition - aPosition;
	float displacement = glm::length(diff) - restingLength;

	this->transform->setPosition((aPosition + bPosition) * 0.5f);
	this->transform->setScale(glm::vec3(0.1f, glm::length(diff), 0.1f));
	this->transform->setRotation(glm::angleAxis(glm::angle(glm::vec3(0.0f, 1.0f, 0.0f), bPosition - transform->getPosition()), glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), bPosition - transform->getPosition()))));

	diff = glm::normalize(diff);
	diff *= k * displacement;
	a.addForce(diff, aPosition);
	diff *= -1;
	b.addForce(diff, bPosition);
}