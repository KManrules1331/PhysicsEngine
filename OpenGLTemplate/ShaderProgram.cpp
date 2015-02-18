#include "ShaderProgram.h"

#pragma region Constructors

ShaderProgram::ShaderProgram(char* vertexShader, char* fragmentShader)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	program = createShaderProgram(vertexShader, fragmentShader);
	if(!program) {
		std::cerr << "Error setting up shaders" << std::endl;
		exit(1);
	}
	glUseProgram(program);

	//Install Uniforms
	transformMatrix = glGetUniformLocation(program, "transformMatrix");
	rotationMatrix = glGetUniformLocation(program, "rotationMatrix");
	cameraPosition = glGetUniformLocation(program, "cameraPosition");
	projMatrix = glGetUniformLocation(program, "projMatrix");
	viewMatrix = glGetUniformLocation(program, "viewMatrix");
	lightPosition = glGetUniformLocation(program, "lightPosition");

	//Load textures
	loadTexture("texture.png", &textures[WALL]);
}

#pragma endregion

#pragma region Destructors

ShaderProgram::~ShaderProgram(void)
{
}

#pragma endregion

#pragma region Methods

#pragma region Public

void ShaderProgram::drawMesh(const float* transformMatrix, const float* rotationMatrix, VertexArrayObject* VAO, Texture_IDs texture)
{
	VAO->activate();
	glBindTexture(GL_TEXTURE_2D, textures[texture]);
	glUniformMatrix4fv(this->transformMatrix, 1, GL_FALSE, transformMatrix);
	glUniformMatrix4fv(this->rotationMatrix, 1, GL_FALSE, rotationMatrix);
	glDrawElements(GL_TRIANGLES, VAO->numVertices, GL_UNSIGNED_SHORT, (void*)0);
}

void ShaderProgram::loadTexture(char* textureFilePath, GLuint* textureID)
{
	*textureID = SOIL_load_OGL_texture
		(
			textureFilePath,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
		);
}

void ShaderProgram::updateUniforms(Camera* camera, Light* light)
{
	camera->bindUniforms(program, cameraPosition, projMatrix, viewMatrix);
	light->bindUniform(program, lightPosition);
}

VertexArrayObject* ShaderProgram::createVertexArray()
{
	return new VertexArrayObject(program, &vPosition, &vNormal, &vTexCoord);
}

#pragma endregion

#pragma region Private

GLuint ShaderProgram::createShaderProgram(char* vertexShaderFilePath, char* fragmentShaderFilePath)
{
	//Create program
	GLuint program = glCreateProgram();
	Shader vertexShader = Shader(vertexShaderFilePath, GL_VERTEX_SHADER);
	Shader fragmentShader = Shader(fragmentShaderFilePath, GL_FRAGMENT_SHADER);
	glAttachShader(program, vertexShader.shader);
	glAttachShader(program, fragmentShader.shader);
	printProgramInfoLog(program);

	//Link program
	glLinkProgram(program);
	printProgramInfoLog(program);
	return program;
}

void ShaderProgram::printProgramInfoLog(GLuint program)
{
	GLint infologLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);
	if(infologLength > 0)
	{
		GLsizei charsWritten = 0;
		char* infoLog = new char[infologLength];
		glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
		std::cout << infoLog << std::endl;
		delete[] infoLog;
	}
}

#pragma endregion

#pragma endregion

#pragma region Shader

#pragma region Constructors

ShaderProgram::Shader::Shader(char* shaderFile, GLenum type)
{
	shader = glCreateShader(type);

	{//Lifetime of file

		const char* shaderContents = readTextFile(shaderFile);
		if(!shaderContents)
		{
			std::cerr << "Error reading vertex shader file " << shaderFile << std::endl;
			return;
		}

		glShaderSource(shader, 1, &(shaderContents), NULL);

		delete[] shaderContents;
	}

	//Compile
	glCompileShader(shader);
	printShaderInfoLog(shader);
}

#pragma endregion

#pragma region Destructors

ShaderProgram::Shader::~Shader(void)
{
}

#pragma endregion

#pragma region Methods

#pragma region Public



#pragma endregion

#pragma region Private
char* ShaderProgram::Shader::readTextFile(char* filePath)
{
	char* fileContents = nullptr;
	if(filePath)
	{
		std::ifstream file(filePath, std::ios::binary);
		//Find file end
		file.seekg(0, std::ios::end);
		int fileSize = file.tellg();
		int fileLength = fileSize / sizeof(char);
		if(fileLength > 0)
		{
			fileContents = new char[fileLength+1];
			file.seekg(0, std::ios::beg);
			assert(0 == file.tellg());
			file.read(fileContents, fileSize);
			fileContents[fileLength] = '\0';
		}
		else
		{
			std::cerr << "Error: File " << filePath << " contained no code" << std::endl;
		}
		file.close();
	}
	else
	{
		std::cerr << "Error reading " << filePath << std::endl;
	}
	return fileContents;
}

void ShaderProgram::Shader::printShaderInfoLog(GLuint shader)
{
	GLint infologLength = 0;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

	if(infologLength > 0)
	{
		GLsizei charsWritten = 0;
		char* infoLog = new char[infologLength];
		glGetProgramInfoLog(shader, infologLength, &charsWritten, infoLog);
		std::cout << infoLog << std::endl;
		delete[] infoLog;
	}
}

#pragma endregion

#pragma endregion

#pragma endregion
