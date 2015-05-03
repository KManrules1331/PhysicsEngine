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
		checkNodes();
		prevPosition = transform->getPosition();
	}
}

void ClothNode::connectNode(ClothNode* otherNode)
{
	nodes.push_back(otherNode);
	otherNode->nodes.push_back(this);
	ClothStrand* strand = new ClothStrand(glm::length(
		transform->getPosition() -
		otherNode->transform->getPosition()
		));
	strands.push_back(strand);
	otherNode->strands.push_back(strand);
}

//Breadth first search of nodes centered around one method is called
//Method will check surrounding nodes and pull nodes if necessary
//If a node is pulled, that node will check it's surrounding nodes as well
void ClothNode::checkNodes()
{
	std::queue<ClothNode*> uncheckedNodes;
	uncheckedNodes.push(this);
	while (!uncheckedNodes.empty())
	{
		ClothNode* node = uncheckedNodes.front();
		uncheckedNodes.pop();
		for (int i = 0; i < node->nodes.size(); i++)
		{
			if (node->strands[i]->pull(*(node->physicsComponent), *(node->nodes[i]->physicsComponent)))
			{
				uncheckedNodes.push(node->nodes[i]);
			}
		}
	}
}