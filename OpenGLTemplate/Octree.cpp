#include "CollisionDetector.h"

#pragma region Constructor / Destructor

CollisionDetector::Octree::Octree(glm::vec3 center, float halfWidth)
{
	this->center = center;
	this->halfWidth = halfWidth;
	divided = false;
}

CollisionDetector::Octree::~Octree(void)
{
	clear();
}

#pragma endregion

#pragma region Methods

#pragma region Public

void CollisionDetector::Octree::clear()
{
	colliders.clear();

	delete UpRightFront;
	delete UpRightBack;
	delete UpLeftFront;
	delete UpLeftBack;
	delete DownRightFront;
	delete DownRightBack;
	delete DownLeftFront;
	delete DownLeftBack;

	UpRightFront = nullptr;
	UpRightBack = nullptr;
	UpLeftFront = nullptr;
	UpLeftBack = nullptr;
	DownRightFront = nullptr;
	DownRightBack = nullptr;
	DownLeftFront = nullptr;
	DownLeftBack = nullptr;

	divided = false;
}

void CollisionDetector::Octree::insert(CollisionDetector* c)
{
	if(divided)
	{
		Octree* node = getNode(*c);

		if(node)
		{
			node->insert(c);
			return;
		}
	}

	colliders.push_back(c);

	if(!divided && colliders.size() > TREE_NODE_CAPACITY)
	{
		subdivide();

		unsigned int i = 0;
		while(i < colliders.size())
		{
			Octree* node = getNode(*colliders[i]);
			if(node)
			{
				node->insert(colliders[i]);
				colliders.erase(colliders.begin() + i);
			}
			else
			{
				i++;
			}
		}
	}
}

std::vector<CollisionDetector*>* CollisionDetector::Octree::retrieve(std::vector<CollisionDetector*>* returnObjects, CollisionDetector& c)
{
	Octree* node = getNode(c);
	if(node)
	{
		node->retrieve(returnObjects, c);
	}
	else if(divided)
	{
		for(int i = 0; i < 8; i++)
		{
			getNode(i)->retrieve(returnObjects, c);
		}
	}
	
	returnObjects->insert(returnObjects->end(), colliders.begin(), colliders.end());
	return returnObjects;
}

void CollisionDetector::Octree::remove(CollisionDetector* c)
{
	Octree* node = nullptr;
	for(int i = 0; i < 8; i++)
	{
		node = getNode(i);
		if(node != nullptr)
		{
			node->remove(c);
		}
	}

	colliders.erase(std::remove(colliders.begin(), colliders.end(), c), colliders.end());
}

#pragma endregion

#pragma region Private

const int CollisionDetector::Octree::TREE_NODE_CAPACITY = 8;

void CollisionDetector::Octree::subdivide()
{
	UpRightFront = new Octree(glm::vec3(center.x + halfWidth/2, center.y + halfWidth/2, center.z + halfWidth/2), halfWidth/2);
	UpRightBack = new Octree(glm::vec3(center.x + halfWidth/2, center.y + halfWidth/2, center.z - halfWidth/2), halfWidth/2);
	UpLeftFront = new Octree(glm::vec3(center.x - halfWidth/2, center.y + halfWidth/2, center.z + halfWidth/2), halfWidth/2);
	UpLeftBack = new Octree(glm::vec3(center.x - halfWidth/2, center.y + halfWidth/2, center.z - halfWidth/2), halfWidth/2);
	DownRightFront = new Octree(glm::vec3(center.x + halfWidth/2, center.y - halfWidth/2, center.z + halfWidth/2), halfWidth/2);
	DownRightBack = new Octree(glm::vec3(center.x + halfWidth/2, center.y - halfWidth/2, center.z - halfWidth/2), halfWidth/2);
	DownLeftFront = new Octree(glm::vec3(center.x - halfWidth/2, center.y - halfWidth/2, center.z + halfWidth/2), halfWidth/2);
	DownLeftBack = new Octree(glm::vec3(center.x - halfWidth/2, center.y - halfWidth/2, center.z - halfWidth/2), halfWidth/2);
	divided = true;
}

// getIndex
// Method determines in what octree a CollisionDetector fully belongs to
// 0: UpRightFront
// 1: UpRightBack
// 2: UpLeftFront
// 3: UpLeftBack
// 4: DownRightFront
// 5: DownRightBack
// 6: DownLeftFront
// 7: DownLeftBack
// -1: Only fits in this quadtree
int CollisionDetector::Octree::getIndex(CollisionDetector& c)
{
	int index = -1;

	if(!divided) return index;
	CollisionDetector::ContainingBox contBox = c.getAABB();
	bool upHalf = (contBox.down > center.y);
	bool downHalf = (contBox.up < center.y);
	bool rightHalf = (contBox.left > center.x);
	bool leftHalf = (contBox.right < center.x);
	bool frontHalf = (contBox.back > center.z);
	bool backHalf = (contBox.front < center.z);

	if(upHalf)
	{
		if(rightHalf)
		{
			if(frontHalf)
			{
				index = 0;
			}
			else if(backHalf)
			{
				index = 1;
			}
		}
		else if(leftHalf)
		{
			if(frontHalf)
			{
				index = 2;
			}
			else if(backHalf)
			{
				index = 3;
			}
		}
	}
	else if(downHalf)
	{
		if(rightHalf)
		{
			if(frontHalf)
			{
				index = 4;
			}
			else if(backHalf)
			{
				index = 5;
			}
		}
		else if(leftHalf)
		{
			if(frontHalf)
			{
				index = 6;
			}
			else if(backHalf)
			{
				index = 7;
			}
		}
	}

	return index;
}

// getNode
// Method returns the Octree pointer given an index
// 0: UpRightFront
// 1: UpRightBack
// 2: UpLeftFront
// 3: UpLeftBack
// 4: DownRightFront
// 5: DownRightBack
// 6: DownLeftFront
// 7: DownLeftBack
// returns nullptr if index is -1;
CollisionDetector::Octree* CollisionDetector::Octree::getNode(int index)
{
	//TODO//This should have a default case to clear out that compiler warning
	switch(index)
	{
	case -1:
		return nullptr;
		break;
	case 0:
		return UpRightFront;
		break;
	case 1:
		return UpRightBack;
		break;
	case 2:
		return UpLeftFront;
		break;
	case 3:
		return UpLeftBack;
		break;
	case 4:
		return DownRightFront;
		break;
	case 5:
		return DownRightBack;
		break;
	case 6:
		return DownLeftFront;
		break;
	case 7:
		return DownLeftBack;
		break;
	}
}

CollisionDetector::Octree* CollisionDetector::Octree::getNode(CollisionDetector& c)
{
	return getNode(getIndex(c));
}

#pragma endregion

#pragma endregion