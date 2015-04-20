#include "Web.h"


Web::Web(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject(position, rotation, scale)
{
	layers = 3;
	radials = 10;
	populateNodes(layers, radials);
	populateSprings(layers, radials);
}


Web::~Web()
{
	Springs.clear();
	Nodes.clear();
}


void Web::update(float dt)
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

void Web::draw()
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

void Web::populateSprings(int layers, int radials)
{
	for (int j = 0; j < radials; j++)
	{
		addSpring(0, j + 1);
	}
	for (int i = 0; i < (layers - 1); i++)
	{
		for (int j = 1; j < (radials); j++)
		{
			addSpring(i * radials + j, i * radials + j + 1);
			addSpring(i * radials + j, (i + 1) * radials + j);
		}
		addSpring((i + 1) * radials, i * radials + 1);
		addSpring((i + 1) * radials, (i + 2) * radials);
	}
	for (int j = 1; j < (radials); j++)
	{
		addSpring((layers - 1) * radials + j, (layers - 1) * radials + j + 1);
	}
	addSpring((layers - 1) * radials + 1, (layers - 1) * radials + radials);
}

void Web::addSpring(int nodeIndex1, int nodeIndex2)
{
	PhysicsComponent& a = *(Nodes[nodeIndex1]->physicsComponent);
	PhysicsComponent& b = *(Nodes[nodeIndex2]->physicsComponent);
	Spring* spring = new Spring(a, b, glm::vec3(0.0f), glm::vec3(0.0f));
	spring->setMesh(Mesh::cubeMesh);
	Springs.push_back(spring);
}

void Web::populateNodes(int layers, int radials)
{
	addNode(glm::vec3(0.0f), false);
	float radius = 1.0f;
	for (int i = 0; i < layers - 1; i++)
	{
		for (int j = 0; j < radials; j++)
		{
			float angle = PI * 2 / radials * j;
			addNode(glm::vec3(cos(angle) * radius, sin(angle) * radius, 0.0f), false);
		}
		radius += 1.0f;
	}
	for (int j = 0; j < radials; j++)
	{
		float angle = PI * 2 / radials * j;
		addNode(glm::vec3(cos(angle) * radius, sin(angle) * radius, 0.0f), true);
	}
}

void Web::addNode(glm::vec3 position, bool immovable)
{
	GameObject* node = new GameObject(glm::vec3(transform->transformMatrix * glm::vec4(position, 1.0f)), glm::vec3(0.0f), glm::vec3(0.1f));
	node->setMesh(Mesh::sphereMesh);
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

void Web::twinge()
{
	int nodeTwinge = rand() % ((layers - 1) * radials) + 1;
	Nodes[nodeTwinge]->transform->move(glm::vec3(0.0f, -0.5f, 0.0f));
}