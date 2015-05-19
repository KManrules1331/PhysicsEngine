#include "HardBodyPhysics.h"

std::vector<PhysicsComponent*> HardBodyPhysics::PhysicsComponents;

void HardBodyPhysics::Collide(PhysicsComponent* a, PhysicsComponent* b, glm::vec3 normalOfCollision, glm::vec3 positionOfCollision)
{
	glm::vec3 radiusAP = positionOfCollision - a->GOTransform.getPosition();
	glm::vec3 radiusBP = positionOfCollision - b->GOTransform.getPosition();
	glm::vec3 velocityA = a->getVelocity() + glm::cross(glm::axis(a->getRotationalVelocity()) * glm::angle(a->getRotationalVelocity()), radiusAP);
	glm::vec3 velocityB = b->getVelocity() + glm::cross(glm::axis(b->getRotationalVelocity()) * glm::angle(b->getRotationalVelocity()), radiusBP);
	float j = getMomentumMagnitude(1.0f, velocityA - velocityB, normalOfCollision, a->inverseMass, b->inverseMass, radiusAP, radiusBP, a->inverseMOI, b->inverseMOI);

	a->addImpulse(normalOfCollision * j, positionOfCollision);
	b->addImpulse(normalOfCollision * -j, positionOfCollision);
}
//SAT implementation of this method 
void HardBodyPhysics::Collide(PhysicsComponent* a, PhysicsComponent* b, Contact contact)
{
	//Move each object by half the collision depth
	//TODO//Modify to be proportional to their masses
	a->move(-contact.normal * (contact.depth / 2.0f));
	b->move(contact.normal * (contact.depth / 2.0f));

	//Get the impulse scalar
	glm::vec3 radiusAP = contact.position -a->GOTransform.getPosition();
	glm::vec3 radiusBP = contact.position - b->GOTransform.getPosition();
	glm::vec3 velocityA = a->getVelocity() + glm::cross(glm::axis(a->getRotationalVelocity()) * glm::angle(a->getRotationalVelocity()), radiusAP);
	glm::vec3 velocityB = b->getVelocity() + glm::cross(glm::axis(b->getRotationalVelocity()) * glm::angle(b->getRotationalVelocity()), radiusBP);
	float j = getMomentumMagnitude(1.0f, velocityA - velocityB, contact.normal, a->inverseMass, b->inverseMass, radiusAP, radiusBP, a->inverseMOI, b->inverseMOI);
	j = glm::abs(j);
	//Apply impulse
	//TODO//Determine if direction is correct
	a->addImpulse(-contact.normal * j, contact.position);
	b->addImpulse(contact.normal * j, contact.position);
}

float HardBodyPhysics::getMomentumMagnitude(float e, glm::vec3 combinedVelocities, glm::vec3 normal, float inverseMassA, float inverseMassB, glm::vec3 radiusOfA, glm::vec3 radiusOfB, float InverseIOMA, float InverseIOMB)
{
	float denom1 = glm::dot(normal, normal) * (inverseMassA + inverseMassB);
	float denom2 = pow(glm::length(radiusOfA) * glm::length(normal) * sin(glm::angle(glm::normalize(radiusOfA), glm::normalize(normal))), 2) * InverseIOMA;
	float denom3 = pow(glm::length(radiusOfB) * glm::length(normal) * sin(glm::angle(glm::normalize(radiusOfB), glm::normalize(normal))), 2) * InverseIOMB;

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
			//Get contact and resolve any collision
			Contact contact;
			bool collision;
			collision = PhysicsComponents[i]->GOCollider.getSATCollisionInfo(PhysicsComponents[j]->GOCollider, &contact);
			if (collision)
				Collide(PhysicsComponents[i], PhysicsComponents[j], contact);
		}
	}
}