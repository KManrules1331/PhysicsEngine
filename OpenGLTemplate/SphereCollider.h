#pragma once
#include "Collider.h"
#include "Transform.h"
class SphereCollider :
	public Collider
{
public:
	//Attributes
	float radius;

	//Constructors
	SphereCollider(Transform* GOTransform, float radius);
	~SphereCollider(void);

	//Methods
	bool detectCollision(Collider* c) override;
	bool detectSphereCollision(SphereCollider* c) override;
	bool detectCubeCollision(CubeCollider* c) override;

	Collider::ContainingBox getAABB() override;

private:
	//Methods
	float getDistance(Collider* c);
};

