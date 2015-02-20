#pragma once
#include "OpenGL.h"
#include "Constants.h"
#include <vector>

class GJKCollisionDetection
{
public:
	//Methods
	static bool areColliding(std::vector<glm::vec3> aVertices, std::vector < glm::vec3> bVertices);
private:
	static glm::vec3 Support(std::vector<glm::vec3> vertices, glm::vec3 direction);
	static bool doSimplex(std::vector<glm::vec3>& vertices, glm::vec3& direction);
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
	};
};

