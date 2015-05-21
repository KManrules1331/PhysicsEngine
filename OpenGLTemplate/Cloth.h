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
	Cloth(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int width, int height);
	~Cloth();

	//Methods
	void draw() override;
	void update(float dt) override;
	Command* getPullCommand();
	void addForce(const glm::vec3& force);

protected:
	std::vector<Spring*> StructuralSprings;
	std::vector<Spring*> ShearSprings;
	std::vector<Spring*> BendSprings;

	Cloth(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);


	void addNode(glm::vec3 position, bool immovable);
	void addSpring(GameObject& node1, GameObject& node2, std::vector<Spring*>& springList);

private:
	//Attributes
	int width;
	int height;

	//Methods
	void createCloth(int width, int height);
	void populateNodes(int width, int height);
	void populateStructuralSprings(int width, int height);
	void populateShearSprings(int width, int height);
	void populateBendSprings(int width, int height);
};

