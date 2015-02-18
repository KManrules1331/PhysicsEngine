#pragma once
//System includes

//Custom includes
#include "OpenGL.h"
#include "Mesh.h"
#include "SphereCollider.h"
#include "Transform.h"

class GameObject
{
public:
	//Attributes
	Transform* transform;	//OWNS TRANSFORM
	Mesh* mesh;				//DOES NOT OWN MESH
	Collider* collider;		//OWNS COLLIDER

	enum Primitive {
		Cube,
		Sphere,
		Square,
		Circle
	} type;

	//Constructors
	GameObject(Primitive p, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~GameObject(void);

	//Methods
	void draw();
	void update();
};

