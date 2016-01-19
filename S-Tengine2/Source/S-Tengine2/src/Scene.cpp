#pragma once

#include "Scene.h"
#include "DepthFrameBuffer.h"
#include "StandardFrameBuffer.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "VoxRenderOptions.h"
#include "RenderSurface.h"
#include "MatrixStack.h"
#include "shader/DepthMapShader.h"
#include "shader/BlurShader.h"
#include "Light.h"
#include "Entity.h"
#include "MatrixStack.h"
#include "Game.h"
#include "Sprite.h"
#include <shader/ShaderComponentNormals.h>
#include <shader/ComponentShaderBase.h>
#include <shader/ShaderComponentDepth.h>

#include <sweet/Input.h>
#include <MousePerspectiveCamera.h>

Scene::Scene(Game * _game):
	depthBuffer(new StandardFrameBuffer(true)),
	normalBuffer(new StandardFrameBuffer(true)),
	shadowBuffer(new StandardFrameBuffer(true)),
	depthShader(new ComponentShaderBase(true)),
	shadowShader(new BlurShader(true)),
	normalsShader(new ComponentShaderBase(true)),
	shadowSurface(new RenderSurface(shadowShader)),
	game(_game),
	mouse(&Mouse::getInstance()),
	//Singletons
	keyboard(&Keyboard::getInstance()),
	activeCamera(new MousePerspectiveCamera())
{

	cameras.push_back(activeCamera);
	childTransform->addChild(activeCamera);

	normalsShader->addComponent(new ShaderComponentNormals(normalsShader));
	normalsShader->compileShader();

	depthShader->addComponent(new ShaderComponentDepth(depthShader));
	depthShader->compileShader();

	++depthBuffer->referenceCount;
	++normalBuffer->referenceCount;
	++shadowBuffer->referenceCount;
	++depthShader->referenceCount;
	++shadowShader->referenceCount;
	++normalsShader->referenceCount;
	++shadowSurface->referenceCount;
}

Scene::~Scene(void){
	while(cameras.size() > 0){
		//delete cameras.back(); don't need to delete because they're children of the scene
		cameras.pop_back();
	}
	activeCamera = nullptr;

	while(lights.size() > 0){
		//delete lights.back(); don't need to delete because they're children of the scene
		lights.pop_back();
	}
	

	depthBuffer->decrementAndDelete();
	normalBuffer->decrementAndDelete();
	shadowBuffer->decrementAndDelete();
	depthShader->decrementAndDelete();
	shadowShader->decrementAndDelete();
	normalsShader->decrementAndDelete();
	shadowSurface->decrementAndDelete();
}

void Scene::update(Step * _step){
	if(keyboard->keyJustDown(GLFW_KEY_F12)){
		game->takeScreenshot();
	}
	Entity::update(_step);
}

void Scene::load(){
	if(!loaded){
		shadowSurface->load();
		depthShader->load();
		shadowBuffer->load();
		depthBuffer->load();
		normalsShader->load();
		normalBuffer->load();
	}
	Entity::load();
}

void Scene::unload(){
	if(loaded){
		depthBuffer->unload();
		shadowBuffer->unload();
		depthShader->unload();
		shadowSurface->unload();
		normalsShader->unload();
		normalBuffer->unload();
	}
	Entity::unload();
}


void Scene::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	// don't bother doing any work if we aren't rendering anyway
	if(!isVisible()){
		return;
	}

	// render options
	glEnable(GL_SCISSOR_TEST);
	if(_renderOptions->depthEnabled){
		glEnable(GL_DEPTH_TEST);
	}else{
		glDisable(GL_DEPTH_TEST);
	}if(_renderOptions->alphaEnabled){
		glEnable(GL_ALPHA_TEST);
		//glAlphaFunc ( GL_GREATER, 0.1 ) ;
		//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
	}else{
		glDisable(GL_ALPHA_TEST);
	}
	_renderOptions->lights = &lights;
	//Back-face culling
	//glEnable (GL_CULL_FACE); // cull face
	//glCullFace (GL_BACK); // cull back face
	//glFrontFace (GL_CW); // GL_CCW for counter clock-wise, GL_CW for clock-wise


	// matrix stack
	_matrixStack->pushMatrix();
	_matrixStack->resetCurrentMatrix();
	_matrixStack->setCamera(activeCamera);

	// render
	Entity::render(_matrixStack, _renderOptions);

	_matrixStack->popMatrix();

	checkForGlError(false);

	// clean the lights
	for(Light * l : lights){
		l->lightDirty = false;
	}
}

void Scene::renderDepth(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
	// Store a reference to the current override shader so we can restore it 
	Shader * backupOverride = _renderOptions->overrideShader;
	
	// Resize and bind the depth buffer
	depthBuffer->resize(game->viewPortWidth, game->viewPortHeight);
	depthBuffer->bindFrameBuffer();

	// Make sure the depth buffer is actually loaded so this will work
	if(!depthBuffer->loaded) {
		depthBuffer->load();
	}

	// Same for the depth shader
	if(!depthShader->loaded) {
		depthShader->load();
	}
	
	// Make the depth shader dirty so it updates it uniforms
	depthShader->makeDirty();

	// Set the override shader to the depth shader so we can render the depth
	_renderOptions->overrideShader = depthShader;
	
	// Render the scene using this shader 
	Scene::render(_matrixStack, _renderOptions);

	// Restore the previous override shader 
	_renderOptions->overrideShader = backupOverride;

	// Binf the main OpenGL buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::renderDepthBufferToSurface(RenderSurface* _renderSurface) {
	// resize and bind the depth buffer
	depthBuffer->resize(game->viewPortWidth, game->viewPortHeight);
	depthBuffer->bindFrameBuffer();

	// Make sure the depth buffer is actually loaded so this will work
	if(!depthBuffer->loaded) {
		depthBuffer->load();
	}
	
	// Render it to the surface 
	_renderSurface->render(depthBuffer->getTextureId());

	// Re-bind the main OpenGL buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::renderNormals(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
	// Store a reference to the current override shader so we can restore it 
	Shader * backupOverride = _renderOptions->overrideShader;
	
	// Resize and bind the normal buffer
	normalBuffer->resize(game->viewPortWidth, game->viewPortHeight);
	normalBuffer->bindFrameBuffer();

	// Make sure the normal buffer is actually loaded so this will work
	if(!normalBuffer->loaded) {
		normalBuffer->load();
	}

	// Same for the normal shader
	if(!normalsShader->loaded) {
		normalsShader->load();
	}
	
	// Make the normal shader dirty so it updates it uniforms
	normalsShader->makeDirty();

	// Set the override shader to the normal shader so we can render the normal
	_renderOptions->overrideShader = normalsShader;
	
	// Render the scene using this shader 
	Scene::render(_matrixStack, _renderOptions);

	// Restore the previous override shader 
	_renderOptions->overrideShader = backupOverride;

	// Binf the main OpenGL buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::renderNormalBufferToSurface(RenderSurface* _renderSurface) {
	// resize and bind the normal buffer
	normalBuffer->resize(game->viewPortWidth, game->viewPortHeight);
	normalBuffer->bindFrameBuffer();

	// Make sure the normal buffer is actually loaded so this will work
	if(!normalBuffer->loaded) {
		normalBuffer->load();
	}
	
	// Render it to the surface 
	_renderSurface->render(normalBuffer->getTextureId());

	// Re-bind the main OpenGL buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Scene::renderShadows(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
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

Camera * Scene::cycleCamera(){
	if(activeCamera == cameras.back()){
		activeCamera = cameras.at(0);
	}else{
		for(unsigned long int i = 0; i < cameras.size()-1; ++i){
			if(activeCamera == cameras.at(i)){
				activeCamera = cameras.at(i+1);
				break;
			}
		}
	}
	return activeCamera;
}