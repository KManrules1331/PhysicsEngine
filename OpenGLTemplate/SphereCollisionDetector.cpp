#include "SphereCollisionDetector.h"


SphereCollisionDetector::SphereCollisionDetector(Transform& GOTransform, float radius) : CollisionDetector(GOTransform)
{
	this->radius = radius;
}


SphereCollisionDetector::~SphereCollisionDetector(void)
{
}

bool SphereCollisionDetector::detectCollision(CollisionDetector& c)
{
	if (AABBCollision(c))
	{
		return c.detectSphereCollision(*this);
	}
	return false;
}

bool SphereCollisionDetector::detectSphereCollision(SphereCollisionDetector& c)
{
	return getDistance(c.GOTransform.getPosition()) <= c.radius + this->radius;
}

bool SphereCollisionDetector::detectCubeCollision(CubeCollisionDetector& c)
{
	return false;
}

float SphereCollisionDetector::getDistance(glm::vec3 point)
{
	glm::vec3 thisCenter = GOTransform.getPosition();
	glm::vec3 displacement = point - thisCenter;
	float answer = glm::abs(displacement.x * displacement.x);
	answer += glm::abs(displacement.y * displacement.y);
	answer += glm::abs(displacement.z * displacement.z);
	return glm::sqrt(answer);
}

CollisionDetector::ContainingBox SphereCollisionDetector::getAABB()
{
	glm::vec3 center = GOTransform.getPosition();
	glm::vec3 scale = GOTransform.getScale();
	ContainingBox returnBox;
	returnBox.left = center.x - scale.x / 2;
	returnBox.right = center.x + scale.x / 2;
	returnBox.down = center.y - scale.y / 2;
	returnBox.up = center.y + scale.y / 2;
	returnBox.front = center.z - scale.z / 2;
	returnBox.back = center.z + scale.z / 2;

	return returnBox;
}