#include "Scene.h"

Scene::Scene(void):
	children(new std::vector<Entity *>()),
	camera(new Camera()),

	//Singletons
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
	RenderSystem::render(vox::currentContext, children, camera->getProjectionMatrix(), camera->getViewMatrix());
}

void Scene::addChild(Entity* _child){
	children->push_back(_child);
}

void Scene::toggleFullScreen(){
	// Toggle fullscreen flag.
	vox::fullscreen = !vox::fullscreen;
	//get size
	int w, h;
	//if(vox::fullscreen){
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	w = mode->width;
	h = mode->height;

	if(!vox::fullscreen){
		w /= 2;
		h /= 2;
	}

	// Create the new window.
	GLFWwindow* window;
	window = glfwCreateWindow(w, h, "VOX",  vox::fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if(!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	vox::initWindow(window);

	glfwDestroyWindow(vox::currentContext);

	glfwMakeContextCurrent(window);
	vox::currentContext = window;

	for(Entity * e : *children){
		e->unload();
	}
	for(Entity * e : *children){
		e->reset();
	}

	GLUtils::checkForError(0,__FILE__,__LINE__);
}