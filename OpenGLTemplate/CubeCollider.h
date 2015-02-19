#pragma once
#include "Collider.h"
class CubeCollider :
	public Collider
{
public:
	//Attributes
	float sideLength;

	//Constructors
	CubeCollider(Transform* GOTransform, float sideLength);
	~CubeCollider(void);

	//Methods
	void HandleCollision(Collider* c) override;
	void handleSphereCollision(SphereCollider* c) override;
	void handleCubeCollision(CubeCollider* c) override;

	//Methods
	bool detectCollision(Collider* c) override;
	bool detectSphereCollision(SphereCollider* c) override;
	bool detectCubeCollision(CubeCollider* c) override;

	Collider::ContainingBox getAABB() override;
};

