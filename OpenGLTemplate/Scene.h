#pragma once
//System includes

//Custom includes
#include "Light.h"
#include "GameObject.h"
#include "ShaderProgram.h"
#include "HardBodyPhysics.h"
#include "Camera.h"

class Scene
{
public:
	Scene();
	Scene(Camera* camera, Light* light);
	~Scene(void);

	//Attributes
	ShaderProgram* shader;

	//getters/setters
	Camera* getCamera() const;
	void setCamera(Camera* newCamera);

	Light* getLight() const;
	void setLight(Light* newLight);

	//Methods
	void Init();
	void updateScene(float dt);
	void drawScene();
	void addObject(GameObject* obj);

private:

	//Attributes
	Light* light;
	std::vector<GameObject*> gameObjects;
	Camera* camera;
};

