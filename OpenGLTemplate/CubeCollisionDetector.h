#pragma once
#include "CollisionDetector.h"
#include "Transform.h"
class CubeCollisionDetector :
	public CollisionDetector
{
public:

	enum CollisionType {
		Cube,
		Sphere
	};

	//Constructors
	CubeCollisionDetector(Transform& GOTransform);
	~CubeCollisionDetector(void);

	//Methods
	bool detectCollision(CollisionDetector& c) override;
	//CollisionTypes
	bool detectCubeCollision(CubeCollisionDetector& c) override;
	bool detectSphereCollision(SphereCollisionDetector& c) override;

	bool getCollisionInfo(CollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact) override;

	//CollisionTypes
	bool getCubeCollisionInfo(CubeCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact) override;
	bool getSphereCollisionInfo(SphereCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact) override;

	CollisionDetector::ContainingBox getAABB() override;

private:
	std::vector<glm::vec3> getVertices();
	std::vector<std::vector<glm::vec3>> getEdges();
	std::vector<glm::vec3> getNormals();
	void addEdgeToList(std::vector<std::vector<glm::vec3>>* list, std::vector<glm::vec3>* points, int pointNum1, int pointNum2);
};

