#include "Cloth.h"


Cloth::Cloth(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject(position, rotation, scale)
{
	float width = 5;
	float height = 5;
	populateNodes(width, height);
	populateStrands(width, height);
}


Cloth::~Cloth()
{
	Nodes.clear();
}

void Cloth::update(float dt)
{
	for (int i = 0; i < Nodes.size(); i++)
	{
		Nodes[i]->update(dt);
	}
}

void Cloth::draw()
{
	for (int i = 0; i < Nodes.size(); i++)
	{
		Nodes[i]->draw();
	}
}

void Cloth::populateNodes(int width, int height)
{
	float distanceBetweenNodes = 0.5f;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			float X = -(distanceBetweenNodes * (width - 1) / 2.0f) + (i * distanceBetweenNodes);
			float Y = (distanceBetweenNodes * (height - 1) / 2.0f) - (j * distanceBetweenNodes);
			addNode(glm::vec3(X, Y, 0.0f));
		}
	}
}

void Cloth::addNode(glm::vec3 position)
{
	ClothNode* node = new ClothNode(glm::vec3(transform->transformMatrix * glm::vec4(position, 1.0f)), glm::vec3(0.0f), glm::vec3(0.1f));
	node->setMesh(Mesh::sphereMesh);
	node->addCollisionDetector(CollisionDetector::DetectorType::Sphere);
	node->addPhysicsComponent(1.0f, 1.0f);
	Nodes.push_back(node);
}

void Cloth::populateStrands(int width, int height)
{
	for (int i = 0; i < width - 1; i++)
	{
		for (int j = 0; j < height - 1; j++)
		{
			addStrand(Nodes[j * width + i], Nodes[j * width + i + 1]);
			addStrand(Nodes[j * width + i], Nodes[(j + 1) * width + i]);
		}
	}
	for (int i = 0; i < width - 1; i++)
	{
		addStrand(Nodes[(height - 1) * width + i], Nodes[(height - 1) * width + i + 1]);
	}
	for (int j = 0; j < height - 1; j++)
	{
		addStrand(Nodes[(j + 1) * width - 1], Nodes[(j + 2) * width - 1]);
	}
}

void Cloth::addStrand(ClothNode* nodeA, ClothNode* nodeB)
{
	nodeA->connectNode(nodeB);
}