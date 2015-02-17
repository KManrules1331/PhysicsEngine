#pragma once
#include "Collider.h"
class SphereCollider :
	public Collider
{
public:
	//Attributes
	float radius;

	//Constructors
	SphereCollider(glm::vec3* center, float radius);
	~SphereCollider(void);

	//Methods
	void HandleCollision(Collider* c) override;
	void handleSphereCollision(SphereCollider* c) override;
	void handleCubeCollision(CubeCollider* c) override;

	//Methods
	bool detectCollision(Collider* c) override;
	bool detectSphereCollision(SphereCollider* c) override;
	bool detectCubeCollision(CubeCollider* c) override;

private:
	//Methods
	float getDistance(Collider* c);
};

