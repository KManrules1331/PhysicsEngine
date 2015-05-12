#pragma once
#include "OpenGL.h"
#include "PhysicsComponent.h"

class HardBodyPhysics
{
public:
	static void addPhysicsComponent(PhysicsComponent* a);
	static void removePhysicsComponent(PhysicsComponent* a);

	static void update();
private:
	static void Collide(PhysicsComponent* a, PhysicsComponent* b, glm::vec3 normalOfCollision, glm::vec3 positionOfCollision);
	static void Collide(PhysicsComponent* a, PhysicsComponent* b, Contact contact);
	static float getMomentumMagnitude(float e, glm::vec3 combinedVelocities, glm::vec3 normal, float inverseMassA, float inverseMassB, glm::vec3 radiusOfA, glm::vec3 radiusOfB, float InverseIOMA, float InverseIOMB);
	static std::vector<PhysicsComponent*> PhysicsComponents;
};

