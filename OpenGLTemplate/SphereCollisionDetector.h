#pragma once
#include "CollisionDetector.h"
#include "Transform.h"
class SphereCollisionDetector :
	public CollisionDetector
{
public:
	//Attributes
	float radius;

	//Constructors
	SphereCollisionDetector(Transform& GOTransform, float radius);
	~SphereCollisionDetector(void);

	//Methods
	bool detectCollision(CollisionDetector& c) override;
	bool detectSphereCollision(SphereCollisionDetector& c) override;
	bool detectCubeCollision(CubeCollisionDetector& c) override;

	CollisionDetector::ContainingBox getAABB() override;

private:
	//Methods
	float getDistance(glm::vec3 point);
};

