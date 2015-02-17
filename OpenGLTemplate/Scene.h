#pragma once
//System includes

//Custom includes
#include "Light.h"
#include "GameObject.h"
#include "ShaderProgram.h"
#include "Camera.h"

class Scene
{
public:
	Scene(Camera* camera, Light* light);
	~Scene(void);

	//Attributes
	ShaderProgram* shader;

	//Methods
	void updateScene();
	void drawScene();
	void addObject(GameObject* obj);

private:

	//Attributes
	Light* light;
	std::vector<GameObject*> gameObjects;
	Camera* camera;
};

