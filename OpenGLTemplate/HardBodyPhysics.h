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
	static void Collide(
		PhysicsComponent& a, 
		PhysicsComponent& b, 
		const glm::vec3& normalOfCollision, 
		const glm::vec3& positionOfCollision);
	static void Collide(
		PhysicsComponent& a, 
		PhysicsComponent& b, 
		Contact& contact);
	static float getMomentumMagnitude(
		float e, 
		const glm::vec3& combinedVelocities, 
		const glm::vec3& normal, 
		float inverseMassA, 
		float inverseMassB, 
		const glm::vec3& contactToA, 
		const glm::vec3& contactToB, 
		float InverseIOMA, 
		float InverseIOMB);

	static std::vector<PhysicsComponent*> PhysicsComponents;
};

