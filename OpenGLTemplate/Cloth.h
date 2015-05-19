#pragma once
#include "Spring.h"
#include "CommandTypes.h"
class Cloth :
	public GameObject
{
public:
	Cloth(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~Cloth();

	void draw() override;
	void update(float dt) override;

	Command* getPullCommand();
private:
	std::vector<GameObject*> ClothNodes;
	std::vector<Spring*> StructuralSprings;
	std::vector<Spring*> ShearSprings;
	std::vector<Spring*> BendSprings;
	int width;
	int height;

	void createCloth(int width, int height);
	void populateNodes(int width, int height);
	void addNode(glm::vec3 position, bool immovable);
	void populateStructuralSprings(int width, int height);
	void populateShearSprings(int width, int height);
	void populateBendSprings(int width, int height);
	void addSpring(GameObject& node1, GameObject& node2, std::vector<Spring*>& springList);
};

