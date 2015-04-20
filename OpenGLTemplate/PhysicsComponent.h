#pragma once
#include "Transform.h"
#include "CollisionDetector.h"

class PhysicsComponent
{
public:
	float inverseMass;
	float inverseMOI;
	Transform& GOTransform;
	CollisionDetector* GOCollider;	//DOES NOT OWN, BUT USES POINTER TO

	//Constructor
	PhysicsComponent(Transform& t, CollisionDetector* d, float mass, float MOI);

	//Destructor
	~PhysicsComponent();

	//Getters
	glm::vec3 getVelocity();
	glm::quat getRotationalVelocity();

	//Methods
	void reset();
	void update(float dt);

	//MovementMethods?
	void addForce(glm::vec3 force, glm::vec3 positionOfForce);
	void addImpulse(glm::vec3 impulse, glm::vec3 positionOfImpulse);

private:
	//Attributes
	glm::vec3 acceleration;
	glm::quat rotationalAcceleration;
	glm::vec3 velocity;
	glm::quat rotationalVelocity;
};

