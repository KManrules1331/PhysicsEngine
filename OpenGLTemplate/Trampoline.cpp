#include "Trampoline.h"


Trampoline::Trampoline(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int width, int height) : GameObject(position, rotation, scale)
{
	this->width = width;
	this->height = height;
	cloth = new Cloth(position, rotation, scale * (float(width - 2) / float(width)), width - 2, height - 2);
	populateNodes(width, height);
	populateSprings(width, height);
}


Trampoline::~Trampoline()
{
	Springs.clear();
	Nodes.clear();
	delete cloth;
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
	cloth->update(dt);
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
	cloth->draw();
}

void Trampoline::populateSprings(int width, int height)
{
	for (int i = 1; i < width - 1; i++)
	{
		//Bottom row to Bottom row cloth
		addSpring(2 * i, i - 1);

		//Top row to Top row cloth
		int index = (width - 2) *(height - 3) + (i - 1);
		addSpring(2 * i + 1, index);
	}
	for (int i = 1; i < height - 1; i++)
	{
		//Left row to Left row cloth
		int trampIndex = 2 * width + (i - 1) * 2;
		int clothIndex = (i - 1) * (width - 2);
		addSpring(trampIndex, clothIndex);

		//Right row to right row cloth
		trampIndex = 2 * width + (i - 1) * 2 + 1;
		clothIndex = (i - 1) * (width - 2) + (width - 3);
		addSpring(trampIndex, clothIndex);
	}
}

void Trampoline::addSpring(int nodeIndex1, int nodeIndex2)
{
	PhysicsComponent& a = *(Nodes[nodeIndex1]->physicsComponent);
	PhysicsComponent& b = *(cloth->ClothNodes[nodeIndex2]->physicsComponent);
	Spring* spring = new Spring(a, b, glm::vec3(0.0f), glm::vec3(0.0f), 10.0f);
	spring->setMesh(Mesh::cubeMesh);
	spring->setColor(192, 192, 192, 255);
	Springs.push_back(spring);
}

void Trampoline::populateNodes(int width, int height)
{
	float dX = 1.0f / (width - 1);
	float dY = 1.0f / (height - 1);
	for (int i = 0; i < width; i++)
	{
		glm::vec3 pos = glm::vec3(-0.5f + dX * i, -0.5f, 0.0f);
		addNode(pos, true);

		pos = glm::vec3(-0.5f + dX * i, 0.5f, 0.0f);
		addNode(pos, true);
	}
	for (int i = 1; i < height - 1; i++)
	{
		glm::vec3 pos = glm::vec3(-0.5f, -0.5f + dY * i, 0.0f);
		addNode(pos, true);
		
		pos = glm::vec3(0.5f, -0.5f + dY * i, 0.0f);
		addNode(pos, true);
	}
}

void Trampoline::addNode(glm::vec3 position, bool immovable)
{
	GameObject* node = new GameObject(glm::vec3(transform->transformMatrix * glm::vec4(position, 1.0f)), glm::vec3(0.0f), glm::vec3(0.1f));
	node->setMesh(Mesh::sphereMesh);
	node->setColor(192, 192, 192, 255);
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
		return new MoveCommand(glm::vec3(0.0f, 0.0f, -5.5f), *(Nodes[width * height / 2]));
	else
		return new MoveCommand(glm::vec3(0.0f, 0.0f, -5.5f), *(Nodes[width * (height - 1) / 2]));
}