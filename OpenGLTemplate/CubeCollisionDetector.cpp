#include "CubeCollisionDetector.h"
#include <iostream>


CubeCollisionDetector::CubeCollisionDetector(Transform& GOTransform) : CollisionDetector(GOTransform)
{
}


CubeCollisionDetector::~CubeCollisionDetector()
{
}

bool CubeCollisionDetector::detectCollision(CollisionDetector& c)
{
	if (AABBCollision(c))
	{
		return c.detectCubeCollision(*this);
	}
	return false;
}

bool CubeCollisionDetector::detectSphereCollision(SphereCollisionDetector& c)
{
	return false;
}

bool CubeCollisionDetector::detectCubeCollision(CubeCollisionDetector& c)
{
	std::vector<glm::vec3> vertices = getVertices();
	std::vector<glm::vec3> otherVertices = c.getVertices();
	return GJKCollisionDetection::areColliding(vertices, otherVertices);
}

CollisionDetector::ContainingBox CubeCollisionDetector::getAABB()
{
	ContainingBox returnBox;
	float factor = sqrt(3) / 2;
	glm::vec3 center = GOTransform.getPosition();
	glm::vec3 scale = GOTransform.getScale();
	returnBox.right = center.x + scale.x * factor;
	returnBox.left = center.x - scale.x * factor;
	returnBox.up = center.y + scale.y * factor;
	returnBox.down = center.y - scale.y * factor;
	returnBox.front = center.z - scale.z * factor;
	returnBox.back = center.z + scale.z * factor;

	return returnBox;
}

std::vector < glm::vec3> CubeCollisionDetector::getVertices()
{
	//Code specific to the cube collider
	std::vector<glm::vec3> returnVector;
	returnVector.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
	returnVector.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
	returnVector.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));
	returnVector.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));
	returnVector.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
	returnVector.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
	returnVector.push_back(glm::vec3(0.5f, 0.5f, -0.5f));
	returnVector.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
	for (int i = 0; i < returnVector.size(); i++)
	{
		glm::vec4 multiplyVector = glm::vec4(returnVector[i], 1.0f);
		multiplyVector = GOTransform.transformMatrix * multiplyVector;
		returnVector[i] = glm::vec3(multiplyVector);
	}

	return returnVector;
}