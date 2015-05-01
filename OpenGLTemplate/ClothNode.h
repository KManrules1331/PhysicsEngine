#pragma once
#include "GameObject.h"
#include "ClothStrand.h"
class ClothNode :
	public GameObject
{
public:
	ClothNode(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~ClothNode();

	void update(float dt) override;
	void connectNode(ClothNode* otherNode);
private:
	std::vector<ClothStrand*> strands;
	std::vector<ClothNode*> nodes;
	glm::vec3 prevPosition;
	bool checked;

	void checkNodes();
};

