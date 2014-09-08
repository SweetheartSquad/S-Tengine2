#include "Scene.h"

Scene::Scene(void):
	children(new std::vector<Entity *>()),
	camera(new Camera()),

	//singletons
	renderSystem(&RenderSystem::getInstance()),
	keyboard(&Keyboard::getInstance()),
	mouse(&Mouse::getInstance())
{
}

Scene::~Scene(void){
	delete children;
	delete camera;
}

void Scene::update(void){
	camera->update();
}

void Scene::draw(){
	renderSystem->render(vox::currentContext, children, camera->getProjectionMatrix(), camera->getViewMatrix());
}

void Scene::addChild(Entity* _child){
	children->push_back(_child);
}
