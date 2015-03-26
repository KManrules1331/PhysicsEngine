#include "GameObject.h"


GameObject::GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->transform = new Transform(position, rotation, scale);
}


GameObject::~GameObject(void)
{
	delete transform;
	delete collisionListener;
}

void GameObject::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}
void GameObject::addCollisionDetector(CollisionDetector::DetectorType t)
{
	switch (t)
	{
	case CollisionDetector::DetectorType::Cube:
	{
		collisionListener = new CubeCollisionDetector(*transform);
		break;
	}
	case CollisionDetector::DetectorType::Sphere:
	{
		collisionListener = new SphereCollisionDetector(*transform, transform->getScale().x);
		break;
	}
	}
}

void GameObject::draw()
{
	if (mesh)
		mesh->draw(transform);
}

void GameObject::update()
{
	if (collisionListener)
	{
		std::vector<CollisionDetector*> CollidingBodies;
		collisionListener->GetCollidingCollisionDetectors(&CollidingBodies);
		if (CollidingBodies.size() > 1)
			std::cout << "Colliding" << std::endl;
	}
}