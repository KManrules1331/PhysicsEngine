#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent(Transform& t, CollisionDetector& d, float mass, float MOI) : GOTransform{ t }, GOCollider{ d }
{
	if (mass == 0.0f) {
		inverseMass = 0.0f;
	}
	else {
		inverseMass = 1.0f / mass;
	}

	if (MOI == 0.0f) {
		inverseMOI = 0.0f;
	}
	else {
		inverseMOI = 1.0f / MOI;
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
	if (glm::length(force) > 0)
	{
		acceleration += glm::normalize(radiusAP) * glm::dot(force, glm::normalize(radiusAP)) * inverseMass;
		rotationalAcceleration += glm::cross(force, radiusAP) * inverseMOI;
	}
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
	if (glm::length(impulse) > 0)
	{
		GOTransform.move(-velocity);
		GOTransform.rotate(-rotationalVelocity);
		velocity += glm::normalize(radiusAP) * glm::dot(impulse, glm::normalize(radiusAP)) * inverseMass;
		rotationalVelocity += glm::cross(impulse, radiusAP) * inverseMOI;
		//GOTransform.move(velocity);
		//GOTransform.rotate(rotationalVelocity);
	}
}