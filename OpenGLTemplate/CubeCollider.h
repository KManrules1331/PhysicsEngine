#pragma once
#include "Collider.h"
#include "Transform.h"
class CubeCollider :
	public Collider
{
public:
	//Attributes
	float sideLength;

	//Constructors
	CubeCollider(Transform* GOTransform);
	~CubeCollider(void);

	//Methods
	bool detectCollision(Collider* c) override;
	bool detectSphereCollision(SphereCollider* c) override;
	bool detectCubeCollision(CubeCollider* c) override;

	Collider::ContainingBox getAABB() override;
private:
	std::vector<glm::vec3> getVertices();
};

