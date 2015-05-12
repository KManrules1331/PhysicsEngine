#include "CollisionDetector.h"


CollisionDetector::CollisionDetector(Transform& GameObjectTransform) : GOTransform{ GameObjectTransform }
{
	this->GOTransform = GOTransform;

	CollisionTree->insert(this);
}


CollisionDetector::~CollisionDetector(void)
{
	CollisionTree->remove(this);
}

CollisionDetector::Octree* CollisionDetector::CollisionTree;

void CollisionDetector::init()
{
	CollisionTree = new Octree(glm::vec3(0.0f), 100.0f);
}

//Broad phase culling//No modification necessary for SAT
//Return a vector of collisionDetectors that could be colliding with this
void CollisionDetector::GetCollidingCollisionDetectors(std::vector<CollisionDetector*>* CollidingDetectors)
{
	CollisionTree->retrieve(CollidingDetectors, *this);
	for(unsigned int i = 0; i < CollidingDetectors->size(); i++)
	{
		if (!((*CollidingDetectors)[i] == this))
		{
			//Colliders could be colliding, further tests required
			if (!detectCollision(*(*CollidingDetectors)[i]))
				CollidingDetectors->erase(CollidingDetectors->begin() + i);
		}
	}
}

void CollisionDetector::reEvaluate()
{
	CollisionTree->remove(this);
	CollisionTree->insert(this);
}

CollisionDetector::ContainingBox CollisionDetector::getAABB()
{
	ContainingBox returnBox;
	float factor = sqrt(3) / 2;
	glm::vec3 center = GOTransform.getPosition();
	glm::vec3 scale = GOTransform.getScale();
	returnBox.right = center.x + scale.x * factor;
	returnBox.left = center.x - scale.x * factor;
	returnBox.up = center.y + scale.y * factor;
	returnBox.down = center.y - scale.y * factor;
	returnBox.front = center.z - scale.z * factor;
	returnBox.back = center.z + scale.z * factor;

	return returnBox;
}

bool CollisionDetector::AABBCollision(CollisionDetector& c)
{
	ContainingBox a = getAABB();
	ContainingBox b = c.getAABB();

	if (a.down < b.up && a.up > b.down)
	{
		if (a.left < b.right && a.right > b.left)
		{
			if (a.back > b.front && a.front < b.back)
			{
				return true;
			}
		}
	}

	return false;
}