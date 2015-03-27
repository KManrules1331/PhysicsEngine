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

std::vector<std::vector<glm::vec3>> CubeCollisionDetector::getEdges()
{
	std::vector<glm::vec3> vertices = getVertices();
	std::vector<std::vector<glm::vec3>> returnVector;
	//Edge 0-1
	addEdgeToList(&returnVector, &vertices, 0, 1);
	//Edge 0-2
	addEdgeToList(&returnVector, &vertices, 0, 2);
	//Edge 0-4
	addEdgeToList(&returnVector, &vertices, 0, 4);
	//Edge 1-3
	addEdgeToList(&returnVector, &vertices, 1, 3);
	//Edge 1-5
	addEdgeToList(&returnVector, &vertices, 1, 5);
	//Edge 2-3
	addEdgeToList(&returnVector, &vertices, 2, 3);
	//Edge 2-6
	addEdgeToList(&returnVector, &vertices, 2, 6);
	//Edge 3-7
	addEdgeToList(&returnVector, &vertices, 3, 7);
	//Edge 4-5
	addEdgeToList(&returnVector, &vertices, 4, 5);
	//Edge 4-6
	addEdgeToList(&returnVector, &vertices, 4, 6);
	//Edge 5-7
	addEdgeToList(&returnVector, &vertices, 5, 7);
	//Edge 6-7
	addEdgeToList(&returnVector, &vertices, 6, 7);

	return returnVector;
}

std::vector<glm::vec3> CubeCollisionDetector::getNormals()
{
	std::vector<glm::vec3> returnVector;
	returnVector.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	returnVector.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	returnVector.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	returnVector.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	returnVector.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	returnVector.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	for (int i = 0; i < returnVector.size(); i++)
	{
		glm::vec4 multiplyVector = glm::vec4(returnVector[i], 1.0f);
		multiplyVector = GOTransform.rotationMatrix * multiplyVector;
		returnVector[i] = glm::vec3(multiplyVector);
	}

	return returnVector;
}

void CubeCollisionDetector::addEdgeToList(std::vector<std::vector<glm::vec3>>* list, std::vector<glm::vec3>* points, int pointNum1, int pointNum2)
{
	(*list).push_back(std::vector<glm::vec3>());
	int edgeNum = (*list).size();
	(*list)[edgeNum].push_back((*points)[pointNum1]);
	(*list)[edgeNum].push_back((*points)[pointNum2]);
}

bool CubeCollisionDetector::getCollisionInfo(CollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact)
{
	return c.getCubeCollisionInfo(*this, pointOfContact, normalOfContact);
}

bool CubeCollisionDetector::getCubeCollisionInfo(CubeCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact)
{
	//First, point of contact
		//Check for vertex in Mesh
	std::vector<glm::vec3> vertices = getVertices();
	std::vector<glm::vec3> collidingVertices;
	for (int i = 0; i < vertices.size(); i++)
	{
		std::vector<glm::vec3> fakeVertexList;
		fakeVertexList.push_back(vertices[i]);
		if (GJKCollisionDetection::areColliding(fakeVertexList, c.getVertices()))
		{
			collidingVertices.push_back(vertices[i]);
		}
	}
	if (collidingVertices.size() > 0)
	{
		//We have vertex in mesh, we can derive point of contact
		*pointOfContact = glm::vec3();
		for (int i = 0; i < collidingVertices.size(); i++)
		{
			*pointOfContact += collidingVertices[i];
		}
		*pointOfContact /= collidingVertices.size();

		glm::vec3 diff = *pointOfContact - c.GOTransform.getPosition();
		std::vector<glm::vec3> normals = c.getNormals();
		int index;
		float max = 0;
		for (int i = 0; i < normals.size(); i++)
		{
			float weight = glm::dot(diff, normals[i]);
			if (weight > max)
			{
				max = weight;
				index = i;
			}
		}

		*normalOfContact = normals[index];
	}
	else
	{
		//Check for edge on edge
		std::vector<std::vector<glm::vec3>> edges = getEdges();
		int numEdgesColliding = 0;
		for (int i = 0; i < edges.size(); i++)
		{
			if (GJKCollisionDetection::areColliding(edges[i], c.getVertices()))
			{
				for (int j = 0; j < edges[i].size(); j++)
				{
					*pointOfContact += edges[i][j];
				}
				numEdgesColliding += 2;
			}
		}
		*pointOfContact /= numEdgesColliding;

		if (numEdgesColliding > 0)
		{
			std::vector<std::vector<glm::vec3>> otherEdges = c.getEdges();
			int numEdgesColliding2 = 0;
			glm::vec3 normal = glm::vec3();
			for (int i = 0; i < edges.size(); i++)
			{
				if (GJKCollisionDetection::areColliding(getVertices(), edges[i]))
				{
					for (int j = 0; j < edges[i].size(); j++)
					{
						normal += edges[i][j] - c.GOTransform.getPosition();
					}
					numEdgesColliding2 += 2;
				}
			}
			normal /= numEdgesColliding2;

			*normalOfContact = normal;
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CubeCollisionDetector::getSphereCollisionInfo(SphereCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact)
{
	return false;
}