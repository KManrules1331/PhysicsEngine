#include "Cloth.h"


Cloth::Cloth(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int width, int height) : GameObject(position, rotation, scale)
{
	this->width = width;
	this->height = height;
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

//Testing Method
Command* Cloth::pull()
{
	if (Nodes.size() % 2 == 1)
		return new MoveCommand(glm::vec3(0.0f, 0.0f, 0.5f), *(Nodes[width * height / 2]));
	else
		return new MoveCommand(glm::vec3(0.0f, 0.0f, 0.5f), *(Nodes[width * (height - 1) / 2]));
}

Command* Cloth::pullCorners(glm::vec3 displacement)
{
	Command* a = new MoveCommand(displacement, *(Nodes[0]));
	Command* b = new MoveCommand(displacement, *(Nodes[width - 1]));
	Command* c = new MoveCommand(displacement, *(Nodes[height * (width - 1) - 1]));
	Command* d = new MoveCommand(displacement, *(Nodes[height * width - 1]));
	MultiCommand* multiCommand = new MultiCommand();
	multiCommand->addCommand(a);
	multiCommand->addCommand(b);
	multiCommand->addCommand(c);
	multiCommand->addCommand(d);
	return multiCommand;
}