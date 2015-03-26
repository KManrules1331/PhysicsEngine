#include "HardBodyPhysics.h"

void HardBodyPhysics::Collide(PhysicsComponent& a, PhysicsComponent& b, glm::vec3 normalOfCollision, glm::vec3 positionOfCollision)
{
	glm::vec3 radiusAP = positionOfCollision - a.GOTransform.getPosition();
	glm::vec3 radiusBP = positionOfCollision = b.GOTransform.getPosition();
	float j = getMomentumMagnitude(1.0f, a.getVelocity() - b.getVelocity(), normalOfCollision, a.inverseMass, b.inverseMass, radiusAP, radiusBP, a.inverseMOI, b.inverseMOI);

	//Apply
	a.addImpulse(normalOfCollision * j, positionOfCollision);
	b.addImpulse(normalOfCollision * -j, positionOfCollision);
}

float HardBodyPhysics::getMomentumMagnitude(float e, glm::vec3 combinedVelocities, glm::vec3 normal, float inverseMassA, float inverseMassB, glm::vec3 radiusOfA, glm::vec3 radiusOfB, float InverseIOMA, float InverseIOMB)
{
	float denom1 = glm::dot(normal, normal) * (inverseMassA + inverseMassB);
	float denom2 = pow(glm::dot(radiusOfA, normal), 2) * InverseIOMA;
	float denom3 = pow(glm::dot(radiusOfB, normal), 2) * InverseIOMB;

	float denominator = denom1 + denom2 + denom3;

	float numerator = -(1 + e) * glm::dot(combinedVelocities, normal);

	return numerator / denominator;
}