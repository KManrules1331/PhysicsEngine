#include "Mesh.h"

#pragma region Static Attributes

Mesh* Mesh::cubeMesh;
Mesh* Mesh::sphereMesh;
Mesh* Mesh::squareMesh;
Mesh* Mesh::circleMesh;

#pragma endregion

#pragma region Constructor / Destructor

Mesh::Mesh(ShaderProgram* shader, Texture_IDs textureID)
{
	this->shader = shader;
	this->textureID = textureID;
	VAO = shader->createVertexArray();
}


Mesh::~Mesh(void)
{
	delete VAO;
}

#pragma endregion

#pragma region Methods

#pragma region Public

void Mesh::draw(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	shader->drawMesh(&position[0], &rotation[0], &scale[0], VAO, textureID);
}

void Mesh::init(ShaderProgram* shader)
{
	Mesh::cubeMesh = Mesh::createCube(shader);
	Mesh::sphereMesh = Mesh::createSphere(shader);
	Mesh::squareMesh = Mesh::createSquare(shader);
	Mesh::circleMesh = Mesh::createCircle(shader);
}

#pragma endregion

#pragma region Private

Mesh* Mesh::createCube(ShaderProgram* shader, int subdivisions)
{
	Mesh* cube = new Mesh(shader, Texture_IDs::WALL);
	std::vector<float> x = divisionLoop(-.5f, .5f, subdivisions);
	std::vector<float> y = divisionLoop(-.5f, .5f, subdivisions);
	std::vector<float> z = divisionLoop(-.5f, .5f, subdivisions);
	std::vector<float> u = divisionLoop(0.0f, 1.0f, subdivisions);
	std::vector<float> v = divisionLoop(0.0f, 1.0f, subdivisions);

	int n = x.size();
	for(int i = 0; i < n - 1; i++)
	{
		for(int j = 0; j < n - 1; j++)
		{
			float textureSquare1[8] = {
				u[i],	v[j],
				u[i+1],	v[j],
				u[i+1],	v[j+1],
				u[i],	v[j+1]
			};
			float textureSquare2[8] = {
				u[i],	v[j],
				u[i],	v[j+1],
				u[i+1],	v[j+1],
				u[i+1],	v[j]
			};

			//Front
			float fSquare[12] = {
				x[i],	y[j],	.5f,
				x[i+1],	y[j],	.5f,
				x[i+1],	y[j+1],	.5f,
				x[i],	y[j+1],	.5f
			};
			cube->addSquare(fSquare, textureSquare1);

			//Back
			float bSquare[12] = {
				x[i],	y[j],	-.5f,
				x[i],	y[j+1],	-.5f,
				x[i+1],	y[j+1],	-.5f,
				x[i+1],	y[j],	-.5f
			};
			cube->addSquare(bSquare, textureSquare2);

			//Top
			float tSquare[12] = {
				x[i],	.5f,	z[j],
				x[i],	.5f,	z[j+1],
				x[i+1],	.5f,	z[j+1],
				x[i+1],	.5f,	z[j]
			};
			cube->addSquare(tSquare, textureSquare2);

			//Bottom
			float boSquare[12] = {
				x[i],	-.5f,	z[j],
				x[i+1],	-.5f,	z[j],
				x[i+1],	-.5f,	z[j+1],
				x[i],	-.5f,	z[j+1]
			};
			cube->addSquare(boSquare, textureSquare1);

			//Left
			float lSquare[12] = {
				-.5f,	y[i],	z[j],
				-.5f,	y[i],	z[j+1],
				-.5f,	y[i+1],	z[j+1],
				-.5f,	y[i+1],	z[j]
			};
			cube->addSquare(lSquare, textureSquare2);

			//Right
			float rSquare[12] = {
				.5f,	y[i],	z[j],
				.5f,	y[i+1],	z[j],
				.5f,	y[i+1],	z[j+1],
				.5f,	y[i],	z[j+1]
			};
			cube->addSquare(rSquare, textureSquare1);
		}
	}
	cube->loadVAO();

	return cube;
}
Mesh* Mesh::createSphere(ShaderProgram* shader, int stacks, int slices)
{
	Mesh* sphere = new Mesh(shader, Texture_IDs::WALL);
	std::vector<float> y = cosRadialLoopDivisions(.5f, 0, PI, stacks);
	std::vector<float> xL = cosRadialLoopDivisions(0, 0, 2 * PI, slices);
	std::vector<float> zL = sinRadialLoopDivisions(0, 0, 2 * PI, slices);
	std::vector<float> u = divisionLoop(0.0f, 1.0f, slices);
	std::vector<float> v = divisionLoop(0.0f, 1.0f, stacks);

	int n = y.size();
	int m = xL.size();
	for(int j = 0; j < n-1; j++)
	{
		float upperStackRadius = .5f * sin((float)(j+1)/(float)(n-1) * PI);
		std::vector<float> xU = cosRadialLoopDivisions(upperStackRadius, 0, 2 * PI, slices);
		std::vector<float> zU = sinRadialLoopDivisions(upperStackRadius, 0, 2 * PI, slices);
		for(int i = 0; i < m - 1; i++)
		{
			float square[12] = {
				xL[i], y[j], zL[i],
				xL[i+1], y[j], zL[i+1],
				xU[i+1], y[j+1], zU[i+1],
				xU[i], y[j+1], zU[i]
			};
			float textureSquare[8] = {
				u[i],	v[j],
				u[i+1],	v[j],
				u[i+1],	v[j+1],
				u[i],	v[j+1]
			};
			sphere->addSquare(square, textureSquare);
		}

		xL = xU;
		zL = zU;
	}
	sphere->loadVAO();

	return sphere;
}
Mesh* Mesh::createSquare(ShaderProgram* shader, int subdivisions)
{
	Mesh* squareMesh = new Mesh(shader, Texture_IDs::WALL);
	std::vector<float> x = divisionLoop(-0.5f, 0.5f, subdivisions);
	std::vector<float> z = divisionLoop(-0.5f, 0.5f, subdivisions);
	std::vector<float> u = divisionLoop(0.0f, 1.0f, subdivisions);
	std::vector<float> v = divisionLoop(0.0f, 1.0f, subdivisions);

	int n = x.size();

	for (int i = 0; i < n - 1; i++)
	{
		for (int k = 0; k < n - 1; k++)
		{
			float square[12] = {
				x[i], 0.0f, z[k],
				x[i], 0.0f, z[k + 1],
				x[i + 1], 0.0f, z[k + 1],
				x[i+1], 0.0f, z[k]
			};
			float textureSquare[8] = {
				u[i], v[k],
				u[i], v[k + 1],
				u[i + 1], v[k + 1],
				u[i + 1], v[k]
			};
			squareMesh->addSquare(square, textureSquare);
		}
	}
	squareMesh->loadVAO();

	return squareMesh;
}
Mesh* Mesh::createCircle(ShaderProgram* shader, int slices)
{
	Mesh* circle = new Mesh(shader, Texture_IDs::WALL);
	std::vector<float> x = cosRadialLoopDivisions(0.5f, 0.0f, 2 * PI, slices);
	std::vector<float> z = sinRadialLoopDivisions(0.5f, 0.0f, 2 * PI, slices);
	std::vector<float> u = x;
	std::vector<float> v = z;

	int n = x.size();

	for (int i = 0; i < x.size(); i++)
	{
		u[i] = u[i] + 0.5f;
		v[i] = v[i] + 0.5f;
	}

	for (int i = 0; i < n - 1; i++)
	{
		float triangle[9] = {
			x[i+1], 0.0f, z[i+1],
			x[i], 0.0f, z[i],
			0.0f, 0.0f, 0.0f
		};
		float textureTriangle[6] = {
			u[i+1], v[i+1],
			u[i], v[i],
			0.0f, 0.0f
		};
		circle->addTriangle(triangle, textureTriangle);
	}

	circle->loadVAO();

	return circle;
}

void Mesh::loadVAO()
{
	VAO->loadData(&localVertexContainer, &localNormalContainer, &localTexCoordContainer);
}

void Mesh::addTriangle(float* vertData, float* textureData)
{
	for(int i = 0; i < 9; i++)
	{
		localVertexContainer.push_back(vertData[i]);
	}
	glm::vec3 vertex1 = glm::vec3(vertData[0], vertData[1], vertData[2]);
	glm::vec3 vertex2 = glm::vec3(vertData[3], vertData[4], vertData[5]);
	glm::vec3 vertex3 = glm::vec3(vertData[6], vertData[7], vertData[8]);

	//Calculate normals
	glm::vec3 edge1 = vertex2 - vertex1;
	glm::vec3 edge2 = vertex3 - vertex1;

	glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

	for(int i = 0; i < 3; i++)
	{
		localNormalContainer.push_back(normal.x);
		localNormalContainer.push_back(normal.y);
		localNormalContainer.push_back(normal.z);
	}


	for(int i = 0; i < 6; i++)
	{
		localTexCoordContainer.push_back(textureData[i]);
	}
}

void Mesh::addSquare(float* vertData, float* textureData)
{
	float tri1[9] = {
		vertData[0],	vertData[1],	vertData[2],
		vertData[3],	vertData[4],	vertData[5],
		vertData[6],	vertData[7],	vertData[8]
	};
	float texTri1[6] = {
		textureData[0],	textureData[1],
		textureData[2],	textureData[3],
		textureData[4],	textureData[5]
	};
	addTriangle(tri1, texTri1);

	float tri2[9] = {
		vertData[6],	vertData[7],	vertData[8],
		vertData[9],	vertData[10],	vertData[11],
		vertData[0],	vertData[1],	vertData[2]
	};
	float texTri2[6] = {
		textureData[4],	textureData[5],
		textureData[6],	textureData[7],
		textureData[0],	textureData[1]
	};
	addTriangle(tri2, texTri2);
}

std::vector<float> Mesh::divisionLoop(float min, float max, int subdivisions)
{
	std::vector<float> returnVector;

	for(float c = min; c < max - (float(max-min)/float(subdivisions)/2.0f); c+= float(max-min)/float(subdivisions))
	{
		returnVector.push_back(c);
	}
	returnVector.push_back(max);

	return returnVector;
}
std::vector<float> Mesh::sinRadialLoopDivisions(float radius, float minAngle, float maxAngle, int subdivisions)
{
	std::vector<float> returnVector;
	std::vector<float> angles = divisionLoop(minAngle, maxAngle, subdivisions);

	for(int i = 0; i < angles.size(); i++)
	{
		returnVector.push_back(radius * sin(angles[i]));
	}

	return returnVector;
}
std::vector<float> Mesh::cosRadialLoopDivisions(float radius, float minAngle, float maxAngle, int subdivisions)
{
	std::vector<float> returnVector;
	std::vector<float> angles = divisionLoop(minAngle, maxAngle, subdivisions);

	for(int i = 0; i < angles.size(); i++)
	{
		returnVector.push_back(radius * cos(angles[i]));
	}

	return returnVector;
}

#pragma endregion

#pragma endregion