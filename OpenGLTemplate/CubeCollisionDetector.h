#pragma once
#include "CollisionDetector.h"
#include "Transform.h"
class CubeCollisionDetector :
	public CollisionDetector
{
public:
	//Constructors
	CubeCollisionDetector(Transform& GOTransform);
	~CubeCollisionDetector(void);

	//Methods
	bool detectCollision(CollisionDetector& c) override;
	//CollisionTypes
	bool detectCubeCollision(CubeCollisionDetector& c) override;
	bool detectSphereCollision(SphereCollisionDetector& c) override;

	CollisionDetector::ContainingBox getAABB() override;

private:
	std::vector<glm::vec3> getVertices();
};

