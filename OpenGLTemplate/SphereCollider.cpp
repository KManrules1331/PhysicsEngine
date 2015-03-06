#include "SphereCollider.h"


SphereCollider::SphereCollider(Transform* GOTransform, float radius) : Collider(GOTransform)
{
	this->radius = radius;
}


SphereCollider::~SphereCollider(void)
{
}

bool SphereCollider::detectCollision(Collider* c)
{
	if (AABBCollision(c))
	{
		return c->detectSphereCollision(this);
	}
	return false;
}

bool SphereCollider::detectSphereCollision(SphereCollider* c)
{
	return getDistance(c) <= c->radius + this->radius;
}

bool SphereCollider::detectCubeCollision(CubeCollider* c)
{
	return false;
}

float SphereCollider::getDistance(Collider* c)
{
	glm::vec3 cCenter = c->GOTransform->getPosition();
	glm::vec3 thisCenter = GOTransform->getPosition();
	glm::vec3 displacement = cCenter - thisCenter;
	float answer = glm::abs(displacement.x * displacement.x);
	answer += glm::abs(displacement.y * displacement.y);
	answer += glm::abs(displacement.z * displacement.z);
	return glm::sqrt(answer);
}

Collider::ContainingBox SphereCollider::getAABB()
{
	glm::vec3 center = GOTransform->getPosition();
	glm::vec3 scale = GOTransform->getScale();
	ContainingBox returnBox;
	returnBox.left = center.x - scale.x / 2;
	returnBox.right = center.x + scale.x / 2;
	returnBox.down = center.y - scale.y / 2;
	returnBox.up = center.y + scale.y / 2;
	returnBox.front = center.z - scale.z / 2;
	returnBox.back = center.z + scale.z / 2;

	return returnBox;
}