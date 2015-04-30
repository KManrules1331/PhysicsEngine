#pragma once
#include "GameObject.h"
#include "Spring.h"

class Trampoline :
	public GameObject
{
public:
	Trampoline(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~Trampoline();

	void update(float dt) override;
	void draw() override;

private:
	int width;
	int height;
	std::vector<GameObject*> Springs;
	std::vector<GameObject*> Nodes;

	void populateNodes(int width, int height);
	void populateSprings(int width, int height);

	void addNode(glm::vec3 position, bool immovable);
	void addSpring(int nodeIndex1, int nodeIndex2);
};

