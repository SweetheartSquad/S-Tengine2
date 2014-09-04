#include "Scene.h"

Scene::Scene(void){
	this->children = new std::vector<Entity*>();
	this->camera = new Camera();

	//singletons
	this->renderSystem = &RenderSystem::getInstance();
	this->keyboard = &Keyboard::getInstance();
	this->mouse = &Mouse::getInstance();
}

Scene::~Scene(void){
	delete children;
	delete camera;
}

void Scene::update(void){
	camera->update();
}

void Scene::draw(){
	renderSystem->render(children, camera->getProjectionMatrix(), camera->getViewMatrix());
}

void Scene::addChild(Entity* child){
	children->push_back(child);
}
