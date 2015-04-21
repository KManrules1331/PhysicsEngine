#include "GameObject.h"


GameObject::GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	
	this->transform = new Transform(position, glm::quat(rotation), scale);
}


GameObject::~GameObject(void)
{
	delete transform;
	delete collisionListener;
	if (physicsComponent)
		HardBodyPhysics::removePhysicsComponent(physicsComponent);
	delete physicsComponent;
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
void GameObject::addPhysicsComponent(float Mass, float MOI)
{
	physicsComponent = new PhysicsComponent(*transform, *collisionListener, Mass, MOI);
	HardBodyPhysics::addPhysicsComponent(physicsComponent);
}

void GameObject::draw()
{
	if (mesh)
		mesh->draw(transform);
}

void GameObject::update(float dt)
{
	if (physicsComponent)
	{
		physicsComponent->update(dt);
	}
}