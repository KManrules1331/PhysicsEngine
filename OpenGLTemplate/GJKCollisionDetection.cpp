#include "GJKCollisionDetection.h"

bool GJKCollisionDetection::areColliding(std::vector<glm::vec3> aVertices, std::vector<glm::vec3> bVertices)
{
	glm::vec3 initVertex = aVertices[0] - bVertices[0];
	Simplex* simplex = new Simplex();
	simplex->push_back(initVertex);
	glm::vec3 direction = -initVertex;
	while (true)
	{
		glm::vec3 vertex = Support(aVertices, direction) - Support(bVertices, direction);
		if (glm::dot(vertex, direction) < 0) return false;
		simplex->push_back(vertex);
		if (simplex->doSimplex(direction)) return true;
	}
}

glm::vec3 GJKCollisionDetection::Support(std::vector<glm::vec3> vertices, glm::vec3 direction)
{
	int n = vertices.size();
	glm::vec3 furthestPoint = vertices[0];
	float maxDistance = 0.0f;
	for (int i = 0; i < n; i++)
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

bool GJKCollisionDetection::doSimplex(std::vector<glm::vec3>& vertices, glm::vec3& direction)
{
	int size = vertices.size();
	switch (size)
	{
	case 2:
		if (glm::dot(vertices[1] - vertices[0], vertices[0]) >= 0)
		{

		}
		else
		{
			vertices.
		}
		break;
	case 3:
		break;
	case 4:
		break;
	}
}