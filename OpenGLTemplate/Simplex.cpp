#include "GJKCollisionDetection.h"

GJKCollisionDetection::Simplex::Simplex()
{
	size = 0;
	for (int i = 0; i < 4; i++)
	{
		vertices[i] = glm::vec3(0.0f);
	}
}

bool GJKCollisionDetection::Simplex::push_back(glm::vec3 vertex)
{
	if (size >= 4) return false;
	vertices[size] = vertex;
	size++;
	return true;
}

bool GJKCollisionDetection::Simplex::remove(int i)
{
	if (size == 0) return false;
	for (int j = i; j < size-1; j++)
	{
		vertices[j] = vertices[j + 1];
	}
	vertices[size - 1] = glm::vec3(0.0f);
	size--;
	return true;
}

bool GJKCollisionDetection::Simplex::doSimplex(glm::vec3& direction)
{
	switch (size)
	{
	case 2:
		glm::vec3 a = vertices[1];
		glm::vec3 b = vertices[0];
		return lineCase(b, a, direction);
		break;
	case 3:
		glm::vec3 a = vertices[2];
		glm::vec3 b = vertices[1];
		glm::vec3 c = vertices[0];
		return triangleCase(a, b, c, direction);
		break;
	case 4:
		break;
	}
}

bool GJKCollisionDetection::Simplex::lineCase(glm::vec3 a, glm::vec3 b, glm::vec3& direction)
{
	if (glm::dot(b - a, a) >= 0)
	{
		vertices[0] = b;
		vertices[1] = a;
		size = 2;
		direction = glm::cross(glm::cross(b - a, a), b - a);
	}
	else
	{
		vertices[0] = a;
		size = 1;
		direction = a;
	}
	return false;
}

bool GJKCollisionDetection::Simplex::triangleCase(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3& direction)
{
	glm::vec3 abc = glm::cross(b - a, c - a);
	if (glm::dot(glm::cross(abc, c - a), a) >= 0)
	{
		if (glm::dot(c - a, a) >= 0)
		{
			vertices[0] = c;
			vertices[1] = a;
			size = 2;
			direction = glm::cross(glm::cross(c - a, a), c - a);
			return false;
		}
		else return lineCase(a, b, direction);
	}
	else
	{
		if (glm::dot(glm::cross(b - a, abc), a) >= 0)
		{
			return lineCase(a, b, direction);
		}
		else
		{
			if (glm::dot(abc, a) >= 0)
			{
				direction = abc;
			}
			else
			{
				vertices[0] = b;
				vertices[1] = c;
				direction = -abc;
			}
		}
	}
}