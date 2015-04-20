#pragma once
#include "GameObject.h"
#include "PhysicsComponent.h"

class Spring : public GameObject
{
public:
	Spring(PhysicsComponent& nodeA, PhysicsComponent& nodeB, glm::vec3 nodeAAnchorPoint, glm::vec3 nodeBAnchorPoint);
	~Spring();

	void update(float dt) override;
	void draw() override;

private:
	PhysicsComponent& a;
	PhysicsComponent& b;
	glm::vec4 aLoc;
	glm::vec4 bLoc;

	float restingLength;
	float k;
};

