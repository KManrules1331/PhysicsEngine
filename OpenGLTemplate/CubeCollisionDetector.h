#pragma once
#include "CollisionDetector.h"
#include "Transform.h"
class CubeCollisionDetector :
	public CollisionDetector
{
public:

	enum CollisionType {
		Cube,
		Sphere
	};

	//Constructors
	CubeCollisionDetector(Transform& GOTransform);
	~CubeCollisionDetector(void);

	//Methods
	bool detectCollision(CollisionDetector& c) override;
	//CollisionTypes
	bool detectCubeCollision(CubeCollisionDetector& c) override;
	bool detectSphereCollision(SphereCollisionDetector& c) override;

	bool getCollisionInfo(CollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact) override;

	//CollisionTypes
	bool getCubeCollisionInfo(CubeCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact) override;
	bool getSphereCollisionInfo(SphereCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact) override;

	//SAT collisions//TODO//Implement overloads for spheres
	bool getSATCollisionInfo(CubeCollisionDetector& c, Contact* contact) override;
	bool getSATCollisionInfo(CollisionDetector& c, Contact* contact) override;
	bool getSATCollisionInfo(SphereCollisionDetector& c, Contact* contact) override;

	glm::vec3 getAngularAcceleration(glm::vec3 torque, float inverseMass) const override;

	CollisionDetector::ContainingBox getAABB() override;

	//Some getters
	float getHalfsize() const;

private:
	float halfsize = 0.5f;		//Hard coded this for now but we can parameterize later

	//I like to pass by const reference so I made all this stuff const
	std::vector<glm::vec3> getVertices() const;
	std::vector<std::vector<glm::vec3>> getEdges() const;
	std::vector<glm::vec3> getNormals() const;
	void addEdgeToList(std::vector<std::vector<glm::vec3>>* list, std::vector<glm::vec3>* points, int pointNum1, int pointNum2) const;

	//SAT data/////////////////////////////////////////////////////////////////New Stuff//////////////
	//Don't think I will need this stuff
	//SAT requires axes to be face normals and all cross products of edge axes
	std::vector<glm::vec3> edgeAxes;
	GLuint numVerts;
	std::vector<glm::vec3> verts;

	//SAT helper methods///////////////////////////////////////////////////////
	//glm::vec3 getNormalFromFace(Face face);
	void getMinMaxProjections(const glm::vec3 axis, float* min, float* max, int* minIndex, int* maxIndex) const;
	float getImpulseScalar(CubeCollisionDetector* b, Contact contact);

	//Use this as a helper method in the one below
	glm::vec3 getEdgeContactPoint(const glm::vec3& axisOne, const glm::vec3& axisTwo, const glm::vec3& ptOnEdgeOne, const glm::vec3& ptOnEdgeTwo, float* depth);

	Contact getFaceContact(const CubeCollisionDetector& a, const CubeCollisionDetector& b);
	Contact getEdgeContact(const CubeCollisionDetector& a, const CubeCollisionDetector& b);
	float overlapAmount(glm::vec3 axis, const CubeCollisionDetector& a, const CubeCollisionDetector& b);	//TODO//Write an overload for this for the face projections to use
	float overlapAmount(glm::vec3 axis, const CubeCollisionDetector& a, const CubeCollisionDetector& b, int* vertIndex);
	glm::vec3 findClosest(std::vector<glm::vec3> set, glm::vec3 point);
	std::vector<glm::vec3> getEdgePoints(int index) const;

	//This is for initialization
	void createCube();

	//Old getters for edges and verts
	std::vector<glm::vec3> getModelEdgeAxes() const;
	std::vector<glm::vec3> getWorldEdgeAxes() const;
	glm::vec3 getWorldEdgeAxis(GLuint index) const;

	//New stuff not recycled from old code//
	std::vector<glm::vec3> getWorldVerts() const;
	glm::vec3 getWorldVert(GLuint i) const;

	//KYLE I APOLOGIZE FOR TRIPLING THE SIZE OF THIS CLASS
};

