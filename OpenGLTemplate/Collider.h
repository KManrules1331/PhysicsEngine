#pragma once
//System include
#include <vector>
#include <algorithm>

//Custom Include
#include "OpenGL.h"
#include "Transform.h"

//Forward Declarations
class SphereCollider;
class CubeCollider;

class Collider
{
public:
	//Attributes
	Transform* GOTransform;

	//Constructors
	Collider(Transform* GOTransform);
	~Collider(void);

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
	virtual ContainingBox getAABB() = 0;
	void CheckCollisions();
	virtual void HandleCollision(Collider* c) = 0;
	virtual void handleSphereCollision(SphereCollider* c) = 0;
	virtual void handleCubeCollision(CubeCollider* c) = 0;
	static void init();

	//Different types of collisions
	virtual bool detectCollision(Collider* c) = 0;
	virtual bool detectSphereCollision(SphereCollider* c) = 0;
	virtual bool detectCubeCollision(CubeCollider* c) = 0;

	void reEvaluate();

protected:
	//Methods
	bool AABBCollision(Collider* c);

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
		void insert(Collider* c);
		std::vector<Collider*>* retrieve(std::vector<Collider*>* returnObjects, Collider* c);
		void remove(Collider* c);
	
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
	
		std::vector<Collider*> colliders;
	
		//Methods
		void insert();
		void subdivide();
		int getIndex(Collider* c);
		Octree* getNode(int index);
		Octree* getNode(Collider* c);
		void queryRange();
	};

	static Collider::Octree* CollisionTree;
};