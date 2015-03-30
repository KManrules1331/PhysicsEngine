#pragma once
//System include
#include <vector>
#include <algorithm>

//Custom Include
#include "OpenGL.h"
#include "GJKCollisionDetection.h"
#include "Transform.h"

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
	};

	static CollisionDetector::Octree* CollisionTree;
};