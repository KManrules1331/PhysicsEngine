#include "SphereCollider.h"


SphereCollider::SphereCollider(glm::vec3* center, float radius) : Collider(center, radius * 2, radius * 2, radius * 2)
{
	this->radius = radius;
}


SphereCollider::~SphereCollider(void)
{
}

void SphereCollider::HandleCollision(Collider* c)
{
	c->handleSphereCollision(this);
}

void SphereCollider::handleSphereCollision(SphereCollider* c)
{
	if(detectSphereCollision(c))
	{
		glm::vec3 displacement = *(c->center) - *(this->center);
		displacement *= (this->radius + c->radius) / sqrt(glm::dot(displacement, displacement));
		glm::vec3 halfD = displacement * .5f;
		*(this->center) -= halfD;
		*(c->center) += halfD;
		this->reEvaluate();
		c->reEvaluate();
	}
}

void SphereCollider::handleCubeCollision(CubeCollider* c)
{
	if (detectCubeCollision(c))
	{
		//CollisionHandler
	}
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
	glm::vec3 displacement = *(c->center) - *(this->center);
	float answer = glm::abs(displacement.x * displacement.x);
	answer += glm::abs(displacement.y * displacement.y);
	answer += glm::abs(displacement.z * displacement.z);
	return glm::sqrt(answer);
}