#include "SphereCollider.h"


SphereCollider::SphereCollider(Transform* GOTransform, float radius) : Collider(GOTransform)
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
		glm::vec3 cCenter = c->GOTransform->getPosition();
		glm::vec3 thisCenter = GOTransform->getPosition();
		glm::vec3 displacement = cCenter - thisCenter;
		displacement *= (this->radius + c->radius) / sqrt(glm::dot(displacement, displacement));
		glm::vec3 halfD = displacement * .5f;
		GOTransform->move(-halfD);
		c->GOTransform->move(halfD);
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