#pragma once
#include "GameObject.h"
#include "Spring.h"
class Web :
	public GameObject
{
public:
	Web(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~Web();

	void update() override;
	void draw() override;

	void twinge();

private:
	int layers;
	int radials;
	std::vector<GameObject*> Springs;
	std::vector<GameObject*> Nodes;

	void populateNodes(int layers, int radials);
	void populateSprings(int layers, int radials);

	void addNode(glm::vec3 position, bool immovable);
	void addSpring(int nodeIndex1, int nodeIndex2);
};

