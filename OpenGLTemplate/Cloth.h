#pragma once
#include "GameObject.h"
#include "ClothNode.h"
class Cloth :
	public GameObject
{
public:
	Cloth(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~Cloth();

	void update(float dt) override;
	void draw() override;
private:
	std::vector<ClothNode*> Nodes;

	void populateNodes(int width, int height);
	void populateStrands(int width, int height);
	void addNode(glm::vec3 position);
	void addStrand(ClothNode* nodeA, ClothNode* nodeB);
};

