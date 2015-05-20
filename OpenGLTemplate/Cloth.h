#pragma once
#include "Spring.h"
#include "CommandTypes.h"
class Cloth :
	public GameObject
{
public:
	//Attributes
	std::vector<GameObject*> ClothNodes;

	//Constructors/Destructors
	Cloth(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~Cloth();

	//Methods
	void draw() override;
	void update(float dt) override;
	Command* getPullCommand();

private:
	//Attributes
	std::vector<Spring*> StructuralSprings;
	std::vector<Spring*> ShearSprings;
	std::vector<Spring*> BendSprings;
	int width;
	int height;

	//Methods
	void createCloth(int width, int height);
	void populateNodes(int width, int height);
	void addNode(glm::vec3 position, bool immovable);
	void populateStructuralSprings(int width, int height);
	void populateShearSprings(int width, int height);
	void populateBendSprings(int width, int height);
	void addSpring(GameObject& node1, GameObject& node2, std::vector<Spring*>& springList);
};

