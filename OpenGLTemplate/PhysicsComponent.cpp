#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent(Transform& t, CollisionDetector& d, float mass, float MOI) : GOTransform{ t }, GOCollider{ d }
{
	if (mass == 0.0f) {
		inverseMass = 0.0f;
	}
	else {
		inverseMass = 1 / mass;
	}

	if (MOI == 0.0f) {
		inverseMOI = 0.0f;
	}
	else {
		inverseMOI = 1 / MOI;
	}
}


PhysicsComponent::~PhysicsComponent()
{
}

glm::vec3 PhysicsComponent::getVelocity()
{
	return velocity;
}

glm::vec3 PhysicsComponent::getRotationalVelocity()
{
	return rotationalVelocity;
}

void PhysicsComponent::reset() {
	velocity = glm::vec3(0.0f);
	rotationalVelocity = glm::vec3(0.0f);
}

void PhysicsComponent::update() {
	//Update consistent variables
	velocity += acceleration;
	rotationalVelocity += rotationalAcceleration;

	GOTransform.move(velocity);
	GOTransform.rotate(rotationalVelocity);

	//Reset frame variables
	acceleration = glm::vec3(0.0f);
	rotationalAcceleration = glm::vec3(0.0f);
}

void PhysicsComponent::addForce(glm::vec3 force, glm::vec3 positionOfForce)
{
	glm::vec3 radiusAP;
	if (positionOfForce == GOTransform.getPosition())
	{
		radiusAP = force;
	}
	else
	{
		radiusAP = positionOfForce - GOTransform.getPosition();
	}
	acceleration += glm::dot(force, glm::normalize(positionOfForce)) * inverseMass;
	rotationalAcceleration += glm::cross(force, radiusAP) * inverseMOI;
}

void PhysicsComponent::addImpulse(glm::vec3 impulse, glm::vec3 positionOfImpulse)
{
	glm::vec3 radiusAP;
	if (positionOfImpulse == GOTransform.getPosition())
	{
		radiusAP = impulse;
	}
	else
	{
		radiusAP = positionOfImpulse - GOTransform.getPosition();
	}
	velocity += glm::dot(impulse, glm::normalize(positionOfImpulse)) * inverseMass;
	rotationalVelocity += glm::cross(impulse, radiusAP) * inverseMOI;
}