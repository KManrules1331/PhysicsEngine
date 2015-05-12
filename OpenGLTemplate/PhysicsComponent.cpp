#include "PhysicsComponent.h"

float PhysicsComponent::dampeningFactor = 0.8f;

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

glm::quat PhysicsComponent::getRotationalVelocity()
{
	return rotationalVelocity;
}

void PhysicsComponent::reset() {
	velocity = glm::vec3(0.0f);
	rotationalVelocity = glm::quat();
}

void PhysicsComponent::update(float dt) {
	//Update consistent variables
	velocity += acceleration * dt;
	rotationalVelocity *= glm::angleAxis(glm::angle(rotationalAcceleration) * dt, glm::axis(rotationalAcceleration));

	//Dampening
	float frameDamp = std::max(1.0f - ((1.0f - dampeningFactor) * dt), 0.0f);
	velocity *= frameDamp;
	rotationalVelocity = glm::angleAxis(glm::angle(rotationalVelocity) * frameDamp, glm::axis(rotationalVelocity));

	GOTransform.move(velocity * dt);
	GOTransform.rotate(glm::angleAxis(glm::angle(rotationalVelocity) * dt, glm::axis(rotationalVelocity)));

	//Reset frame variables
	acceleration = glm::vec3(0.0f);
	rotationalAcceleration = glm::quat();
}

void PhysicsComponent::addForce(glm::vec3 force, glm::vec3 positionOfForce)
{
	glm::vec3 radiusAP, axis;
	if (positionOfForce == GOTransform.getPosition())
	{
		radiusAP = force;
		axis = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		radiusAP = positionOfForce - GOTransform.getPosition();
		axis = glm::normalize(glm::cross(force, radiusAP));
		axis = glm::vec3(glm::inverse(GOTransform.rotationMatrix) * glm::vec4(axis, 1.0f));
	}
	if (glm::length(force) > 0)
	{
		acceleration += glm::normalize(radiusAP) * glm::dot(force, glm::normalize(radiusAP)) * inverseMass;
		rotationalAcceleration *= glm::angleAxis(glm::length(glm::cross(force, radiusAP)) * inverseMOI, axis);
	}
}

void PhysicsComponent::addImpulse(glm::vec3 impulse, glm::vec3 positionOfImpulse)
{
	glm::vec3 radiusAP, axis;
	if (positionOfImpulse == GOTransform.getPosition())
	{
		radiusAP = impulse;
		axis = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		radiusAP = positionOfImpulse - GOTransform.getPosition();
		axis = glm::normalize(glm::cross(impulse, radiusAP));
		if (glm::length(axis) > 0)
			axis = glm::vec3(glm::inverse(GOTransform.rotationMatrix) * glm::vec4(axis, 1.0f));
		else
			axis = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	if (glm::length(impulse) > 0)
	{
		//GOTransform.move(-velocity);
		//GOTransform.rotate(-rotationalVelocity);
		velocity += glm::normalize(radiusAP) * glm::dot(impulse, glm::normalize(radiusAP)) * inverseMass;
		rotationalVelocity *= glm::angleAxis(glm::length(glm::cross(impulse, radiusAP)) * inverseMOI, axis);
		//GOTransform.move(velocity);
		//GOTransform.rotate(rotationalVelocity);
	}
}

//Move the component by the offset vector
//Moves instantly with no change in velocity or acceleration
void PhysicsComponent::move(glm::vec3 offset)
{
	GOTransform.move(offset);
}