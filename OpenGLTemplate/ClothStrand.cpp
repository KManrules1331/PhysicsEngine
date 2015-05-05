#include "ClothStrand.h"


ClothStrand::ClothStrand(float restingLength) : GameObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f))
{
	this->restingLength = restingLength;
}


ClothStrand::~ClothStrand()
{
}

bool ClothStrand::pull(PhysicsComponent& anchorNode, PhysicsComponent& flexibleNode)
{
	//Find distance between nodes
	glm::vec3 diff = anchorNode.GOTransform.getPosition() - flexibleNode.GOTransform.getPosition();
	glm::vec3 movement;
	//If it needs to pull
	if (glm::length(diff) > restingLength * 1.01f)	//Constant is for error rounding
	{
		//Find where node should be
		movement = diff - (diff * (restingLength / glm::length(diff)));
		//Move flexible node
		flexibleNode.GOTransform.move(movement);

		//Inform caller node has moved
		return true;
	}
	//Inform caller node has not moved
	return false;
}

void ClothStrand::drawStrand(glm::vec3 positionA, glm::vec3 positionB)
{
	glm::vec3 diff = positionB = positionA;
	this->transform->setPosition((positionA + positionB) * 0.5f);
	this->transform->setScale(glm::vec3(0.01f, glm::length(diff), 0.01f));
	glm::vec3 axis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), positionB - positionA);
	if (glm::length(axis) <= 0)
		axis = glm::vec3(0.0f, 0.0f, 1.0f);
	else
		axis = glm::normalize(axis);
	this->transform->setRotation(glm::angle(glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(positionB - positionA)), axis);

	GameObject::draw();
}