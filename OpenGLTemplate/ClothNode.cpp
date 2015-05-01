#include "ClothNode.h"


ClothNode::ClothNode(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: GameObject(position, rotation, scale)
{
}


ClothNode::~ClothNode()
{
}

void ClothNode::update(float dt)
{
	physicsComponent->update(dt);
	if (prevPosition != transform->getPosition())
	{
		checked = false;
		checkNodes();
		prevPosition = transform->getPosition();
	}
}

void ClothNode::connectNode(ClothNode* otherNode)
{
	nodes.push_back(otherNode);
	strands.push_back(new ClothStrand(glm::length(
		transform->getPosition() - 
		otherNode->transform->getPosition()
		)));
}

//Currently depth-first search, should change to breadth-first search
void ClothNode::checkNodes()
{
	if (!checked)
	{
		for (int i = 0; i < strands.size(); i++)
		{
			if (strands[i]->pull(*physicsComponent, *(nodes[i]->physicsComponent)))
			{
				nodes[i]->checked = false;
			}
		}
		checked = true;
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->checkNodes();
		}
	}
}