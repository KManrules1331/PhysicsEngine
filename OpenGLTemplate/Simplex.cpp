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
	glm::vec3 a, b, c, d;
	switch (size)
	{
	case 2:
		a = vertices[1];
		b = vertices[0];
		return lineCase(a, b, direction);
		break;
	case 3:
		a = vertices[2];
		b = vertices[1];
		c = vertices[0];
		return triangleCase(a, b, c, direction);
		break;
	case 4:
		a = vertices[3];
		b = vertices[2];
		c = vertices[1];
		d = vertices[0];
		return prismCase(a, b, c, d, direction);
		break;
	}
}

bool GJKCollisionDetection::Simplex::lineCase(glm::vec3 a, glm::vec3 b, glm::vec3& direction)
{
	if (glm::dot(b - a, -a) >= 0)
	{
		vertices[0] = b;
		vertices[1] = a;
		size = 2;
		direction = glm::cross(glm::cross(b - a, -a), b - a);
	}
	else
	{
		//This case should never come up?  We try to overshoot the origin,
		//And if the vertex we just added doesn't overshoot it, then we should
		//fail before doing simplex calculations.
		vertices[0] = a;
		size = 1;
		direction = -a;
	}
	return false;
}

bool GJKCollisionDetection::Simplex::triangleCase(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3& direction)
{
	glm::vec3 abc = getTriangleNormal(a, b, c);
	if (glm::dot(glm::cross(c - a, abc), -a) >= 0)
	{
		if (glm::dot(c - a, -a) >= 0)
		{
			vertices[0] = c;
			vertices[1] = a;
			size = 2;
			direction = glm::cross(glm::cross(c - a, -a), c - a);
			return false;
		}
		else return lineCase(a, b, direction);
	}
	else
	{
		if (glm::dot(glm::cross(abc, b - a), -a) >= 0)
		{
			return lineCase(a, b, direction);
		}
		else
		{
			if (glm::dot(abc, -a) >= 0)
			{
				vertices[0] = c;
				vertices[1] = b;
				vertices[2] = a;
				size = 3;
				direction = abc;
				return false;
			}
			else
			{
				vertices[0] = b;
				vertices[1] = c;
				vertices[2] = a;
				size = 3;
				direction = -abc;
				return false;
			}
		}
	}
}

bool GJKCollisionDetection::Simplex::prismCase(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec3& direction)
{
	//In this case, bcd make the bottom of the prism, with A being
	//the newly added vertex.  The origin cannot be contained below bcd,
	//but must be contained within the prism of bcd in the direction of
	//the perpendicular of bcd

	//Normals of triangles, we want to point inside of prism
	glm::vec3 abc = getTriangleNormal(a, c, b);
	glm::vec3 adb = getTriangleNormal(a, b, d);
	glm::vec3 acd = getTriangleNormal(a, d, c);
	glm::vec3 bdc = getTriangleNormal(b, c, d);

	if (glm::dot(abc, -a) >= 0)
	{
		if (glm::dot(adb, -a) >= 0)
		{
			if (glm::dot(acd, -a) >= 0)
			{
				return true;
			}
			else
			{
				return triangleCase(a, c, d, direction);
			}
		}
		else
		{
			return triangleCase(a, d, b, direction);
		}
	}
	else
	{
		return triangleCase(a, b, c, direction);
	}
}

//Give vertices in CW order
glm::vec3 GJKCollisionDetection::Simplex::getTriangleNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
	return glm::cross(c - a, b - a);	//vec3 points outward
}