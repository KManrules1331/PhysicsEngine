#pragma once
#include "CollisionDetector.h"
#include "CubeCollisionDetector.h"
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

	bool getCollisionInfo(CollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact) override;
	//CollisionTypes
	bool getCubeCollisionInfo(CubeCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact) override;
	bool getSphereCollisionInfo(SphereCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact) override;

	//SAT dispatch
	bool getSATCollisionInfo(CubeCollisionDetector& c, Contact* contact) override;
	bool getSATCollisionInfo(SphereCollisionDetector& c, Contact* contact) override;
	bool getSATCollisionInfo(CollisionDetector& c, Contact* contact) override;

	CollisionDetector::ContainingBox getAABB() override;

private:
	//Methods
	float getDistance(glm::vec3 point);
};

