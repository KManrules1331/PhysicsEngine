#pragma once
//System include
#include <vector>
#include <algorithm>

//Custom Include
#include "OpenGL.h"
#include "GJKCollisionDetection.h"
#include "Transform.h"

//Store all data necessary for collision resolution
struct Contact{
	glm::vec3 position;
	glm::vec3 normal;
	float depth;
};

//Forward Declarations
class CubeCollisionDetector;
class SphereCollisionDetector;

class CollisionDetector
{
public:

	enum DetectorType {
		Cube,
		Sphere
	};

	//Destructors
	virtual ~CollisionDetector();

	//Attributes
	Transform& GOTransform;

	//AABB type
	struct ContainingBox
	{
		float up;
		float down;
		float right;
		float left;
		float front;
		float back;
	};

	//Methods
	virtual ContainingBox getAABB();
	void GetCollidingCollisionDetectors(std::vector<CollisionDetector*>* CollidingDetectors);
	static void init();

	//Different types of collisions
	virtual bool detectCollision(CollisionDetector& c) = 0;

	//CollisionTypes
	virtual bool detectCubeCollision(CubeCollisionDetector& c) = 0;
	virtual bool detectSphereCollision(SphereCollisionDetector& c) = 0;

	virtual bool getCollisionInfo(CollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact) = 0;

	//CollisionTypes
	virtual bool getCubeCollisionInfo(CubeCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact) = 0;
	virtual bool getSphereCollisionInfo(SphereCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact) = 0;

	//New dispatch track//I think I have all the methods I need here
	virtual bool getSATCollisionInfo(CubeCollisionDetector& c, Contact* contact) = 0;
	virtual bool getSATCollisionInfo(SphereCollisionDetector& c, Contact* contact) = 0;
	virtual bool getSATCollisionInfo(CollisionDetector& c, Contact* contact) = 0;

	//For inertia tensors
	//Takes angle axis representing torque in world coords and returns deltaAcceleration in world coords
	virtual glm::vec3 getAngularAcceleration(glm::vec3 torque, float inverseMass) const = 0;

	void reEvaluate();

protected:
	//Constructors
	CollisionDetector(Transform& GOTransform);

	//Methods
	bool AABBCollision(CollisionDetector& c);

private:
	class Octree
	{
	public:
		//Attributes
	
		//Constructors/Destructors
		Octree(glm::vec3 center, float halfWidth);
		~Octree(void);
	
		//Methods
		void clear();
		void insert(CollisionDetector* c);
		std::vector<CollisionDetector*>* retrieve(std::vector<CollisionDetector*>* returnObjects, CollisionDetector& c);
		void remove(CollisionDetector* c);
	
	private:
		//Attributes
		static const int TREE_NODE_CAPACITY;
	
		Octree* UpRightFront;
		Octree* UpRightBack;
		Octree* UpLeftFront;
		Octree* UpLeftBack;
		Octree* DownRightFront;
		Octree* DownRightBack;
		Octree* DownLeftFront;
		Octree* DownLeftBack;
		bool divided;
	
		glm::vec3 center;
		float halfWidth;
	
		std::vector<CollisionDetector*> colliders;
	
		//Methods
		void subdivide();
		int getIndex(CollisionDetector& c);
		Octree* getNode(int index);
		Octree* getNode(CollisionDetector& c);
		void queryRange();

		glm::mat3 inertiaTensor;
	};

	static CollisionDetector::Octree* CollisionTree;
};