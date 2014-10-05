#include "Scene.h"

Scene::Scene(void):
	children(new std::vector<Entity *>()),
	camera(new Camera()),
	lights(new std::vector<Light *>()),

	//Singletons
	keyboard(&Keyboard::getInstance()),
	mouse(&Mouse::getInstance())
{
}

Scene::~Scene(void){
	delete children;
	delete camera;
	delete lights;
}

void Scene::update(void){
	camera->update();
}

void Scene::draw(){
	RenderSystem::render(vox::currentContext, children, camera->getProjectionMatrix(), camera->getViewMatrix());
}

void Scene::addChild(Entity* _child){
	children->push_back(_child);
}