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
	const glm::vec3& getVelocity() const;
	glm::vec3& getVelocity();
	const glm::quat& getRotationalVelocity() const;
	glm::quat& getRotationalVelocity();

	//Setters
	void setVelocity(glm::vec3 newVelocity);
	void setRotationalVelocity(glm::quat newRotationalVelocity);

	//Methods
	void reset();
	void update(float dt);

	//MovementMethods?
	void addForce(
		const glm::vec3& force, 
		const glm::vec3& positionOfForce);
	void addImpulse(
		const glm::vec3& impulse, 
		const glm::vec3& positionOfImpulse);

	//I need this to resolve interpenetrations
	void move(const glm::vec3& offset);

private:
	glm::vec3 acceleration;
	glm::vec3 velocity;
	glm::quat rotationalAcceleration;
	glm::quat rotationalVelocity;
	static const float dampeningFactor;

};

