#include "Collider.h"


Collider::Collider(glm::vec3* center, glm::vec3* rotation, glm::vec3* scale)
{
	this->center = center;
	this->rotation = rotation;
	this->scale = scale;

	CollisionTree->insert(this);
}


Collider::~Collider(void)
{
	CollisionTree->remove(this);
}

Collider::ContainingBox Collider::getAABB()
{
	ContainingBox returnBox;
	returnBox.left = center->x - scale->x / 2;
	returnBox.right = center->x + scale->x / 2;
	returnBox.down = center->y - scale->y / 2;
	returnBox.up = center->y + scale->y / 2;
	returnBox.front = center->z - scale->z / 2;
	returnBox.back = center->z + scale->z / 2;

	return returnBox;
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
		if(!(potentialColliders[i] == this))
			HandleCollision(potentialColliders[i]);
	}
}

void Collider::reEvaluate()
{
	CollisionTree->remove(this);
	CollisionTree->insert(this);
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