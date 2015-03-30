#pragma once
//System includes

//Custom includes
#include "OpenGL.h"
#include "Mesh.h"
#include "PhysicsComponent.h"
#include "HardBodyPhysics.h"
#include "SphereCollisionDetector.h"
#include "CubeCollisionDetector.h"
#include "Transform.h"

class GameObject
{
public:
	//Attributes
	Transform* transform;	//OWNS TRANSFORM
	Mesh* mesh;				//DOES NOT OWN MESH
	CollisionDetector* collisionListener;		//OWNS COLLIDER
	PhysicsComponent* physicsComponent;

	enum Primitive {
		Cube,
		Sphere,
		Square,
		Circle
	};

	//Constructors
	GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~GameObject(void);

	//InitializerMethods
	void setMesh(Mesh* mesh);
	void addCollisionDetector(CollisionDetector::DetectorType t);
	void addPhysicsComponent(float Mass, float MOI);

	//Methods
	void draw();
	void update();
};

