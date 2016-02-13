#include "HardBodyPhysics.h"

std::vector<PhysicsComponent*> HardBodyPhysics::PhysicsComponents;

void HardBodyPhysics::Collide(
	PhysicsComponent& a, 
	PhysicsComponent& b, 
	const glm::vec3& normalOfCollision, 
	const glm::vec3& positionOfCollision)
{
	//Get distance between contacts and center of masses
	glm::vec3 contactToA = positionOfCollision - a.GOTransform.getPosition();
	glm::vec3 contactToB = positionOfCollision - b.GOTransform.getPosition();

	//Get the velocity of the points of contact at time of contact
	const glm::quat& aRotVel = a.getRotationalVelocity();
	const glm::quat& bRotVel = b.getRotationalVelocity();
	glm::vec3 velocityA = a.getVelocity() + glm::cross(glm::axis(aRotVel) * glm::angle(aRotVel), contactToA);
	glm::vec3 velocityB = b.getVelocity() + glm::cross(glm::axis(bRotVel) * glm::angle(bRotVel), contactToB);

	//Plug into equation
	float j = getMomentumMagnitude(
		1.0f, 
		velocityA - velocityB, 
		normalOfCollision, 
		a.inverseMass, 
		b.inverseMass, 
		contactToA, 
		contactToB, 
		a.inverseMOI, 
		b.inverseMOI);

	//Apply calculated force back into object's movement.
	a.addImpulse(normalOfCollision * j, positionOfCollision);
	b.addImpulse(normalOfCollision * -j, positionOfCollision);
}
//SAT implementation of this method 
void HardBodyPhysics::Collide(
	PhysicsComponent& a, 
	PhysicsComponent& b, 
	Contact& contact)
{
	//Move objects to eliminate intersections
	glm::vec3& aVelocity = a.getVelocity();	
	glm::vec3& bVelocity = b.getVelocity();
	glm::vec3 relVelocity = aVelocity - bVelocity;
	float testNumber = glm::dot(relVelocity, contact.normal);
	float dt = contact.depth / testNumber * 0.5f;
	a.move(-aVelocity * dt);	b.move(-bVelocity * dt);
	a.GOCollider.getSATCollisionInfo(b.GOCollider, &contact);


	/*float aVelocity = glm::dot(a->getVelocity(), contact.normal);
	float bVelocity = glm::dot(b->getVelocity(), contact.normal);
	float total = aVelocity - bVelocity;*/



	/*a->move(-contact.normal * contact.depth * (aVelocity / total));
	b->move(contact.normal * contact.depth * (bVelocity / total));*/

	//Get the impulse scalar
	glm::vec3 radiusAP = contact.position - a.GOTransform.getPosition();
	glm::vec3 radiusBP = contact.position - b.GOTransform.getPosition();
	glm::vec3 velocityA = a.getVelocity() + glm::cross(glm::axis(a.getRotationalVelocity()) * glm::angle(a.getRotationalVelocity()), radiusAP);
	glm::vec3 velocityB = b.getVelocity() + glm::cross(glm::axis(b.getRotationalVelocity()) * glm::angle(b.getRotationalVelocity()), radiusBP);
	float j = getMomentumMagnitude(
		1.0f, 
		velocityA - velocityB, 
		contact.normal, 
		a.inverseMass, 
		b.inverseMass, 
		radiusAP, 
		radiusBP, 
		a.inverseMOI, 
		b.inverseMOI);
	j = glm::abs(j);

	//Apply impulse
	//TODO//Determine if direction is correct
	a.addImpulse(contact.normal * -j, contact.position);
	b.addImpulse(contact.normal * j, contact.position);

	a.move(a.getVelocity() * dt);
	b.move(b.getVelocity() * dt);
}

float HardBodyPhysics::getMomentumMagnitude(
	float e, 
	const glm::vec3& combinedVelocities, 
	const glm::vec3& normal, 
	float inverseMassA, 
	float inverseMassB, 
	const glm::vec3& contactToA, 
	const glm::vec3& contactToB, 
	float InverseIOMA, 
	float InverseIOMB)
{
	float denom1 = inverseMassA + inverseMassB;
	float contactToALength = glm::length(contactToA);
	float contactToBLength = glm::length(contactToB);
	float denom2 = pow(contactToALength * sin(glm::angle(contactToA / contactToALength, normal)), 2) * InverseIOMA;
	float denom3 = pow(contactToBLength * sin(glm::angle(contactToB / contactToBLength, normal)), 2) * InverseIOMB;

	float denominator = denom1 + denom2 + denom3;

	float numerator = -(1 + e) * glm::dot(combinedVelocities, normal);

	return numerator / denominator;
}

void HardBodyPhysics::addPhysicsComponent(PhysicsComponent* a)
{
	PhysicsComponents.push_back(a);
}

void HardBodyPhysics::removePhysicsComponent(PhysicsComponent* b)
{
	for (unsigned int i = 0; i < PhysicsComponents.size(); i++)
	{
		if (PhysicsComponents[i] == b)
		{
			PhysicsComponents.erase(PhysicsComponents.begin() + i);
			break;
		}
	}
}

//This has been changed to use the SAT dispatch track
void HardBodyPhysics::update()
{
	for (unsigned int i = 0; i < PhysicsComponents.size(); i++)
	{
		for (unsigned int j = i + 1; j < PhysicsComponents.size(); j++)
		{
			//TODO//Add code to resolve collision//
			//TODO//Figure out if the contact is pointing in the wrong direction
			//I might be able to change this to use a single contact and one call to getSATCollisionInfo
			Contact contact;
			bool collision;
			collision = PhysicsComponents[i]->GOCollider.getSATCollisionInfo(PhysicsComponents[j]->GOCollider, &contact);
			if (collision)
				Collide(*PhysicsComponents[i], *PhysicsComponents[j], contact);
		}
	}
}