#include "PhysicsComponent.h"

float PhysicsComponent::dampeningFactor = 0.8f;

PhysicsComponent::PhysicsComponent(Transform& t, CollisionDetector& d, float mass, float MOI, bool softBody) 
	: GOTransform{ t }, GOCollider{ d }, softBody{ softBody }
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

	deformationMat = glm::mat4(1.0f);
	elasticity = 0.9f;
	deformAmount = 0;
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

glm::mat4 PhysicsComponent::getDeformMat() const{ return deformationMat; }

glm::vec3 PhysicsComponent::getScaleVector() const
{
	if (deformAmount > 0.01f)
		return scaleVector;
	return glm::vec3(1.0f);
}

bool PhysicsComponent::isSoftBody() const { return softBody; }

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

	//SOFT BODY STUFF
	if (softBody)
	{
		//Integration step for soft bodies
		deformVelocity -= elasticity * deformAmount * dt;		//Deformation force
		deformAmount += deformVelocity * dt;	
		deformVelocity *= 0.95f;								//Damping
		float scaleRatio = (1.0f + deformAmount) / 1.0f;		//1.0f literal is for hardcoded size

		//Create orthonormal directions
		glm::vec3 u;
		u.x = -deformDirection.y;
		u.y = deformDirection.x;
		u.z = deformDirection.z;

		u = glm::normalize(u);
		glm::vec3 v = glm::normalize(glm::cross(u, deformDirection));

		//Sum all scaled orthogonal vectors 
		scaleVector = glm::abs(deformDirection * (u + v + deformDirection));
		scaleVector = scaleVector + glm::vec3(1.0f);
	}
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
		//Add linear acceleration
		acceleration += glm::normalize(radiusAP) * glm::dot(force, glm::normalize(radiusAP)) * inverseMass;

		//Add rotational acceleration
		//I'm about 60% sure this will work
		float torqueScalar = glm::length(force) * glm::length(radiusAP);
		axis *= torqueScalar;
		axis = GOCollider.getAngularAcceleration(axis, inverseMass);
		rotationalAcceleration *= glm::angleAxis(glm::length(axis), glm::normalize(axis));
		
		//rotationalAcceleration *= glm::angleAxis(glm::length(glm::cross(force, radiusAP)) * inverseMOI, axis);
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
		//velocity += glm::normalize(radiusAP) * glm::dot(impulse, glm::normalize(radiusAP)) * inverseMass;
		//rotationalVelocity *= glm::angleAxis(glm::length(glm::cross(impulse, radiusAP)) * inverseMOI, axis);
		velocity += impulse * inverseMass;
		//GOTransform.move(velocity);
		//GOTransform.rotate(rotationalVelocity);

		float torqueScalar = glm::length(impulse) * glm::length(radiusAP);
		axis *= torqueScalar;
		axis = GOCollider.getAngularAcceleration(axis, inverseMass);
		rotationalVelocity *= glm::angleAxis(glm::length(axis), glm::normalize(axis));
	}

	//SOFT BODY PHYSICS STUFF
	if (softBody)
	{
		//Calculate deform amount
		float impulseMag = glm::length(impulse);
		deformAmount = 1.0f / (impulseMag + 1.0f) - 0.5f;		//Ojbect size is hardcoded for now

		std::cout << deformAmount << std::endl;
		deformDirection = impulse / impulseMag;		//This is now normalized
		
		//Convert deform direction into object coordinates
		deformDirection = transformVec3(deformDirection, GOTransform.getInverseMatrix());

		//deformAmount is a distance. Need to convert it to a ratio first
		float scaleRatio = (1.0f + deformAmount) / 1.0f;			

		//TODO//Initialize this properly
		deformVelocity = 0;
	}
}

//Move the component by the offset vector
//Moves instantly with no change in velocity or acceleration
void PhysicsComponent::move(glm::vec3 offset)
{
	GOTransform.move(offset);
}