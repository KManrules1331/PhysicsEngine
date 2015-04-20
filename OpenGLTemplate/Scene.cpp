#include "Scene.h"


Scene::Scene(Camera* camera, Light* light)
{
	shader = new ShaderProgram("vShader.glsl", "fShader.glsl");
	this->camera = camera;
	this->light = light;
}


Scene::~Scene(void)
{
	delete shader;
	delete camera;
	delete light;
	for(int i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
}

void Scene::updateScene(float dt)
{
	for(int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update(dt);
	}
	HardBodyPhysics::update(dt);
}

void Scene::drawScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	shader->updateUniforms(camera, light);
	for(int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->draw();
	}

	glutSwapBuffers();
}

void Scene::addObject(GameObject* obj)
{
	gameObjects.push_back(obj);
}