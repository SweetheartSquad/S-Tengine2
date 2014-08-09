#include "Scene.h"


Scene::Scene(void)
{
	this->children = new std::vector<Entity*>();
	this->renderSystem = &RenderSystem::getInstance();
	this->keyboard = &Keyboard::getInstance();
	this->mouse = &Mouse::getInstance();
}

Scene::~Scene(void)
{
}

void Scene::update(void){
}

void Scene::draw(void){
	renderSystem->render(children);
}


