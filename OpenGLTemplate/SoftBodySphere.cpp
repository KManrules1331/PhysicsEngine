#include "SoftBodySphere.h"


SoftBodySphere::SoftBodySphere(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Cloth(position, rotation, scale)
{
	width = 5;
	height = 5;
	depth = 5;
	radius = scale.x * 0.5f;
	makeCube();
	
	std::vector<ClothList*> list;
	populateStructuralSprings(list);
}


SoftBodySphere::~SoftBodySphere()
{
}

void SoftBodySphere::pushNode(Transform* node)
{
	glm::vec3 position = transform->getPosition();
	glm::vec3 nodePosition = node->getPosition() - position;
	float length = glm::length(nodePosition);
	if (length < radius)
	{
		node->setPosition(glm::normalize(nodePosition) * radius + position);
	}
}

void SoftBodySphere::makeCube()
{
	std::vector<ClothList> list;
	float halfSideLength = 0.5f * (1.0f / glm::sqrt(3.0f));
	glm::vec3 TopLeftBack = glm::vec3(-halfSideLength, halfSideLength, halfSideLength);

	float dX = (2 * halfSideLength) / (width - 1);
	float dY = (2 * halfSideLength) / (height - 1);
	float dZ = (2 * halfSideLength) / (depth - 1);

	for (int k = 0; k < depth; k++)
	{
		for (int i = 0; i < width; i++)
		{
			glm::vec3 pos = TopLeftBack + glm::vec3(dX * i, 0.0f, -dZ * k);
			ClothList listItem;
			listItem.nodeIndex = ClothNodes.size();
			if (i == width - 1)
			{
				if (k == 0)
				{
					listItem.rightIndex = (width * depth) + (width * (height - 1)) + (width * (depth - 1)) + (width * (height - 2));
				}
				else if (k == depth - 1)
				{
					listItem.rightIndex = list.size() - depth;
				}
				else
				{
					int offset = (width * depth) + (width * (height - 1)) + (width * (depth - 1)) + (width * (height - 2)) + (depth - 2) * (height - 2);
					listItem.rightIndex = k - 1 + offset;
				}
			}
			else
			{
				listItem.rightIndex = list.size() + 1;
			}
			listItem.downIndex = list.size() + width;
			list.push_back(listItem);
			addNode(pos, false);
		}
	}
	glm::vec3 TopLeftFront = glm::vec3(-halfSideLength, halfSideLength, -halfSideLength);
	for (int j = 1; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			glm::vec3 pos = TopLeftFront + glm::vec3(dX * i, -dY * j, 0.0f);
			ClothList listItem;
			listItem.nodeIndex = ClothNodes.size();
			if (i == width - 1)
			{
				if (j == height - 1)
				{
					listItem.rightIndex = list.size() - height;
				}
				else
				{
					int offset = (width * depth) + (width * (height - 1)) + (width * (depth - 1)) + (width * (height - 2)) + (depth - 2) * (height - 2);
					listItem.rightIndex = j * (depth - 2) + offset;
				}
			}
			addNode(pos, false);
		}
	}

	glm::vec3 BottomLeftFront = glm::vec3(-halfSideLength, -halfSideLength, -halfSideLength);
	for (int k = 1; k < depth; k++)
	{
		for (int i = 0; i < width; i++)
		{
			glm::vec3 pos = BottomLeftFront + glm::vec3(dX * i, 0.0f, dZ * k);
			addNode(pos, false);
		}
	}

	glm::vec3 BottomLeftBack = glm::vec3(-halfSideLength, -halfSideLength, halfSideLength);
	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 0; i < width; i++)
		{
			glm::vec3 pos = BottomLeftBack + glm::vec3(dX * i, dY * j, 0.0f);
			addNode(pos, false);
		}
	}

	for (int j = 1; j < height - 1; j++)
	{
		for (int k = 1; k < depth - 1; k++)
		{
			glm::vec3 pos = TopLeftBack + glm::vec3(0.0f, -dY * j, -dZ * k);
			addNode(pos, false);
		}
	}

	glm::vec3 TopRightBack = glm::vec3(halfSideLength, halfSideLength, halfSideLength);
	for (int j = 1; j < height - 1; j++)
	{
		for (int k = 1; k < depth - 1; k++)
		{
			glm::vec3 pos = TopRightBack + glm::vec3(0.0f, -dY * j, -dZ * k);
			addNode(pos, false);
		}
	}

	/*std::vector<ClothList*> nodes;
	int index = 0;
	//TopFace
	glm::vec3 TRBCorner = glm::vec3(halfSideLength, halfSideLength, halfSideLength);
	for (int i = 0; i < width; i++)
	{
		for (int k = 0; k < depth; k++)
		{
			glm::vec3 pos = TRBCorner - glm::vec3(dX * i, 0.0f, dZ * k);
			nodes.push_back(new ClothList{ nullptr, nullptr, new GameObject(pos, glm::vec3(0.0f), glm::vec3(0.1f)) });
			index++;
			//addNode(pos, false);
		}
	}
	for (int i = 0; i < index; i++)
	{
		if (i >= depth)
			nodes[i]->right = nodes[i - depth];
		if ((i + 1) % depth != 0)
			nodes[i]->down = nodes[i + 1];
	}

	//BottomFace
	glm::vec3 BRBCorner = glm::vec3(halfSideLength, -halfSideLength, halfSideLength);
	for (int i = 0; i < width; i++)
	{
		for (int k = 0; k < depth; k++)
		{
			glm::vec3 pos = BRBCorner - glm::vec3(dX * i, 0.0f, dZ * k);
			nodes.push_back(new ClothList{ nullptr, nullptr, new GameObject(pos, glm::vec3(0.0f), glm::vec3(0.1f)) });
			index++;
			//addNode(pos, false);
		}
	}
	for (int i = width * depth; i < index; i++)
	{
		if (i >= depth + width * depth)
			nodes[i]->right = nodes[i - depth];
		if ((i + 1) % depth != 0)
			nodes[i]->down = nodes[i + 1];
	}

	int frontOffset = index;
	//FrontFace
	glm::vec3 TRFCorner = glm::vec3(halfSideLength, halfSideLength, -halfSideLength);
	for (int i = 0; i < width; i++)
	{
		for (int j = 1; j < height - 1; j++)
		{
			glm::vec3 pos = TRFCorner - glm::vec3(dX * i, dY * j, 0.0f);
			nodes.push_back(new ClothList{ nullptr, nullptr, new GameObject(pos, glm::vec3(0.0f), glm::vec3(0.1f)) });
			index++;
			//addNode(pos, false);
		}
	}
	
	for (int i = 0; i < index - frontOffset; i++)
	{
		if (i > height - 3)
			nodes[i + frontOffset]->right = nodes[i + frontOffset - height - 2];
		if ((i + 1) % (height - 2) != 0)
			nodes[i + frontOffset]->down = nodes[i + frontOffset + 1];
	}

	//BackFace
	int backOffset = index;
	for (int i = 0; i < width; i++)
	{
		for (int j = 1; j < height - 1; j++)
		{
			glm::vec3 pos = TRBCorner - glm::vec3(dX * i, dY * j, 0.0f);
			nodes.push_back(new ClothList{ nullptr, nullptr, new GameObject(pos, glm::vec3(0.0f), glm::vec3(0.1f)) });
			index++;
			//addNode(pos, false);
		}
	}
	for (int i = 0; i < index - backOffset; i++)
	{
		if (i >= depth - 2)
			nodes[i + backOffset]->right = nodes[i + backOffset - depth - 2];
		if ((i + 1) % (depth - 2) != 0)
			nodes[i + backOffset]->down = nodes[i + backOffset - 1];
	}

	//RightFace
	int rightOffset = index;
	for (int j = 1; j < height - 1; j++)
	{
		for (int k = 1; k < depth - 1; k++)
		{
			glm::vec3 pos = TRBCorner - glm::vec3(0.0f, dY * j, dZ * k);
			nodes.push_back(new ClothList{ nullptr, nullptr, new GameObject(pos, glm::vec3(0.0f), glm::vec3(0.1f)) });
			index++;
			//addNode(pos, false);
		}
	}
	for (int i = 0; i < index - rightOffset; i++)
	{
		if ((i + 1) % (depth - 2) != 0)
			nodes[i + rightOffset]->down = nodes[i + rightOffset + 1];
		if (i < index - rightOffset - depth + 1)
			nodes[i + rightOffset]->right = nodes[i + rightOffset + depth - 2];
	}

	//LeftFace
	int leftOffset = index;
	glm::vec3 TLBCorner = glm::vec3(-halfSideLength, halfSideLength, halfSideLength);
	for (int j = 1; j < height - 1; j++)
	{
		for (int k = 1; k < depth - 1; k++)
		{
			glm::vec3 pos = TLBCorner - glm::vec3(0.0f, dY * j, dZ * k);
			nodes.push_back(new ClothList{ nullptr, nullptr, new GameObject(pos, glm::vec3(0.0f), glm::vec3(0.1f)) });
			index++;
			//addNode(pos, false);
		}
	}
	for (int i = 0; i < index - leftOffset; i++)
	{
		if ((i + 1) % (depth - 2) != 0)
			nodes[i + leftOffset]->down = nodes[i + leftOffset + 1];
		if (i >= depth - 2)
			nodes[i + leftOffset]->right = nodes[i + leftOffset - depth + 2];
	}



	for (int i = 0; i < nodes.size(); i++)
	{
		pushNode(nodes[i]->node->transform);
		addNode(nodes[i]->node, false);
	}
	for (int i = 0; i < nodes.size(); i++)
	{
		makeAllSprings(nodes[i]);
	}*/
}

void SoftBodySphere::makeAllSprings(ClothList* a)
{
	if (a->down != nullptr && a->down->node)
		addSpring(*(a->node), *(a->down->node), StructuralSprings);
	if (a->right != nullptr && a->right->node)
		addSpring(*(a->node), *(a->right->node), StructuralSprings);
}

void SoftBodySphere::populateStructuralSprings(std::vector<ClothList*>& list)
{
	//Radial
	ClothList* node = new ClothList();
	node->node = ClothNodes[0];
	for (int r = 0; r < 2 * height + 2 * depth - 1; r++)
	{
		for (int i = 0; i < width - 1; i++)
		{
			
		}
	}

	/*
	//Top
	for (int i = 0; i < width - 1; i++)
	{
		for (int k = 0; k < depth - 1; k++)
		{
			addSpring(*(ClothNodes[i * depth + k]), *(ClothNodes[i * depth + k + 1]), StructuralSprings);
			addSpring(*(ClothNodes[i * depth + k]), *(ClothNodes[i * depth + k + depth]), StructuralSprings);
		}
		addSpring(*(ClothNodes[i * depth + depth - 1]), *(ClothNodes[i * depth + depth + depth - 1]), StructuralSprings);
	}
	for (int k = 0; k < depth - 1; k++)
	{
		addSpring(*(ClothNodes[(width - 1) * depth + k]), *(ClothNodes[(width - 1) * depth + k + 1]), StructuralSprings);
	}

	//Bottom
	int offset = width * depth;
	for (int i = 0; i < width - 1; i++)
	{
		for (int k = 0; k < depth - 1; k++)
		{
			addSpring(*(ClothNodes[i * depth + k + offset]), *(ClothNodes[i * depth + k + 1 + offset]), StructuralSprings);
			addSpring(*(ClothNodes[i * depth + k + offset]), *(ClothNodes[i * depth + k + depth + offset]), StructuralSprings);
		}
		addSpring(*(ClothNodes[i * depth + depth - 1 + offset]), *(ClothNodes[i * depth + depth + depth - 1 + offset]), StructuralSprings);
	}
	for (int k = 0; k < depth - 1; k++)
	{
		addSpring(*(ClothNodes[(width - 1) * depth + k + offset]), *(ClothNodes[(width - 1) * depth + k + 1 + offset]), StructuralSprings);
	}

	//Front
	offset *= 2;
	for (int i = 0; i < width - 1; i++)
	{
		for (int j = 0; j < height - 3; j++)
		{
			addSpring(*(ClothNodes[i * (height - 2) + j + offset]), *(ClothNodes[i * (height-2) + j + 1 + offset]), StructuralSprings);
			addSpring(*(ClothNodes[i * (height - 2) + j + offset]), *(ClothNodes[i * (height-2) + j + height - 2 + offset]), StructuralSprings);
		}
		addSpring(*(ClothNodes[i * (height - 2) + (height - 2) - 1 + offset]), *(ClothNodes[i * (height - 2) + height - 2 + height - 2 - 1 + offset]), StructuralSprings);
	}
	for (int j = 0; j < height - 3; j++)
	{
		addSpring(*(ClothNodes[(width - 1) * (height - 2) + j + offset]), *(ClothNodes[(width - 1) * (height - 2) + j + 1 + offset]), StructuralSprings);
	}

	//Top to Front connection
	for (int i = 0; i < width; i++)
	{
		addSpring(*(ClothNodes[i * (height - 2) + offset]), *(ClothNodes[i * depth + depth - 1]), StructuralSprings);
	}

	//Bottom to Front connection
	for (int i = 0; i < width; i++)
	{
		int backoffset = width * depth;
		addSpring(*(ClothNodes[i * (height - 2) + height - 3 + offset]), *(ClothNodes[i * depth + depth - 1 + backoffset]), StructuralSprings);
	}

	//Back
	offset += width * (height - 2);
	for (int i = 0; i < width - 1; i++)
	{
		for (int j = 0; j < height - 3; j++)
		{
			addSpring(*(ClothNodes[i * (height - 2) + j + offset]), *(ClothNodes[i * (height - 2) + j + 1 + offset]), StructuralSprings);
			addSpring(*(ClothNodes[i * (height - 2) + j + offset]), *(ClothNodes[i * (height - 2) + j + height - 2 + offset]), StructuralSprings);
		}
		addSpring(*(ClothNodes[i * (height - 2) + (height - 2) - 1 + offset]), *(ClothNodes[i * (height - 2) + height - 2 + height - 2 - 1 + offset]), StructuralSprings);
	}
	for (int j = 0; j < height - 3; j++)
	{
		addSpring(*(ClothNodes[(width - 1) * (height - 2) + j + offset]), *(ClothNodes[(width - 1) * (height - 2) + j + 1 + offset]), StructuralSprings);
	}

	//Top to Back connection
	for (int i = 0; i < width; i++)
	{
		addSpring(*(ClothNodes[i * (height - 2) + offset]), *(ClothNodes[i * depth]), StructuralSprings);
	}

	//Bottom to Back connection
	for (int i = 0; i < width; i++)
	{
		int backoffset = width * depth;
		addSpring(*(ClothNodes[i * (height - 2) + height - 3 + offset]), *(ClothNodes[i * depth + backoffset]), StructuralSprings);
	}

	//Right
	offset += width * (height - 2);
	for (int j = 0; j < height - 3; j++)
	{
		for (int k = 0; k < depth - 3; k++)
		{
			addSpring(*(ClothNodes[j * (depth - 2) + k + offset]), *(ClothNodes[j * (depth - 2) + k + 1 + offset]), StructuralSprings);
			addSpring(*(ClothNodes[j * (depth - 2) + k + offset]), *(ClothNodes[j * (depth - 2) + k + depth - 2 + offset]), StructuralSprings);
		}
		addSpring(*(ClothNodes[j * (depth - 2) + (depth - 2) - 1 + offset]), *(ClothNodes[j * (depth - 2) + depth - 2 + depth - 2 - 1 + offset]), StructuralSprings);
	}
	for (int k = 0; k < depth - 3; k++)
	{
		addSpring(*(ClothNodes[(height - 3) * (depth - 2) + k + offset]), *(ClothNodes[(height - 3) * (depth - 2) + k + 1 + offset]), StructuralSprings);
	}

	//Right to Top Connection
	for (int k = 0; k < depth - 2; k++)
	{
		addSpring(*(ClothNodes[offset + k]), *(ClothNodes[k + 1]), StructuralSprings);
	}

	//Right to Bottom connection
	for (int k = 0; k < depth - 2; k++)
	{
		int backoffset = width* height;
		addSpring(*(ClothNodes[offset + (depth - 2) * (height - 3) + k]), *(ClothNodes[backoffset + k + 1]), StructuralSprings);
	}

	//Right to Front connection
	for (int j = 0; j < height - 2; j++)
	{
		int frontOffset = 2 * width * height;
		addSpring(*(ClothNodes[offset + j * (depth - 2) + (depth - 3)]), *(ClothNodes[frontOffset + j]), StructuralSprings);
	}

	//Right to Back connection
	for (int j = 0; j < height - 2; j++)
	{
		int backOffset = 2 * width * height + width * (height - 2);
		addSpring(*(ClothNodes[offset + j * (depth - 2)]), *(ClothNodes[backOffset + j]), StructuralSprings);
	}

	//Left
	offset += (height - 2) * (depth - 2);
	for (int j = 0; j < height - 3; j++)
	{
		for (int k = 0; k < depth - 3; k++)
		{
			addSpring(*(ClothNodes[j * (depth - 2) + k + offset]), *(ClothNodes[j * (depth - 2) + k + 1 + offset]), StructuralSprings);
			addSpring(*(ClothNodes[j * (depth - 2) + k + offset]), *(ClothNodes[j * (depth - 2) + k + depth - 2 + offset]), StructuralSprings);
		}
		addSpring(*(ClothNodes[j * (depth - 2) + (depth - 2) - 1 + offset]), *(ClothNodes[j * (depth - 2) + depth - 2 + depth - 2 - 1 + offset]), StructuralSprings);
	}
	for (int k = 0; k < depth - 3; k++)
	{
		addSpring(*(ClothNodes[(height - 3) * (depth - 2) + k + offset]), *(ClothNodes[(height - 3) * (depth - 2) + k + 1 + offset]), StructuralSprings);
	}

	//Left to Top Connection
	for (int k = 0; k < depth - 2; k++)
	{
		addSpring(*(ClothNodes[offset + k]), *(ClothNodes[k + 1 + ((width - 1) * depth)]), StructuralSprings);
	}

	//Left to Bottom connection
	for (int k = 0; k < depth - 2; k++)
	{
		int backoffset = width* height;
		addSpring(*(ClothNodes[offset + (depth - 2) * (height - 3) + k]), *(ClothNodes[backoffset + k + 1 + ((width - 1) * depth)]), StructuralSprings);
	}

	//Left to Front connection
	for (int j = 0; j < height - 2; j++)
	{
		int frontOffset = 2 * width * height;
		addSpring(*(ClothNodes[offset + j * (depth - 2) + (depth - 3)]), *(ClothNodes[frontOffset + j + ((width - 1) * (height - 2))]), StructuralSprings);
	}

	//Left to Back connection
	for (int j = 0; j < height - 2; j++)
	{
		int backOffset = 2 * width * height + width * (height - 2);
		addSpring(*(ClothNodes[offset + j * (depth - 2)]), *(ClothNodes[backOffset + j + ((width - 1) * (height - 2))]), StructuralSprings);
	}*/
}

SoftBodySphere::ClothList::~ClothList()
{
}