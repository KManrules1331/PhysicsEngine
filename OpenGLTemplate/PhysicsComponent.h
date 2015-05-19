#pragma once
#include "Transform.h"
#include "CollisionDetector.h"

#include <iostream>

class PhysicsComponent
{
public:
	float inverseMass;
	float inverseMOI;
	Transform& GOTransform;
	CollisionDetector& GOCollider;

	//Constructor
	PhysicsComponent(Transform& t, CollisionDetector& d, float mass, float MOI, bool softBody = false);

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

	//I need this to resolve interpenetrations
	void move(glm::vec3 offset);

	//For soft body physics
	glm::mat4 getDeformMat() const;
	glm::vec3 getScaleVector() const;
	bool isSoftBody() const;

private:
	//Attributes
	glm::vec3 acceleration;
	glm::quat rotationalAcceleration;
	glm::vec3 velocity;
	glm::quat rotationalVelocity;
	static float dampeningFactor;

	//Attributes for soft body physics
	//This will only work for a single deformation at a time
	bool softBody;
	float elasticity = 0.9f;				//Values 0 - 1
	glm::mat4 deformationMat;		//Just a scale matrix
	glm::vec3 deformDirection;
	glm::vec3 scaleVector;
	float deformAmount;				//Analogous to displacement
	float deformVelocity;			//Analogous to velocity
};

