#include "GameObject.h"


GameObject::GameObject() : m_transform()
{
}

GameObject::GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
	m_transform(position, glm::quat(rotation), scale)
{
}


GameObject::~GameObject(void)
{
	delete collisionListener;
	if (physicsComponent)
		HardBodyPhysics::removePhysicsComponent(physicsComponent);
	delete physicsComponent;
}

const Transform & GameObject::getTransform() const
{
	return m_transform;
}

Transform & GameObject::getTransform()
{
	return m_transform;
}

void GameObject::setTransform(const Transform & newTransform)
{
	m_transform = newTransform;
}

void GameObject::setMesh(Mesh* mesh)
{
	this->m_mesh = mesh;
}
void GameObject::addCollisionDetector(CollisionDetector::DetectorType t)
{
	switch (t)
	{
	case CollisionDetector::DetectorType::Cube:
	{
		collisionListener = new CubeCollisionDetector(m_transform);
		break;
	}
	case CollisionDetector::DetectorType::Sphere:
	{
		collisionListener = new SphereCollisionDetector(m_transform, m_transform.getScale().x * 0.5f);
		break;
	}
	}
}
void GameObject::addPhysicsComponent(float Mass, float MOI)
{
	if (!collisionListener)
		assert(false);
	physicsComponent = new PhysicsComponent(m_transform, *collisionListener, Mass, MOI);
	HardBodyPhysics::addPhysicsComponent(physicsComponent);
}

void GameObject::draw()
{
	if (m_mesh != nullptr)
		m_mesh->draw(&m_transform, &color[0]);
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