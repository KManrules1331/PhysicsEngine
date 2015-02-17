#pragma once
//System Includes
#include <iostream>
#include <fstream>
#include <map>

//Custom Includes
#include "OpenGL.h"
#include "Camera.h"
#include "Light.h"
#include "VertexArrayObject.h"
#include "Textures.h"

class ShaderProgram
{
public:
#pragma region Constructors/Desturctors
	ShaderProgram(char* vertexShader = "vShader.glsl", char* fragmentShader = "fShader.glsl");
	~ShaderProgram(void);
#pragma endregion

#pragma region Attributes
	//Shader Program
	GLuint program;


#pragma endregion

#pragma region Methods

	void drawMesh(float* position, float* rotation, float* scale, VertexArrayObject* VAO, Texture_IDs texture);
	void loadData(std::vector<float>* vertices, std::vector<float>* normals, std::vector<float>* textureCoordinates);
	void updateUniforms(Camera*, Light*);
	void loadTexture(char*, GLuint*);
	VertexArrayObject* createVertexArray();

	void loadAttributes(int normalsOffset, int texturesOffset);

#pragma endregion

private:
#pragma region Attributes
	//VAO
	std::map<Texture_IDs, GLuint> textures;

	//Uniforms
	GLuint position;
	GLuint rotation;
	GLuint scale;
	GLuint cameraPosition;
	GLuint projMatrix;
	GLuint viewMatrix;
	GLuint lightPosition;
	GLuint texture;

	//Shader Attributes
	GLuint vPosition;
	GLuint vNormal;
	GLuint vTexCoord;


#pragma endregion

#pragma region Methods

	GLuint createShaderProgram(char* vertexShaderFile, char* fragmentShaderFile);
	char* readTextFile(char* filePath);
	void printProgramInfoLog(GLuint program);

#pragma endregion

#pragma region Classes

	class Shader
	{
	public:
		GLuint shader;
		Shader(char* shaderFile, GLenum type);
		~Shader(void);
		GLuint createShaderProgram(char* shaderFile);
	private:
		char* readTextFile(char* filePath);
		void printShaderInfoLog(GLuint shader);
	};

#pragma endregion
};