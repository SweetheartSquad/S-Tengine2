#include "Scene.h"

Scene::Scene(Game * _game):
	game(_game),
	camera(new Camera()),
	frameBuffer(FrameBufferInterface(0, 0)),
	renderSurface(RenderSurface(new Shader("../assets/RenderSurface", false, true))),
	//Singletons
	keyboard(&Keyboard::getInstance()),
	mouse(&Mouse::getInstance())
{
}

Scene::~Scene(void){
	delete camera;
}

void Scene::update(void){
	camera->update();
	//Copy the transform to the light's data
	for(Light * light : lights){
		light->update();
	}
}

void Scene::render(){
	int width, height;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
	
	frameBuffer.resize(width, height);
	frameBuffer.bindFrameBuffer();
	
	glfwMakeContextCurrent(glfwGetCurrentContext());
	float ratio;
	ratio = width / static_cast<float>(height);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//Back-face culling
	//glEnable (GL_CULL_FACE); // cull face
	//glCullFace (GL_BACK); // cull back face

	glFrontFace (GL_CW); // GL_CCW for counter clock-wise, GL_CW for clock-wise

	for(Entity * e : children){
		e->draw(camera->getProjectionMatrix(), camera->getViewMatrix(), lights);
	}

	renderSurface.render(frameBuffer);
	GLUtils::checkForError(0,__FILE__,__LINE__);
}

void Scene::addChild(Entity* _child){
	children.push_back(_child);
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
	GLFWwindow * window;
	window = glfwCreateWindow(w, h, "VOX",  /*vox::fullscreen ? glfwGetPrimaryMonitor() :*/ nullptr, nullptr);
	if(!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	vox::initWindow(window);
	glfwDestroyWindow(vox::currentContext);
	glfwMakeContextCurrent(window);
	vox::currentContext = window;

	for(Entity * e : children){
		e->unload();
	}
	for(Entity * e : children){
		e->reset();
	}

	GLUtils::checkForError(0,__FILE__,__LINE__);
}