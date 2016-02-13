#include "PhysicsComponent.h"

const float PhysicsComponent::dampeningFactor = 0.9f;

PhysicsComponent::PhysicsComponent(
	Transform& t, 
	CollisionDetector& d, 
	float mass, 
	float MOI) 
	: GOTransform{ t }, 
	GOCollider{ d }
{
	inverseMass = mass == 0.0f ? 0.0f : 1.0f / mass;
	inverseMOI = MOI == 0.0f ? 0.0f : 1.0f / mass;
}


PhysicsComponent::~PhysicsComponent()
{
}

const glm::vec3& PhysicsComponent::getVelocity() const
{
	return velocity;
}
glm::vec3& PhysicsComponent::getVelocity()
{
	return velocity;
}

void PhysicsComponent::setVelocity(glm::vec3 newVelocity)
{
	velocity = std::move(newVelocity);
}

const glm::quat& PhysicsComponent::getRotationalVelocity() const
{
	return rotationalVelocity;
}
glm::quat& PhysicsComponent::getRotationalVelocity()
{
	return rotationalVelocity;
}

void PhysicsComponent::setRotationalVelocity(glm::quat newRotationalVelocity)
{
	rotationalVelocity = std::move(newRotationalVelocity);
}

void PhysicsComponent::reset() {
	velocity = glm::vec3(0.0f);
	rotationalVelocity = glm::quat();
}

void PhysicsComponent::update(float dt) 
{
	//Update consistent variables
	velocity += acceleration * dt;
	glm::quat frameRotationalAcceleration = glm::normalize(glm::angleAxis(glm::angle(rotationalAcceleration) * dt, glm::axis(rotationalAcceleration)));
	glm::quat newRotationalVelocity = rotationalVelocity * frameRotationalAcceleration;
	rotationalVelocity = newRotationalVelocity;
	glm::quat frameRotationalVelocity = glm::normalize(glm::angleAxis(glm::angle(newRotationalVelocity) * dt, glm::axis(newRotationalVelocity)));
	GOTransform.rotate(frameRotationalVelocity);

	glm::clamp(velocity, glm::vec3(1000.0f), glm::vec3(-1000.0f));
	assert(!glm::any(glm::isnan(velocity)));
	assert(!glm::any(glm::isinf(velocity)));

	GOTransform.move(velocity * dt);

	//Reset frame variables
	acceleration = glm::vec3(0.0f);
	rotationalAcceleration = glm::quat();
}

void PhysicsComponent::addForce(
	const glm::vec3& force, 
	const glm::vec3& positionOfForce)
{
	if (positionOfForce == GOTransform.getPosition())
	{
		glm::vec3 radiusAPDirection = glm::normalize(force);
		acceleration += radiusAPDirection * glm::dot(force, radiusAPDirection) * inverseMass;
		return;
	}

	glm::vec3 radiusAP = positionOfForce - GOTransform.getPosition();
	glm::vec3 axisCandidate = glm::cross(force, radiusAP);
	if (glm::length(axisCandidate) == 0.0f)
	{
		axisCandidate = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	glm::vec3 axis = glm::normalize(glm::rotate(GOTransform.getRotation(), glm::normalize(axisCandidate)));
	assert(glm::length(axis) > 0.0f);
	glm::vec3 radiusAPDirection = glm::normalize(radiusAP);
	acceleration += radiusAPDirection * glm::dot(force, radiusAPDirection) * inverseMass;
	rotationalAcceleration *= glm::angleAxis(glm::length(glm::cross(force, radiusAP)) * inverseMOI, axis);
}

void PhysicsComponent::addImpulse(
	const glm::vec3& impulse, 
	const glm::vec3& positionOfImpulse)
{
	if (positionOfImpulse == GOTransform.getPosition())
	{
		glm::vec3 radiusAPDirection = glm::normalize(impulse);
		velocity += radiusAPDirection * glm::dot(impulse, radiusAPDirection) * inverseMass;
		return;
	}

	glm::vec3 radiusAP = positionOfImpulse - GOTransform.getPosition();
	glm::vec3 axisCandidate = glm::cross(impulse, radiusAP);
	if (glm::length(axisCandidate) == 0.0f)
	{
		axisCandidate = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	glm::vec3 axis = glm::normalize(glm::rotate(GOTransform.getRotation(), glm::normalize(axisCandidate)));
	assert(glm::length(axis) > 0.0f);
	glm::vec3 radiusAPDirection = glm::normalize(radiusAP);
	velocity += radiusAPDirection * glm::dot(impulse, radiusAPDirection) * inverseMass;
	rotationalVelocity *= glm::angleAxis(glm::length(glm::cross(impulse, radiusAP)) * inverseMOI, axis);
}

//Move the component by the offset vector
//Moves instantly with no change in velocity or acceleration
void PhysicsComponent::move(const glm::vec3& offset)
{
	GOTransform.move(offset);
}