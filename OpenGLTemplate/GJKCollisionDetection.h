#pragma once
#include "OpenGL.h"
#include "Constants.h"
#include <vector>

class GJKCollisionDetection
{
public:
	//Methods
	static bool areColliding(const std::vector<glm::vec3>& aVertices, const std::vector<glm::vec3>& bVertices);
private:
	static glm::vec3 Support(const std::vector<glm::vec3>& vertices, const glm::vec3& direction);
	static glm::vec3 getVertex(const std::vector<glm::vec3>& aVertices, const std::vector<glm::vec3>& bVertices, const glm::vec3& direction);
	class Simplex
	{
	public:
		//Attributes
		int size;

		//Constructors
		Simplex();
		~Simplex();

		//Methods
		bool push_back(glm::vec3 vertex);
		bool remove(int i);
		bool doSimplex(glm::vec3& direction);

	private:
		glm::vec3 vertices[4];

		bool lineCase(glm::vec3 a, glm::vec3 b, glm::vec3& direction);
		bool triangleCase(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3& direction);
		bool prismCase(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec3& direction);

		glm::vec3 getTriangleNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
	};
};

