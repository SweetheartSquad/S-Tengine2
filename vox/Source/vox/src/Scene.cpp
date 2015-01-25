#pragma once

#include "Scene.h"
#include "DepthFrameBuffer.h"
#include "StandardFrameBuffer.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "VoxRenderOptions.h"
#include "RenderSurface.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "MatrixStack.h"
#include "shader/DepthMapShader.h"
#include "shader/BlurShader.h"
#include "Light.h"
#include "Entity.h"
#include "MatrixStack.h"

Scene::Scene(Game * _game):
	game(_game),
	mouse(&Mouse::getInstance()),
	keyboard(&Keyboard::getInstance()),
	camera(new PerspectiveCamera()),
	renderOptions(new RenderOptions(nullptr, &lights)),
	depthBuffer(new StandardFrameBuffer(true)),
	shadowBuffer(new StandardFrameBuffer(true)),
	depthShader(new DepthMapShader(true)),
	//Singletons
	shadowSurface(new RenderSurface(new BlurShader(true))),
	matrixStack(new vox::MatrixStack())
{
	int width, height;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
	viewPortWidth = width;
	viewPortHeight = height;
	viewPortX = 0;
	viewPortY = 0;
}

Scene::~Scene(void){
	delete camera;
	delete matrixStack;
	delete renderOptions;
	delete depthBuffer;
	delete shadowBuffer;
	delete depthShader;
	delete shadowSurface;
	for(auto light : lights){
		delete light;
	}
}

void Scene::update(Step * _step){
	camera->update(_step);
	for(Entity * e : children){
		e->update(&vox::step);
	}
}

void Scene::load(){
	for(Entity * e : children){
		e->load();
	}
	depthBuffer->load();
	shadowBuffer->load();
	depthShader->load();
	shadowSurface->load();
}

void Scene::unload(){
	for(Entity * e : children){
		e->unload();
	}
	depthBuffer->unload();
	shadowBuffer->unload();
	depthShader->unload();
	shadowSurface->unload();
}

void Scene::setViewport(float _x, float _y, float _w, float _h){
	viewPortX = _x;
	viewPortY = _y;
	viewPortWidth = _w;
	viewPortHeight = _h;
}

void Scene::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	//glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
	glfwMakeContextCurrent(glfwGetCurrentContext());
	float ratio;
	ratio = viewPortWidth / static_cast<float>(viewPortHeight);

	glEnable(GL_SCISSOR_TEST);
    //glEnable(GL_POLYGON_OFFSET_FILL);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ZERO);
	glEnable (GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glViewport(viewPortX, viewPortY, viewPortWidth, viewPortHeight);
	glScissor(viewPortX, viewPortY, viewPortWidth, viewPortHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	//Back-face culling
	//glEnable (GL_CULL_FACE); // cull face
    //glCullFace (GL_BACK); // cull back face

	//glFrontFace (GL_CW); // GL_CCW for counter clock-wise, GL_CW for clock-wise

	matrixStack->projectionMatrix = camera->getProjectionMatrix();
	matrixStack->viewMatrix		  = camera->getViewMatrix();

	//float offset = children.size();
	for(Entity * e : children){
		e->render(matrixStack, renderOptions);
		//glPolygonOffset(offset, 1);
		//offset -= 1.f;
	}

	GLUtils::checkForError(0,__FILE__,__LINE__);
    //glDisable(GL_POLYGON_OFFSET_FILL);
}

void Scene::addChild(Entity* _child){
	if(renderOptions->alphaSorting){
		float z;// = -100000;
		float childZ = _child->transform->translationVector.z;
		if(children.size() > 0){
			z = children.at(0)->transform->translationVector.z;
			std::cout << "first check; z: " << z << " vs. childZ: " << childZ << std::endl;
			if(childZ < z){
				std::cout << "inserted at start" << std::endl;
				children.insert(children.begin(), _child);
				return;
			}
			for(unsigned long int i = 0; i < children.size(); ++i){
				z = children.at(i)->transform->translationVector.z;
				std::cout << "z: " << z << " vs. childZ: " << childZ << std::endl;
				if(childZ > z && i+1 < children.size() && childZ < children.at(i+1)->transform->translationVector.z){
					std::cout << "inserted at " << i+1 << std::endl;
					children.insert(children.begin()+i+1, _child);
					return;
				}
			}
		}
	}
	children.push_back(_child);
	std::cout << "inserted at end" << std::endl;
}

void Scene::alphaSort(){

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

	int width, height;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
	viewPortWidth = width;
	viewPortHeight = height;
	viewPortX = 0;
	viewPortY = 0;

	unload();
	load();

	GLUtils::checkForError(0,__FILE__,__LINE__);
}

void Scene::renderShadows(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	Shader * backupOverride = renderOptions->overrideShader;
	depthBuffer->resize(viewPortWidth, viewPortHeight);
	depthBuffer->bindFrameBuffer();
	renderOptions->overrideShader = depthShader;
	Scene::render(_matrixStack, _renderStack);

	shadowBuffer->resize(viewPortWidth, viewPortHeight);
	shadowBuffer->bindFrameBuffer();
	shadowSurface->render(depthBuffer->getTextureId(), shadowBuffer->frameBufferId);
	static_cast<VoxRenderOptions *>(renderOptions)->shadowMapTextureId = shadowBuffer->getTextureId();
	renderOptions->overrideShader = backupOverride;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}