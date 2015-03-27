#pragma once
#include "Transform.h"
#include "CollisionDetector.h"

class PhysicsComponent
{
public:
	float inverseMass;
	float inverseMOI;
	Transform& GOTransform;
	CollisionDetector& GOCollider;

	//Constructor
	PhysicsComponent(Transform& t, CollisionDetector& d, float mass, float MOI);

	//Destructor
	~PhysicsComponent();

	//Getters
	glm::vec3 getVelocity();
	glm::vec3 getRotationalVelocity();

	//Methods
	void reset();
	void update();

	//MovementMethods?
	void addForce(glm::vec3 force, glm::vec3 positionOfForce);
	void addImpulse(glm::vec3 impulse, glm::vec3 positionOfImpulse);

private:
	//Attributes
	glm::vec3 acceleration;
	glm::vec3 rotationalAcceleration;
	glm::vec3 velocity;
	glm::vec3 rotationalVelocity;
};

