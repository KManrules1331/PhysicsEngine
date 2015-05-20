#pragma once
#include "GameObject.h"
#include "Spring.h"
#include "CommandTypes.h"
#include "Cloth.h"

class Trampoline :
	public GameObject
{
public:
	Trampoline(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int width, int height);
	~Trampoline();

	void update(float dt) override;
	void draw() override;

	Command* getPullCommand();

private:
	int width;
	int height;
	std::vector<GameObject*> Springs;
	std::vector<GameObject*> Nodes;
	Cloth* cloth;

	void populateNodes(int width, int height);
	void populateSprings(int width, int height);

	void addNode(glm::vec3 position, bool immovable);
	void addSpring(int nodeIndex1, int clothNodeIndex);
};

