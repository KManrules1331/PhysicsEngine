#pragma once
#include "Collider.h"
class CubeCollider :
	public Collider
{
public:
	//Attributes
	float sideLength;

	//Constructors
	CubeCollider(glm::vec3* center, float sideLength);
	~CubeCollider(void);

	//Methods
	void HandleCollision(Collider* c) override;
	void handleSphereCollision(SphereCollider* c) override;
	void handleCubeCollision(CubeCollider* c);

	//Methods
	bool detectCollision(Collider* c) override;
	bool detectSphereCollision(SphereCollider* c) override;
	bool detectCubeCollision(CubeCollider* c);
};

