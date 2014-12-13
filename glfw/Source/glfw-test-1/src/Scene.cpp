#pragma once

#include "Scene.h"
#include "DepthFrameBuffer.h"
#include "StandardFrameBuffer.h"
#include "Camera.h"
#include "GLFWRenderOptions.h"
#include "RenderSurface.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "MatrixStack.h"

Scene::Scene(Game * _game):
	game(_game),
	camera(new Camera()),
	matrixStack(new vox::MatrixStack()),
	renderOptions(new RenderOptions(nullptr, &lights)),
	shadowBuffer(new StandardFrameBuffer(true)),
	depthBuffer(new StandardFrameBuffer(true)),
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
	for(Entity * e : children){
		e->update();
	}
}

void Scene::setViewport(float _x, float _y, float _w, float _h){
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

void Scene::render(){
	//glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
	glfwMakeContextCurrent(glfwGetCurrentContext());
	float ratio;
	ratio = w / static_cast<float>(h);

	glEnable(GL_SCISSOR_TEST);

	glViewport(x, y, w, h);
	glScissor(x, y, w ,h);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	//Back-face culling
	//glEnable (GL_CULL_FACE); // cull face
	//glCullFace (GL_BACK); // cull back face

	glFrontFace (GL_CW); // GL_CCW for counter clock-wise, GL_CW for clock-wise

	matrixStack->projectionMatrix = camera->getProjectionMatrix();
	matrixStack->viewMatrix		  = camera->getViewMatrix();

	for(Entity * e : children){
		e->render(matrixStack, renderOptions);
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
	//int width, height;
	//Do we actuall need this?
	//glCullFace(GL_FRONT);
	//glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
	depthBuffer->resize(w, h);
	depthBuffer->bindFrameBuffer();
	renderOptions->overrideShader = depthShader;
	Scene::render();

	shadowBuffer->resize(w, h);
	shadowBuffer->bindFrameBuffer();
	shadowSurface->render(depthBuffer->getTextureId(), shadowBuffer->frameBufferId);
	((GLFWRenderOptions *)renderOptions)->shadowMapTextureId = shadowBuffer->getTextureId();
	renderOptions->overrideShader = backupOverride;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Do we actually need this?
	//glCullFace(GL_BACK);
}