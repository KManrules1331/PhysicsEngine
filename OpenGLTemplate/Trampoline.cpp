#include "Trampoline.h"


Trampoline::Trampoline(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject(position, rotation, scale)
{
	width = 10;
	height = 10;
	populateNodes(width, height);
	populateSprings(width, height);
}


Trampoline::~Trampoline()
{
	Springs.clear();
	Nodes.clear();
}

void Trampoline::update(float dt)
{
	for (int i = 0; i < Springs.size(); i++)
	{
		Springs[i]->update(dt);
	}
	for (int i = 0; i < Nodes.size(); i++)
	{
		Nodes[i]->update(dt);
	}
}

void Trampoline::draw()
{
	for (int i = 0; i < Springs.size(); i++)
	{
		Springs[i]->draw();
	}
	for (int i = 0; i < Nodes.size(); i++)
	{
		Nodes[i]->draw();
	}
}

void Trampoline::populateSprings(int width, int height)
{
	for (int i = 0; i < width - 1; i++)
	{
		for (int j = 0; j < height - 1; j++)
		{
			addSpring(j * width + i, j * width + i + 1);	// Horizontal
			addSpring(j * width + i, (j + 1) * width + i);	// Vertical
		}
	}
	for (int i = 0; i < width - 1; i++) //Bottom row
	{
		addSpring((height - 1) * width + i, (height - 1) * width + i + 1);
	}
	for (int j = 0; j < height - 1; j++) //Right Column
	{
		addSpring((j + 1) * width - 1, (j + 2) * width - 1);
	}
}

void Trampoline::addSpring(int nodeIndex1, int nodeIndex2)
{
	PhysicsComponent& a = *(Nodes[nodeIndex1]->physicsComponent);
	PhysicsComponent& b = *(Nodes[nodeIndex2]->physicsComponent);
	Spring* spring = new Spring(a, b, glm::vec3(0.0f), glm::vec3(0.0f));
	spring->setMesh(Mesh::cubeMesh);
	Springs.push_back(spring);
}

void Trampoline::populateNodes(int width, int height)
{
	float distanceBetweenNodes = 0.5f;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			//X = -(dBM * width / 2.0f) + (i * dBM)
			//Y = (dBM * height / 2.0f) - (j * dBM)
			float X = -(distanceBetweenNodes * (width - 1) / 2.0f) + (i * distanceBetweenNodes);
			float Y = (distanceBetweenNodes * (height - 1) / 2.0f) - (j * distanceBetweenNodes);
			if (i == 0 || i == width - 1 || j == 0 || j == height - 1)
			{
				addNode(glm::vec3(X, Y, 0.0f), true);
			}
			else
			{
				addNode(glm::vec3(X, Y, 0.0f), false);
			}
		}
	}
}

void Trampoline::addNode(glm::vec3 position, bool immovable)
{
	GameObject* node = new GameObject(glm::vec3(transform->transformMatrix * glm::vec4(position, 1.0f)), glm::vec3(0.0f), glm::vec3(0.1f));
	node->setMesh(Mesh::sphereMesh);
	node->addCollisionDetector(CollisionDetector::DetectorType::Sphere);
	if (immovable)
	{
		node->addPhysicsComponent(0.0f, 0.0f);
	}
	else
	{
		node->addPhysicsComponent(1.0f, 1.0f);
	}
	Nodes.push_back(node);
}

Command* Trampoline::getPullCommand()
{
	if (Nodes.size() % 2 == 1)
		return new MoveCommand(glm::vec3(0.0f, 0.0f, -0.5f), *(Nodes[width * height / 2]));
	else
		return new MoveCommand(glm::vec3(0.0f, 0.0f, -0.5f), *(Nodes[width * (height - 1) / 2]));
}