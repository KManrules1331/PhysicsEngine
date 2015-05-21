#pragma once
#include "Cloth.h"

class SoftBodySphere :
	public Cloth
{
public:
	SoftBodySphere(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~SoftBodySphere();

private:
	struct ClothList {
		int rightIndex;
		int downIndex;
		int nodeIndex;
		~ClothList();
	};
	int width;
	int height;
	int depth;
	float radius;

	void pushNode(Transform* node);
	void makeCube();
	void populateStructuralSprings(std::vector<ClothList*>& list);

	void makeAllSprings(ClothList* a);
};

