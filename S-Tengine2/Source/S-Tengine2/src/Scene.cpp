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
	depthBuffer(new StandardFrameBuffer(true)),
	shadowBuffer(new StandardFrameBuffer(true)),
	depthShader(new DepthMapShader(true)),
	//Singletons
	shadowShader(new BlurShader(true)),
	shadowSurface(new RenderSurface(shadowShader))
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
	
	depthBuffer->safeDelete();
	shadowBuffer->safeDelete();
	depthShader->safeDelete();
	delete shadowSurface;
}

void Scene::update(Step * _step){
	if(!loaded){
		load();
	}
	activeCamera->update(_step);
	childButNotReally->update(_step);
}

void Scene::load(){
	shadowSurface->load();
	depthShader->load();
	shadowBuffer->load();
	depthBuffer->load();
	childButNotReally->load();
	
	NodeLoadable::load();
}

void Scene::unload(){
	childButNotReally->unload();
	depthBuffer->unload();
	shadowBuffer->unload();
	depthShader->unload();
	shadowSurface->unload();

	NodeLoadable::unload();
}


void Scene::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	//glAlphaFunc ( GL_GREATER, 0.1 ) ;
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	//Back-face culling
	//glEnable (GL_CULL_FACE); // cull face
	//glCullFace (GL_BACK); // cull back face

	//glFrontFace (GL_CW); // GL_CCW for counter clock-wise, GL_CW for clock-wise

	_matrixStack->pushMatrix();
	_matrixStack->resetCurrentMatrix();
	_matrixStack->setProjectionMatrix(&activeCamera->getProjectionMatrix());
	_matrixStack->setViewMatrix(&activeCamera->getViewMatrix());

	_renderOptions->lights = &lights;
	
	clear();
	childButNotReally->render(_matrixStack, _renderOptions);
	
	_matrixStack->popMatrix();

	checkForGlError(0,__FILE__,__LINE__);
}

void Scene::renderShadows(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	Shader * backupOverride = _renderOptions->overrideShader;
	depthBuffer->resize(game->viewPortWidth, game->viewPortHeight);
	depthBuffer->bindFrameBuffer();
	_renderOptions->overrideShader = depthShader;
	Scene::render(_matrixStack, _renderOptions);

	shadowBuffer->resize(game->viewPortWidth, game->viewPortHeight);
	shadowBuffer->bindFrameBuffer();
	shadowSurface->render(depthBuffer->getTextureId(), shadowBuffer->frameBufferId);
	static_cast<VoxRenderOptions *>(_renderOptions)->shadowMapTextureId = shadowBuffer->getTextureId();
	_renderOptions->overrideShader = backupOverride;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::clear(){
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}