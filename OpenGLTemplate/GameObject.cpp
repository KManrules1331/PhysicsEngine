#include "GameObject.h"


GameObject::GameObject(Primitive p, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->type = p;
	this->transform = new Transform(position, rotation, scale);

	switch(type)
	{
	case Primitive::Cube:
		{
			this->mesh = Mesh::cubeMesh;
			this->collider = new CubeCollider(this->transform);
			break;
		}
	case Primitive::Sphere:
		{
			this->mesh = Mesh::sphereMesh;
			this->collider = new SphereCollider(this->transform, scale.x/2);
			break;
		}
	case Primitive::Square:
		{
			this->mesh = Mesh::squareMesh;
			this->collider = new SphereCollider(this->transform, scale.x / 2);
			break;
		}
	case Primitive::Circle:
		{
			this->mesh = Mesh::circleMesh;
			this->collider = new SphereCollider(this->transform, scale.x);
			break;
		}
	}
}


GameObject::~GameObject(void)
{
	delete transform;
	delete collider;
}

void GameObject::draw()
{
	mesh->draw(transform);
}

void GameObject::update()
{
	collider->CheckCollisions();
}