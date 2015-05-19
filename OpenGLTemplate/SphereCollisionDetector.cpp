#include "SphereCollisionDetector.h"
#include "CubeCollisionDetector.h"


SphereCollisionDetector::SphereCollisionDetector(Transform& GOTransform, float radius) : CollisionDetector(GOTransform)
{
	this->radius = radius;
	this->radius = 0.5f;
}


SphereCollisionDetector::~SphereCollisionDetector(void)
{
}

bool SphereCollisionDetector::detectCollision(CollisionDetector& c)
{
	return c.detectSphereCollision(*this);
}
//DONE//
bool SphereCollisionDetector::detectSphereCollision(SphereCollisionDetector& c)
{
	return getDistance(c.GOTransform.getPosition()) <= c.radius + this->radius;
}
//This is NOT a perfect collision detection//USE AS MID PHASE CULLING
bool SphereCollisionDetector::detectCubeCollision(CubeCollisionDetector& c)
{
	glm::vec3 relPosition = transformVec3(GOTransform.getPosition(), c.GOTransform.getInverseMatrix());

	//Test by if the sphere is not touching the cube in any of the 3 dimensions
	if (glm::abs(relPosition.x) - radius > c.getHalfsize() ||
		glm::abs(relPosition.y) - radius > c.getHalfsize() ||
		glm::abs(relPosition.z) - radius > c.getHalfsize())
		return false;
	return true;
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

glm::vec3 SphereCollisionDetector::getAngularAcceleration(glm::vec3 torque, float inverseMass) const
{
	//This also is optimized to not use an intertia matrix because the moment of inertia is the same
	//about all axes
	if (inverseMass == 0)
		return glm::vec3();
	return torque * ((0.4f * radius * radius) / inverseMass);

}

bool SphereCollisionDetector::getCollisionInfo(CollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact)
{
	return c.getSphereCollisionInfo(*this, pointOfContact, normalOfContact);
}

//CollisionTypes
//This runs on the original dispatch track
bool SphereCollisionDetector::getCubeCollisionInfo(CubeCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact)
{
	return false;
}
bool SphereCollisionDetector::getSphereCollisionInfo(SphereCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact)
{
	return false;
}

//SAT dispatch track is below//////////////////////////////////////////////////

//Simple dummy method to dispatch properly//DONE//
bool SphereCollisionDetector::getSATCollisionInfo(CollisionDetector& c, Contact* contact)
{
	return c.getSATCollisionInfo(*this, contact);
}

//Cube to sphere collision
//Algorithm from Ian Millington
//I am 75% sure this will work but we should test it anyway 
//Assumes an absence of a prior test intersection query
//The boolean flag that is returned will serve as the pixel-perfect collision detection
bool SphereCollisionDetector::getSATCollisionInfo(CubeCollisionDetector& c, Contact* contact)
{
	//Find the point in the box that is closest to the sphere
	glm::vec3 closestPoint;
	float dist;

	//This is the sphere center in box local coordinates
	glm::vec3 localSphereCenter = transformVec3(GOTransform.getPosition(), c.GOTransform.getInverseMatrix());
	
	//Clamp coordinates of the closest point to the box 
	dist = localSphereCenter.x;
	if (dist > c.getHalfsize()) dist = c.getHalfsize();
	if (dist < -c.getHalfsize()) dist = -c.getHalfsize();
	closestPoint.x = dist;

	dist = localSphereCenter.y;
	if (dist > c.getHalfsize()) dist = c.getHalfsize();
	if (dist < -c.getHalfsize()) dist = -c.getHalfsize();
	closestPoint.y = dist;

	dist = localSphereCenter.z;
	if (dist > c.getHalfsize()) dist = c.getHalfsize();
	if (dist < -c.getHalfsize()) dist = -c.getHalfsize();
	closestPoint.z = dist;

	//Convert our point to world coordinates
	closestPoint = transformVec3(closestPoint, c.GOTransform.transformMatrix);

	//Check to see if it is actually colliding//This is our pixel perfect collision detection 
	glm::vec3 collisionLine = GOTransform.getPosition() - closestPoint;
	float distSqr = glm::dot(collisionLine, collisionLine);
	if (distSqr > square(radius))
		return false;

	//Now fill the contact data structure
	contact->normal = -glm::normalize(collisionLine);
	contact->position = closestPoint;
	contact->depth = radius - glm::sqrt(distSqr);

	return true;
}

//DONE//Cube to sphere collision//Just need to test this somehow
bool SphereCollisionDetector::getSATCollisionInfo(SphereCollisionDetector& c, Contact* contact)
{
	SphereCollisionDetector& a = *this;

	//Point of contact is located on the line between the centers
	//Contact always points at a
	glm::vec3 midline = a.GOTransform.getPosition() - c.GOTransform.getPosition();
	contact->normal = glm::normalize(midline);

	//This is probably redundant but I am running a test intersection query because I need to calculate depth anyway
	float depth = (a.radius + c.radius) - glm::length(midline);
	if (depth < 0)
		return false;

	//Find depth
	contact->depth = depth;

	//Find position
	//Using the point halfway between the spheres centers
	//Don't think it makes any difference if the contact moves up or down the midline
	contact->position = c.GOTransform.getPosition() + midline * 0.5f;

	return true;
}

