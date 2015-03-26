#pragma once

#include "PhysicsComponent.h"

class HardBodyPhysics
{
public:
	static void Collide(PhysicsComponent& a, PhysicsComponent& b, glm::vec3 normalOfCollision, glm::vec3 positionOfCollision);
private:
	static float getMomentumMagnitude(float e, glm::vec3 combinedVelocities, glm::vec3 normal, float inverseMassA, float inverseMassB, glm::vec3 radiusOfA, glm::vec3 radiusOfB, float InverseIOMA, float InverseIOMB);

};

