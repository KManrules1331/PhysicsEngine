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
void GameObject::addPhysicsComponent(float Mass, float MOI)
{
	if (!collisionListener)
		assert(false);
	physicsComponent = new PhysicsComponent(*transform, *collisionListener, Mass, MOI);
	HardBodyPhysics::addPhysicsComponent(physicsComponent);
}

void GameObject::draw()
{
	if (mesh)
		mesh->draw(transform, &color[0]);
}

void GameObject::update(float dt)
{
	if (physicsComponent)
	{
		physicsComponent->update(dt);
	}
}

void GameObject::setColor(int r, int g, int b, int a)
{
	r = glm::clamp(r, 0, 256);
	g = glm::clamp(g, 0, 256);
	b = glm::clamp(b, 0, 256);
	a = glm::clamp(a, 0, 256);
	color.r = 1.0f / 256.0f * r;
	color.g = 1.0f / 256.0f * g;
	color.b = 1.0f / 256.0f * b;
	color.a = 1.0f / 256.0f * a;
}