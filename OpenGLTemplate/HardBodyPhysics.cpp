#include "HardBodyPhysics.h"

std::vector<PhysicsComponent*> HardBodyPhysics::PhysicsComponents;

void HardBodyPhysics::Collide(PhysicsComponent* a, PhysicsComponent* b, glm::vec3 normalOfCollision, glm::vec3 positionOfCollision)
{
	glm::vec3 radiusAP = positionOfCollision - a->GOTransform.getPosition();
	glm::vec3 radiusBP = positionOfCollision - b->GOTransform.getPosition();
	glm::vec3 velocityA = a->getVelocity() + glm::cross(a->getRotationalVelocity(), radiusAP);
	glm::vec3 velocityB = b->getVelocity() + glm::cross(b->getRotationalVelocity(), radiusBP);
	float j = getMomentumMagnitude(1.0f, velocityA - velocityB, normalOfCollision, a->inverseMass, b->inverseMass, radiusAP, radiusBP, a->inverseMOI, b->inverseMOI);

	a->addImpulse(normalOfCollision * j, positionOfCollision);
	b->addImpulse(normalOfCollision * -j, positionOfCollision);
}

float HardBodyPhysics::getMomentumMagnitude(float e, glm::vec3 combinedVelocities, glm::vec3 normal, float inverseMassA, float inverseMassB, glm::vec3 radiusOfA, glm::vec3 radiusOfB, float InverseIOMA, float InverseIOMB)
{
	float denom1 = glm::dot(normal, normal) * (inverseMassA + inverseMassB);
	float denom2 = pow(glm::length(glm::cross(radiusOfA, normal)), 2) * InverseIOMA;
	float denom3 = pow(glm::length(glm::cross(radiusOfB, normal)), 2) * InverseIOMB;

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
	for (int i = 0; i < PhysicsComponents.size(); i++)
	{
		if (PhysicsComponents[i] == b)
		{
			PhysicsComponents.erase(PhysicsComponents.begin() + i);
			break;
		}
	}
}

void HardBodyPhysics::update()
{
	for (int i = 0; i < PhysicsComponents.size(); i++)
	{
		for (int j = i + 1; j < PhysicsComponents.size(); j++)
		{
			if (PhysicsComponents[i]->GOCollider.detectCollision(PhysicsComponents[j]->GOCollider))
			{
				glm::vec3 position, normal;
				if(PhysicsComponents[i]->GOCollider.getCollisionInfo(PhysicsComponents[j]->GOCollider, &position, &normal))
					Collide(PhysicsComponents[i], PhysicsComponents[j], normal, position);
			}
		}
	}
}