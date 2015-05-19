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
		collisionListener = new SphereCollisionDetector(*transform, transform->getScale().x * 0.5f);
		break;
	}
	}
}
void GameObject::addPhysicsComponent(float Mass, float MOI, bool softBody)
{
	physicsComponent = new PhysicsComponent(*transform, *collisionListener, Mass, MOI, softBody);
	HardBodyPhysics::addPhysicsComponent(physicsComponent);
}

void GameObject::draw()
{
	//Need to get deformation matrix to shader and didn't know how else to do it
	if (physicsComponent && physicsComponent->isSoftBody())
	{
		//auto deformVec = physicsComponent->getScaleVector();
		//transform->setScale(deformVec);
	}
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