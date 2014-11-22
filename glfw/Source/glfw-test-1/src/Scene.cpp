#pragma once

#include "Scene.h"
#include "DepthFrameBuffer.h"
#include "StandardFrameBuffer.h"
#include "Camera.h"
#include "RenderOptions.h"
#include "RenderSurface.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "MatrixStack.h"

Scene::Scene(Game * _game):
	game(_game),
	camera(new Camera()),
	matrixStack(new MatrixStack()),
	renderOptions(new RenderOptions(nullptr, &lights)),
	shadowBuffer(new StandardFrameBuffer(true)),
	depthBuffer(new DepthFrameBuffer(true)),
	depthShader(new Shader("../assets/DepthMapShader", false, true)),
	shadowSurface(new RenderSurface(new Shader("../assets/shadow", false, true))),
	//Singletons
	keyboard(&Keyboard::getInstance()),
	mouse(&Mouse::getInstance())
{
}

Scene::~Scene(void){
	delete camera;
	delete matrixStack;
}

void Scene::update(void){
	camera->update();
	//Copy the transform to the light's data
	for(Light * light : lights){
		light->update();
	}
	for(Entity * e : children){
		e->update();
	}
}

void Scene::render(){
	int width, height;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
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

	matrixStack->projectionMatrix = camera->getProjectionMatrix();
	matrixStack->viewMatrix		  = camera->getViewMatrix();

	for(Entity * e : children){
		e->draw(matrixStack, renderOptions);
	}

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
	window = glfwCreateWindow(w, h, "VOX",  vox::fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if(!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	vox::initWindow(window);
	glfwDestroyWindow(vox::currentContext);
	glfwMakeContextCurrent(window);
	vox::currentContext = window;

	onContextChange();

	GLUtils::checkForError(0,__FILE__,__LINE__);
}

void Scene::onContextChange(){
	for(Entity * e : children){
		e->unload();
	}
	for(Entity * e : children){
		e->reset();
	}
}

void Scene::renderShadows(){
	Shader * backupOverride = renderOptions->overrideShader;
	int width, height;
	//Do we actuall need this?
	//glCullFace(GL_FRONT);
	glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
	depthBuffer->resize(width, height);
	depthBuffer->bindFrameBuffer();
	renderOptions->overrideShader = depthShader;

	Scene::render();

	shadowBuffer->resize(width, height);
	shadowBuffer->bindFrameBuffer();
	shadowSurface->render(depthBuffer->getTextureId(), shadowBuffer->frameBufferId);
	renderOptions->shadowMapTextureId = shadowBuffer->getTextureId();
	renderOptions->overrideShader = backupOverride;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Do we actually need this?
	//glCullFace(GL_BACK);
}