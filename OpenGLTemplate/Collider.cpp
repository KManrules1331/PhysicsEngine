#include "Collider.h"


Collider::Collider(Transform* GOTransform)
{
	this->GOTransform = GOTransform;

	CollisionTree->insert(this);
}


Collider::~Collider(void)
{
	CollisionTree->remove(this);
}

Collider::Octree* Collider::CollisionTree;

void Collider::init()
{
	CollisionTree = new Octree(glm::vec3(0.0f), 100.0f);
}

void Collider::CheckCollisions()
{
	std::vector<Collider*> potentialColliders;
	CollisionTree->retrieve(&potentialColliders, this);
	for(int i = 0; i < potentialColliders.size(); i++)
	{
		if (!(potentialColliders[i] == this) && detectCollision(potentialColliders[i]))
			//TODO: The results of this collision detection needs to
			// be reflected in either the detectCollision method or outside
			// here.
			collisionNotifier.notify(potentialColliders[i], Event::Collision);
	}
}

void Collider::reEvaluate()
{
	CollisionTree->remove(this);
	CollisionTree->insert(this);
}

Collider::ContainingBox Collider::getAABB()
{
	ContainingBox returnBox;
	float factor = sqrt(3) / 2;
	glm::vec3 center = GOTransform->getPosition();
	glm::vec3 scale = GOTransform->getScale();
	returnBox.right = center.x + scale.x * factor;
	returnBox.left = center.x - scale.x * factor;
	returnBox.up = center.y + scale.y * factor;
	returnBox.down = center.y - scale.y * factor;
	returnBox.front = center.z - scale.z * factor;
	returnBox.back = center.z + scale.z * factor;

	return returnBox;
}

bool Collider::AABBCollision(Collider* c)
{
	ContainingBox a = this->getAABB();
	ContainingBox b = c->getAABB();

	if (a.down < b.up && a.up > b.down)
	{
		if (a.left < b.right && a.right > b.left)
		{
			if (a.back < b.front && a.front > b.back)
			{
				return true;
			}
		}
	}

	return false;
}

void Collider::addListener(Observer* observer)
{
	collisionNotifier.addObserver(observer);
}

void Collider::removeListener(Observer* observer)
{
	collisionNotifier.removeObserver(observer);
}