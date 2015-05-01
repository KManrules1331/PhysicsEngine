#pragma once
#include "GameObject.h"
#include "PhysicsComponent.h"

class ClothStrand : public GameObject
{
public:
	ClothStrand(float restingLength);
	~ClothStrand();

	bool pull(PhysicsComponent& anchorNode, PhysicsComponent& flexibleNode);

	void drawStrand(glm::vec3 positionA, glm::vec3 positionB);
private:
	float restingLength;
};

