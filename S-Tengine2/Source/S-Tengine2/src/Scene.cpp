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
#include <shader/NormalsShader.h>

// for screenshots
#include <SOIL.h>
#include <ctime>

Scene::Scene(Game * _game):
	game(_game),
	mouse(&Mouse::getInstance()),
	keyboard(&Keyboard::getInstance()),
	activeCamera(new PerspectiveCamera()),
	depthBuffer(new StandardFrameBuffer(true)),
	shadowBuffer(new StandardFrameBuffer(true)),
	normalBuffer(new StandardFrameBuffer(true)),
	depthShader(new DepthMapShader(true)),
	normalsShader(new NormalsShader(true)),
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

	/*while(lights.size() > 0){
		delete lights.back();
		lights.pop_back();
	}*/
	
	depthBuffer->safeDelete();
	shadowBuffer->safeDelete();
	depthShader->safeDelete();
	delete shadowSurface;
}

void Scene::update(Step * _step){
	
	if(keyboard->keyJustUp(GLFW_KEY_F11)){
		game->toggleFullScreen();
	}

	if(keyboard->keyJustDown(GLFW_KEY_F12)){
		std::stringstream filepath;
		
		// create a string with the format "../screenshots/YYYY-MM-DD_TTTTTTTTTT.tga"
		time_t t = time(0);
		struct tm now;
		localtime_s(&now, &t);
		filepath
			<< "../screenshots/"
			<< (now.tm_year + 1900)
			<< '-'
			<< (now.tm_mon + 1)
			<< '-'
			<< now.tm_mday
			<< '_'
			<< t
			<< ".tga";
		SOIL_save_screenshot(filepath.str().c_str(), SOIL_SAVE_TYPE_TGA, 0, 0, game->viewPortWidth, game->viewPortHeight);
	}

	activeCamera->update(_step);
	Entity::update(_step);
}

void Scene::load(){
	shadowSurface->load();
	depthShader->load();
	shadowBuffer->load();
	depthBuffer->load();
	Entity::load();
}

void Scene::unload(){
	depthBuffer->unload();
	shadowBuffer->unload();
	depthShader->unload();
	shadowSurface->unload();
	Entity::unload();
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

	const glm::mat4 * p = &activeCamera->getProjectionMatrix();
	const glm::mat4 * v = &activeCamera->getViewMatrix();

	_matrixStack->pushMatrix();
	_matrixStack->resetCurrentMatrix();
	_matrixStack->setProjectionMatrix(p);
	_matrixStack->setViewMatrix(v);

	_renderOptions->lights = &lights;
	
	clear();
	Entity::render(_matrixStack, _renderOptions);

	_matrixStack->popMatrix();

	checkForGlError(0,__FILE__,__LINE__);
}

void Scene::renderDepth(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
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

void Scene::renderNormals(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
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