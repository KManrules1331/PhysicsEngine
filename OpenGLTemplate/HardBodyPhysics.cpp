#include "HardBodyPhysics.h"

std::vector<PhysicsComponent*> HardBodyPhysics::PhysicsComponents;

void HardBodyPhysics::Collide(PhysicsComponent* a, PhysicsComponent* b, glm::vec3 normalOfCollision, glm::vec3 positionOfCollision)
{
	//All code and private functions mentioned in this section
	//Derive from this article: http://chrishecker.com/images/e/e7/Gdmphys3.pdf

	glm::vec3 radiusAP = positionOfCollision - a->GOTransform.getPosition();
	glm::vec3 radiusBP = positionOfCollision - b->GOTransform.getPosition();
	glm::vec3 velocityA = a->getVelocity() + glm::cross(glm::axis(a->getRotationalVelocity()) * glm::angle(a->getRotationalVelocity()), radiusAP);
	glm::vec3 velocityB = b->getVelocity() + glm::cross(glm::axis(b->getRotationalVelocity()) * glm::angle(b->getRotationalVelocity()), radiusBP);
	float j = getMomentumMagnitude(1.0f, velocityA - velocityB, normalOfCollision, a->inverseMass, b->inverseMass, radiusAP, radiusBP, a->inverseMOI, b->inverseMOI);

	a->addImpulse(normalOfCollision * j, positionOfCollision);
	b->addImpulse(normalOfCollision * -j, positionOfCollision);
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
				glm::vec3 position1, normal1;
				glm::vec3 position2, normal2;
				bool collision1, collision2;
				collision1 = PhysicsComponents[i]->GOCollider.getCollisionInfo(PhysicsComponents[j]->GOCollider, &position1, &normal1);
				//collision2 = PhysicsComponents[j]->GOCollider.getCollisionInfo(PhysicsComponents[i]->GOCollider, &position2, &normal2);
				if (collision1)
					Collide(PhysicsComponents[i], PhysicsComponents[j], normal1, position1);
				//if (collision2)
					//Collide(PhysicsComponents[j], PhysicsComponents[i], normal2, position2);
			}
		}
	}
}