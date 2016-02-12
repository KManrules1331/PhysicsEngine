#include "Scene.h"

Scene::Scene()
{
}


Scene::~Scene(void)
{
	delete shader;
	delete camera;
	delete light;
	for(unsigned int i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
}

void Scene::Init()
{
	shader = new ShaderProgram("vShader.glsl", "fShader.glsl");
}

Camera * Scene::getCamera() const
{
	return camera;
}

void Scene::setCamera(Camera * newCamera)
{
	camera = newCamera;
}

Light * Scene::getLight() const
{
	return light;
}

void Scene::setLight(Light * newLight)
{
	light = newLight;
}

void Scene::updateScene(float dt)
{
	for(unsigned int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update(dt);
	}
	HardBodyPhysics::update();
}

void Scene::drawScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	shader->updateUniforms(camera, light);
	for(unsigned int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->draw();
	}

	glutSwapBuffers();
}

void Scene::addObject(GameObject* obj)
{
	gameObjects.push_back(obj);
}