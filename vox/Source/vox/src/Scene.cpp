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
#include "Game.h"
#include "Sprite.h"

Scene::Scene(Game * _game):
	game(_game),
	mouse(&Mouse::getInstance()),
	keyboard(&Keyboard::getInstance()),
	activeCamera(new PerspectiveCamera()),
	renderOptions(new RenderOptions(nullptr, &lights)),
	depthBuffer(new StandardFrameBuffer(true)),
	shadowBuffer(new StandardFrameBuffer(true)),
	depthShader(new DepthMapShader(true)),
	//Singletons
	shadowShader(new BlurShader(true)),
	shadowSurface(new RenderSurface(shadowShader)),
	matrixStack(new vox::MatrixStack())
{
	clearColor[0] = 0.0;
	clearColor[1] = 0.0;
	clearColor[2] = 0.0;
	clearColor[3] = 1.0;

	cameras.push_back(activeCamera);
}

Scene::~Scene(void){
	while(cameras.size() > 0){
		delete cameras.back();
		cameras.pop_back();
	}
	activeCamera = nullptr;

	while(lights.size() > 0){
		delete lights.back();
		lights.pop_back();
	}
	
	delete matrixStack;
	delete renderOptions;
	delete depthBuffer;
	delete shadowBuffer;
	delete depthShader;
	delete shadowSurface;
	delete shadowShader;
}

void Scene::update(Step * _step){
	if(!loaded){
		load();
	}
	activeCamera->update(_step);
	//for (Entity * e : children){
	for (unsigned long int i = 0; i < children.size(); ++i){
		Entity * e = children.at(i);
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
	
	NodeLoadable::load();
}

void Scene::unload(){
	for(Entity * e : children){
		e->unload();
	}
	depthBuffer->unload();
	shadowBuffer->unload();
	depthShader->unload();
	shadowSurface->unload();

	NodeLoadable::unload();
}


void Scene::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	//glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
	glfwMakeContextCurrent(glfwGetCurrentContext());
	float ratio;
	ratio = game->viewPortWidth / static_cast<float>(game->viewPortHeight);

	glEnable(GL_SCISSOR_TEST);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

	//glBlendFunc(GL_ONE, GL_ZERO);
	glEnable (GL_BLEND);

	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);

	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

	glViewport(game->viewPortX, game->viewPortY, game->viewPortWidth, game->viewPortHeight);
	glScissor(game->viewPortX, game->viewPortY, game->viewPortWidth, game->viewPortHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
     //glAlphaFunc ( GL_GREATER, 0.1 ) ;
    glEnable ( GL_ALPHA_TEST ) ;

	//Back-face culling
	//glEnable (GL_CULL_FACE); // cull face
    //glCullFace (GL_BACK); // cull back face

	//glFrontFace (GL_CW); // GL_CCW for counter clock-wise, GL_CW for clock-wise

	matrixStack->setProjectionMatrix(activeCamera->getProjectionMatrix());
	matrixStack->setViewMatrix(activeCamera->getViewMatrix());

	for(Entity * e : children){
		e->render(matrixStack, renderOptions);
	}
	
	checkForGlError(0,__FILE__,__LINE__);
    //glDisable(GL_POLYGON_OFFSET_FILL);
}

void Scene::addChild(Entity* _child){
	children.push_back(_child);
}

void Scene::removeChild(Entity* _child){
	for(signed long int j = children.size()-1; j >= 0; --j){
		if(children.at(j) == _child){
			children.erase(children.begin() + j);
		}
	}
}

void Scene::renderShadows(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	Shader * backupOverride = renderOptions->overrideShader;
	depthBuffer->resize(game->viewPortWidth, game->viewPortHeight);
	depthBuffer->bindFrameBuffer();
	renderOptions->overrideShader = depthShader;
	Scene::render(_matrixStack, _renderOptions);

	shadowBuffer->resize(game->viewPortWidth, game->viewPortHeight);
	shadowBuffer->bindFrameBuffer();
	shadowSurface->render(depthBuffer->getTextureId(), shadowBuffer->frameBufferId);
	static_cast<VoxRenderOptions *>(renderOptions)->shadowMapTextureId = shadowBuffer->getTextureId();
	renderOptions->overrideShader = backupOverride;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}