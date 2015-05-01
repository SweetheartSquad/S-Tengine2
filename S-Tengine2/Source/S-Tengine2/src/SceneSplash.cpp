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
	shader->components.push_back(new ShaderComponentTexture(shader));
	hsvShader = new ShaderComponentHsv(shader, 0, 1, 1);
	shader->components.push_back(hsvShader);
	shader->compileShader();

	//Set up cameras
	PerspectiveCamera * fc = new PerspectiveCamera();
	cameras.push_back(fc);
	fc->farClip = 1000.f;
	fc->nearClip = 0.001f;
	fc->transform->rotate(90, 0, 1, 0, kWORLD);
	fc->transform->translate(5.0f, 1.5f, 22.5f);
	fc->yaw = 90.0f;
	fc->pitch = -10.0f;
	activeCamera = fc;


	Texture * c = new Texture("../assets/cursor.png", 32, 32, true, false);
	c->load();
	ResourceManager::resources.push_back(c);
	
	mouseIndicator = new Sprite();
	mouseIndicator->mesh->pushTexture2D(c);
	mouseIndicator->transform->scale(16,16,1);
	uiLayer.addChild(mouseIndicator);


	for(unsigned long int i = 0; i < mouseIndicator->mesh->vertices.size(); ++i){
		mouseIndicator->mesh->vertices[i].x -= 1.25;
		mouseIndicator->mesh->vertices[i].y -= 1;
	}
	mouseIndicator->mesh->dirty = true;
	
	Texture * t = new Texture("../assets/S-Tengine2_logo.png", 2048, 2048, true, true);
	t->load();
	logo = new MeshEntity(Resource::loadMeshFromObj("../assets/S-Tengine2_logo.vox").at(0));
	logo->mesh->pushTexture2D(t);
	logo->transform->scale(25,25,25);
	logo->setShader(shader, true);
	addChild(logo);
}

SceneSplash::~SceneSplash(){
	while(children.size() > 0){
		NodeHierarchical::deleteRecursively(children.back());
		children.pop_back();
	}
	
	shader->safeDelete();
	screenSurface->safeDelete();
	//screenSurfaceShader->safeDelete();
	screenFBO->safeDelete();
}

void SceneSplash::update(Step * _step){
	float b;
	if(_step->time-3 < 3){
		b = Easing::easeInQuad(std::max(0.0, _step->time-3), 0, 1, 3);
	}else{
		b = Easing::easeOutQuad(std::max(0.0, _step->time-8), 1, -1, 1);
	}

	hsvShader->setValue(b);

	if(_step->time-3 > 6){
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
		activeCamera->transform->translate((activeCamera->forwardVectorRotated) * static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_DOWN)){
		activeCamera->transform->translate((activeCamera->forwardVectorRotated) * -static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_LEFT)){
		activeCamera->transform->translate((activeCamera->rightVectorRotated) * -static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_RIGHT)){
		activeCamera->transform->translate((activeCamera->rightVectorRotated) * static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}

	Scene::update(_step);

	glm::uvec2 sd = vox::getScreenDimensions();
	uiLayer.resize(0, sd.x, 0, sd.y);
	uiLayer.update(_step);
	
	mouseIndicator->transform->translate(sd.x - mouse->mouseX(), sd.y - mouse->mouseY(), 0.f, false);
}

void SceneSplash::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	//screenFBO->resize(game->viewPortWidth, game->viewPortHeight);
	//Bind frameBuffer
	//screenFBO->bindFrameBuffer();
	//render the scene to the buffer
	Scene::render(_matrixStack, _renderOptions);
	uiLayer.render(_matrixStack, _renderOptions);
	//Render the buffer to the render surface
	//screenSurface->render(screenFBO->getTextureId());
}

void SceneSplash::load(){
	Scene::load();	

	screenSurface->load();
	screenFBO->load();
	uiLayer.load();
}

void SceneSplash::unload(){
	Scene::unload();	

	screenSurface->unload();
	screenFBO->unload();
	uiLayer.unload();
}