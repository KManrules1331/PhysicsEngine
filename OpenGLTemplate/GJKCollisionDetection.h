#pragma once
#include "CubeCollider.h"

class GJKCollisionDetection
{
public:
	GJKCollisionDetection();
	~GJKCollisionDetection();

	static bool Colliding(CubeCollider* a, CubeCollider* b);

private:
	static glm::vec3* GJKCollisionDetection::getVertices(CubeCollider* cube);
	static glm::vec3 GJKCollisionDetection::Support(glm::vec3 direction);
};

