#include "VertexArrayObject.h"


VertexArrayObject::VertexArrayObject(GLuint program, GLuint* vPosition, GLuint* vNormal, GLuint* vTexCoord)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glGenBuffers(1, &eBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eBuffer);
	*vPosition = glGetAttribLocation(program, "vPosition");
	this->vPosition = *vPosition;
	glEnableVertexAttribArray(*vPosition);
	*vNormal = glGetAttribLocation(program, "vNormal");
	this->vNormal = *vNormal;
	glEnableVertexAttribArray(*vNormal);
	*vTexCoord = glGetAttribLocation(program, "vTexCoord");
	this->vTexCoord = *vTexCoord;
	glEnableVertexAttribArray(*vTexCoord);
}


VertexArrayObject::~VertexArrayObject(void)
{
}

void VertexArrayObject::loadData(	std::vector<float>* vertexContainer, 
									std::vector<float>* normalContainer, 
									std::vector<float>* texCoordContainer)
{
	glBindVertexArray(VAO);

	int size = vertexContainer->size();
	numVertices = size / 3;
	float* vertexArray = new float[size];
	float* normalArray = new float[size];
	float* texCooArray = new float[size*2/3];

	for(int i=0; i < size; i++)
	{
		vertexArray[i] = (*vertexContainer)[i];
		normalArray[i] = (*normalContainer)[i];
	}
	for(int i=0; i<size*2/3; i++)
	{
		texCooArray[i] = (*texCoordContainer)[i];
	}
	//Set up glVertexAttribPointer
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, ((void*)0));
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, ((void*)(size * sizeof(float))));
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, ((void*)(2 * size * sizeof(float))));

	//Set up buffer
	glBufferData(GL_ARRAY_BUFFER, size * 8/3 * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(float), vertexArray);
	glBufferSubData(GL_ARRAY_BUFFER, size * sizeof(float), size * sizeof(float), normalArray);
	glBufferSubData(GL_ARRAY_BUFFER, 2 * size * sizeof(float), 2 * size * sizeof(float) / 3, texCooArray);

	delete[] vertexArray;
	delete[] normalArray;
	delete[] texCooArray;

	GLushort* elemArray = new GLushort[size];
	for(int i = 0; i < size; i++)
	{
		elemArray[i] = i;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size / 3 * sizeof(GLushort), elemArray, GL_STATIC_DRAW);
	delete[] elemArray;
}

void VertexArrayObject::activate()
{
	glBindVertexArray(VAO);
}
