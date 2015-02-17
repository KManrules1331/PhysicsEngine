#include "CubeCollider.h"


CubeCollider::CubeCollider(glm::vec3* center, float sideLength) : Collider(center, sideLength, sideLength, sideLength)
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