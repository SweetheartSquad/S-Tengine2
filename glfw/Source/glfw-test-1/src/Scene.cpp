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
	//renderSystem->render(vox::contexts.at(0), children, camera->getProjectionMatrix(), camera->getViewMatrix());
	/*for(GLFWwindow * context : vox::contexts){
		renderSystem->render(context, children, camera->getProjectionMatrix(), camera->getViewMatrix());
	}*/
	
	renderSystem->render(vox::contexts.at(vox::contexts.size()-1), children, camera->getProjectionMatrix(), camera->getViewMatrix());
	
}

void Scene::addChild(Entity* child){
	children->push_back(child);
}
