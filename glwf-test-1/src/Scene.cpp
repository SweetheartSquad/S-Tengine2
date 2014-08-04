#include "Scene.h"


Scene::Scene(void)
{
	this->children = new std::vector<Entity*>();
	this->renderSystem = &RenderSystem::getInstance();
}

Scene::~Scene(void)
{
}

void Scene::update(void){
}

void Scene::draw(void){
	renderSystem->render(children);
}

void Scene::addChild(Entity* child)
{
	children->push_back(child);
}

void Scene::removeChildAtIndex(int index)
{
	children->erase(children->begin()+index-1);
}