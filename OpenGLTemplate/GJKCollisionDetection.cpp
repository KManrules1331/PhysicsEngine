#include "GJKCollisionDetection.h"

bool GJKCollisionDetection::areColliding(const std::vector<glm::vec3>& aVertices, const std::vector<glm::vec3>& bVertices)
{
	glm::vec3 initVertex = aVertices[0] - bVertices[0];
	Simplex* simplex = new Simplex();
	simplex->push_back(initVertex);
	glm::vec3 direction = -initVertex;
	while (true)
	{
		glm::vec3 vertex = getVertex(aVertices, bVertices, direction);
		if (glm::dot(vertex, direction) < 0) return false;
		simplex->push_back(vertex);
		if (simplex->doSimplex(direction)) return true;
	}
}

glm::vec3 GJKCollisionDetection::Support(const std::vector<glm::vec3>& vertices, const glm::vec3& direction)
{
	int n = vertices.size();
	glm::vec3 furthestPoint = vertices[0];
	float maxDistance = glm::dot(direction, vertices[0]);
	for (int i = 1; i < n; i++)
	{
		float distance = glm::dot(direction, vertices[i]);
		if (distance > maxDistance)
		{
			furthestPoint = vertices[i];
			maxDistance = distance;
		}
	}

	return furthestPoint;
}

glm::vec3 GJKCollisionDetection::getVertex(const std::vector<glm::vec3>& aVertices, const std::vector<glm::vec3>& bVertices, const glm::vec3& direction)
{
	return Support(aVertices, direction) - Support(bVertices, -direction);
}