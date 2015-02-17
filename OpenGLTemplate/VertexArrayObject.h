#pragma once
//System Includes
#include <vector>

//Custom includes
#include "OpenGL.h"
class VertexArrayObject
{
public:
	//Attributes
	GLuint VAO;
	GLuint numVertices;

	GLuint vPosition;
	GLuint vNormal;
	GLuint vTexCoord;

	//Constructor/Destructor
	VertexArrayObject(GLuint program, GLuint* vPosition, GLuint* vNormal, GLuint* vTexCoord);
	~VertexArrayObject(void);

	//Methods
	void loadData(std::vector<float>* vertexContainer, std::vector<float>* normalContainer, std::vector<float>* texCoordContainer);
	void activate();

private:
	//Attributes
	GLuint buffer;
	GLuint eBuffer;
};

