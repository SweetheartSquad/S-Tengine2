#pragma once

#include <SceneSplash.h>

#include <algorithm>

#include <Game.h>

#include <MeshEntity.h>
#include <MeshInterface.h>
#include <MeshFactory.h>
#include <Resource.h>
#include <Easing.h>

#include <shader\BaseComponentShader.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentDiffuse.h>
#include <shader\ShaderComponentHsv.h>

#include <FollowCamera.h>
#include <MousePerspectiveCamera.h>

#include <Sprite.h>
#include <PointLight.h>
#include <Texture.h>

#include <Sound.h>
#include <libzplay.h>

#include <System.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <GLFW\glfw3.h>
#include <MatrixStack.h>

#include <RenderSurface.h>
#include <StandardFrameBuffer.h>
#include <NumberUtils.h>

SceneSplash::SceneSplash(Game * _game) :
	Scene(_game),
	shader(new BaseComponentShader(true)),
	screenSurfaceShader(new Shader("../assets/RenderSurface", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader)),
	screenFBO(new StandardFrameBuffer(true)),
	uiLayer(0,0,0,0)
{
	shader->addComponent(new ShaderComponentTexture(shader));
	//hsvShader = new ShaderComponentHsv(shader, 0, 1, 1);
	//shader->addComponent(hsvShader);
	shader->compileShader();

	Transform * t;

	//Set up cameras
	PerspectiveCamera * fc = new PerspectiveCamera();
	t = new Transform();
	t->addChild(fc);
	cameras.push_back(fc);
	fc->farClip = 1000.f;
	fc->nearClip = 0.001f;
	fc->parents.at(0)->rotate(90, 0, 1, 0, kWORLD);
	fc->parents.at(0)->translate(-18.5f, -2.0f, 17.36f);
	fc->yaw = 60.0f;
	fc->pitch = -4.0f;
	fc->lastOrientation = fc->calcOrientation();
	fc->parents.at(0)->setOrientation(fc->lastOrientation);
	activeCamera = fc;

	Texture * c = new Texture("../assets/cursor.png", 32, 32, true, false);
	c->load();
	ResourceManager::resources.push_back(c);
	Texture * logoTex = new Texture("../assets/S-Tengine2_logo.png", 1024, 1024, true, false);
	logoTex->load();
	ResourceManager::resources.push_back(logoTex);
	
	mouseIndicator = new Sprite();
	uiLayer.childTransform->addChild(mouseIndicator);
	mouseIndicator->mesh->pushTexture2D(c);
	mouseIndicator->parents.at(0)->scale(16,16,1);
	mouseIndicator->setShader(uiLayer.shader, true);

	for(unsigned long int i = 0; i < mouseIndicator->mesh->vertices.size(); ++i){
		mouseIndicator->mesh->vertices[i].x -= 1.25;
		mouseIndicator->mesh->vertices[i].y -= 1;
	}
	mouseIndicator->mesh->dirty = true;
	
	logo = new MeshEntity(Resource::loadMeshFromObj("../assets/S-Tengine2_logo.vox").at(0));
	childTransform->addChild(logo);
	logo->mesh->pushTexture2D(logoTex);
	logo->parents.at(0)->scale(25,25,25);
	logo->setShader(shader, true);
}

SceneSplash::~SceneSplash(){
	shader->safeDelete();
	screenSurface->safeDelete();
	//screenSurfaceShader->safeDelete();
	screenFBO->safeDelete();
}

void SceneSplash::update(Step * _step){
	/*float b;
	if(_step->time-2 < 2){
		b = Easing::easeInQuad(std::max(0.0, _step->time-2), 0, 1, 2) - std::rand() % 100 / 1000.f;
	}else{
		b = Easing::easeOutQuad(std::max(0.0, _step->time - 8), 1, -1, 1) - std::rand() % 100 / 1000.f;
	}

	float thing = std::rand() % 100 / 10000.f;

	//glUniform1f(glGetUniformLocation(screenSurfaceShader->getProgramId(), "amount"), std::rand() % 100 / 1000.f);
	//glUniform1f(glGetUniformLocation(screenSurfaceShader->getProgramId(), "center"), 0.5f);

	if (_step->time <= 6){
		screenSurface->vertices.at(0).v = thing + Easing::easeOutCubic(std::min(5.0, _step->time), 0, -50, 5) + 1;
		screenSurface->vertices.at(1).v = thing + Easing::easeOutCubic(std::min(5.0, _step->time), 0, -50, 5) + 1;
		screenSurface->vertices.at(2).v = thing + Easing::easeOutCubic(std::min(5.0, _step->time), 0, -50, 5);
		screenSurface->vertices.at(3).v = thing + Easing::easeOutCubic(std::min(5.0, _step->time), 0, -50, 5);
	}else{
		screenSurface->vertices.at(0).v = thing + Easing::easeInCubic(std::max(0.0, _step->time - 7), 0, -25, 2) + 1;
		screenSurface->vertices.at(1).v = thing + Easing::easeInCubic(std::max(0.0, _step->time - 7), 0, -25, 2) + 1;
		screenSurface->vertices.at(2).v = thing + Easing::easeInCubic(std::max(0.0, _step->time - 7), 0, -25, 2);
		screenSurface->vertices.at(3).v = thing + Easing::easeInCubic(std::max(0.0, _step->time - 7), 0, -25, 2);
	}
	screenSurface->dirty = true;

	hsvShader->setValue(b);*/

	if(_step->time > 9){
		game->switchScene(nextScene, true);
	}

	if(keyboard->keyJustUp(GLFW_KEY_F11)){
		game->toggleFullScreen();
	}

	if(keyboard->keyJustUp(GLFW_KEY_ENTER)){
		game->switchScene(nextScene, true);
	}
	
	// camera controls
	if (keyboard->keyDown(GLFW_KEY_UP)){
		activeCamera->parents.at(0)->translate((activeCamera->forwardVectorRotated) * static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_DOWN)){
		activeCamera->parents.at(0)->translate((activeCamera->forwardVectorRotated) * -static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_LEFT)){
		activeCamera->parents.at(0)->translate((activeCamera->rightVectorRotated) * -static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_RIGHT)){
		activeCamera->parents.at(0)->translate((activeCamera->rightVectorRotated) * static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}

	Scene::update(_step);

	glm::uvec2 sd = vox::getScreenDimensions();
	uiLayer.resize(0.f, sd.x, 0.f, sd.y);
	uiLayer.update(_step);
	
	mouseIndicator->parents.at(0)->translate(sd.x - mouse->mouseX(), sd.y - mouse->mouseY(), 0.f, false);
}

void SceneSplash::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	clear();
	screenFBO->resize(game->viewPortWidth, game->viewPortHeight);
	//Bind frameBuffer
	screenFBO->bindFrameBuffer();
	//render the scene to the buffer
	Scene::render(_matrixStack, _renderOptions);
	//Render the buffer to the render surface
	screenSurface->render(screenFBO->getTextureId());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	uiLayer.render(_matrixStack, _renderOptions);
	
	game->setViewport(0.f, 0.f, game->viewPortWidth*0.5f, game->viewPortHeight*0.5f);
	Scene::render(_matrixStack, _renderOptions);
	uiLayer.render(_matrixStack, _renderOptions);
	game->setViewport(0.f, 0.f, game->viewPortWidth*2.f, game->viewPortHeight*2.f);
}

void SceneSplash::load(){
	Scene::load();	

	screenSurface->load();
	screenFBO->load();
	uiLayer.load();
}

void SceneSplash::unload(){
	uiLayer.unload();
	screenFBO->unload();
	screenSurface->unload();

	Scene::unload();	
}