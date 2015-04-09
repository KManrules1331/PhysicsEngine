#pragma once
//System includes
#include <vector>

//Custom includes
#include "OpenGL.h"
#include "ShaderProgram.h"
#include "Textures.h"
#include "Transform.h"

class Mesh
{
public:
	//Attributes
	static Mesh* sphereMesh;
	static Mesh* cubeMesh;
	static Mesh* squareMesh;
	static Mesh* circleMesh;

	//Constructor/Destructor
	Mesh(ShaderProgram* shader, Texture_IDs texture);
	~Mesh(void);

	//Methods
	static void init(ShaderProgram* shader);
	void draw(const Transform* transform);

private:
	//Attributes
	ShaderProgram* shader;
	VertexArrayObject* VAO;
	Texture_IDs textureID;
	std::vector<float> localVertexContainer;
	std::vector<float> localNormalContainer;
	std::vector<float> localTexCoordContainer;

	void loadVAO();

	//Mesh functions
	static Mesh* createCube(ShaderProgram* shader, int subdivisions = 5);
	static Mesh* createSphere(ShaderProgram* shader, int stacks = 20, int slices = 20);
	static Mesh* createSquare(ShaderProgram* shader, int subdivisions = 1);
	static Mesh* createCircle(ShaderProgram* shader, int slices = 20);

	//Helper functions
	void addTriangle(float* vertData, float* textureData);
	void addSquare(float* vertData, float* textureData);
	void addRoundedSquare(float* vertData, float* textureData, glm::vec3 center);
	void addRoundedTriangle(float* vertData, float* textureData, glm::vec3 center);

	//Math helper functions
	static std::vector<float> divisionLoop(float min, float max, int subdivisions);
	static std::vector<float> sinRadialLoopDivisions(float radius, float minAngle, float maxAngle, int subdivisions);
	static std::vector<float> cosRadialLoopDivisions(float radius, float minAngle, float maxAngle, int subdivisions);
};

