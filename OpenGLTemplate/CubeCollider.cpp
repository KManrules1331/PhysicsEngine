#include "CubeCollider.h"


CubeCollider::CubeCollider(Transform* GOTransform, float sideLength) : Collider(GOTransform)
{
	this->sideLength = sideLength;
}


CubeCollider::~CubeCollider()
{
}

void CubeCollider::HandleCollision(Collider* c)
{
	c->handleCubeCollision(this);
}

void CubeCollider::handleSphereCollision(SphereCollider* c)
{
	if (detectSphereCollision(c))
	{
		//Collision handler
	}
}

void CubeCollider::handleCubeCollision(CubeCollider* c)
{
	if (detectCubeCollision(c))
	{
		//Collision handler
	}
}

bool CubeCollider::detectCollision(Collider* c)
{
	if (AABBCollision(c))
	{
		return c->detectCubeCollision(this);
	}
	return false;
}

bool CubeCollider::detectSphereCollision(SphereCollider* c)
{
	return false;
}

bool CubeCollider::detectCubeCollision(CubeCollider* c)
{
	return false;
}

Collider::ContainingBox CubeCollider::getAABB()
{
	ContainingBox returnBox;
	float factor = sqrt(3) / 2;
	glm::vec3 center = GOTransform->getPosition();
	glm::vec3 scale = GOTransform->getScale();
	returnBox.right = center.x + scale.x * factor;
	returnBox.left = center.x - scale.x * factor;
	returnBox.up = center.y + scale.y * factor;
	returnBox.down = center.y - scale.y * factor;
	returnBox.front = center.z - scale.z * factor;
	returnBox.back = center.z + scale.z * factor;

	return returnBox;
}