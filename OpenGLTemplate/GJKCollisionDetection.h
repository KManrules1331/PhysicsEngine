#pragma once
#include "OpenGL.h"
#include "Constants.h"
#include <vector>

class GJKCollisionDetection
{
public:
	//Methods
	static std::vector<glm::vec3> getMinnowDifference(std::vector<glm::vec3> aVertices, std::vector<glm::vec3> bVertices);
private:

};

