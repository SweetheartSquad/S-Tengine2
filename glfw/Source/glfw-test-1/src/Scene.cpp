#include "Scene.h"

Scene::Scene(void)
{
	this->children = new std::vector<Entity*>();
	this->renderSystem = &RenderSystem::getInstance();
	this->keyboard = &Keyboard::getInstance();
	this->mouse = &Mouse::getInstance();
	this->camera = new Camera();
}

Scene::~Scene(void)
{
}

void Scene::update(void){
}

void Scene::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
	renderSystem->render(children, camera->getProjectionMatrix(), camera->getViewMatrix());
}


