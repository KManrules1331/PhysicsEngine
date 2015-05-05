#pragma once
#include "GameObject.h"
#include "ClothNode.h"
#include "CommandTypes.h"
class Cloth :
	public GameObject
{
public:
	Cloth(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int width = 5, int height = 5);
	~Cloth();

	void update(float dt) override;
	void draw() override;

	//Testing method
	Command* pull();
	Command* pullCorners(glm::vec3 displacement);
private:
	std::vector<ClothNode*> Nodes;
	int width;
	int height;

	void populateNodes(int width, int height);
	void populateStrands(int width, int height);
	void addNode(glm::vec3 position);
	void addStrand(ClothNode* nodeA, ClothNode* nodeB);
};

