#include "GJKCollisionDetection.h"

std::vector<glm::vec3> GJKCollisionDetection::getMinnowDifference(std::vector<glm::vec3> aVertices, std::vector<glm::vec3> bVertices)
{
	std::vector<glm::vec3> returnVector;

	for (int i = 0; i < aVertices.size(); i++)
	{
		for (int j = 0; j < bVertices.size(); j++)
		{
			returnVector.push_back(aVertices[i] - bVertices[j]);
		}
	}

	return returnVector;
}