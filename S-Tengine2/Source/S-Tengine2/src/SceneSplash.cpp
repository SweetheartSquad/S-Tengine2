#pragma once

#include <SceneSplash.h>

#include <algorithm>

#include <Game.h>

#include <MeshEntity.h>
#include <MeshInterface.h>
#include <MeshFactory.h>
#include <Resource.h>
#include <Easing.h>

#include <shader\ComponentShaderBase.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentDiffuse.h>
#include <shader\ShaderComponentHsv.h>
#include <shader\ShaderComponentMVP.h>

#include <FollowCamera.h>
#include <MousePerspectiveCamera.h>

#include <Sprite.h>
#include <PointLight.h>
#include <Texture.h>

#include <System.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <GLFW\glfw3.h>
#include <MatrixStack.h>
#include <RenderOptions.h>

#include <RenderSurface.h>
#include <StandardFrameBuffer.h>
#include <NumberUtils.h>

#include <Font.h>
#include <shader\ShaderComponentText.h>

SceneSplash::SceneSplash(Game * _game) :
	Scene(_game),
	shader(new ComponentShaderBase(true)),
	screenSurfaceShader(new Shader("assets/engine basics/DefaultRenderSurface", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader)),
	screenFBO(new StandardFrameBuffer(true)),
	hsvComponent(new ShaderComponentHsv(shader, 0, 1, 1)),
	uiLayer(0,0,0,0),
	t(0.f)
{
	shader->addComponent(new ShaderComponentMVP(shader));
	shader->addComponent(new ShaderComponentTexture(shader));
	shader->addComponent(hsvComponent);
	shader->compileShader();

	//Set up cameras
	PerspectiveCamera * fc = new PerspectiveCamera();
	childTransform->addChild(fc);
	cameras.push_back(fc);
	fc->farClip = 1000.f;
	fc->nearClip = 0.001f;
	fc->childTransform->rotate(90, 0, 1, 0, kWORLD);
	fc->parents.at(0)->translate(-18.5f, -2.0f, 17.36f);
	fc->yaw = 60.0f;
	fc->pitch = -4.0f;
	fc->lastOrientation = fc->calcOrientation();
	fc->childTransform->setOrientation(fc->lastOrientation);
	activeCamera = fc;

	Texture * c = new Texture("assets/engine basics/cursor.png", true, false);
	c->load();
	ResourceManager::resources.push_back(c);
	Texture * logoTex = new Texture("assets/engine basics/S-Tengine2_logo-tex.png", true, false);
	logoTex->load();
	ResourceManager::resources.push_back(logoTex);
	
	mouseIndicator = new Sprite();
	uiLayer.childTransform->addChild(mouseIndicator);
	mouseIndicator->mesh->pushTexture2D(c);
	mouseIndicator->parents.at(0)->scale(32,32,1);
	mouseIndicator->setShader(uiLayer.shader, true);

	for(unsigned long int i = 0; i < mouseIndicator->mesh->vertices.size(); ++i){
		mouseIndicator->mesh->vertices[i].x += 0.5f;
		mouseIndicator->mesh->vertices[i].y -= 0.5f;
	}
	mouseIndicator->mesh->dirty = true;
	
	logo = new MeshEntity(Resource::loadMeshFromObj("assets/engine basics/S-Tengine2_logo.obj").at(0));
	childTransform->addChild(logo)->scale(25);
	logo->mesh->pushTexture2D(logoTex);
	logo->setShader(shader, true);



	ComponentShaderBase * textShader = new ComponentShaderBase(true);
	textShader->addComponent(new ShaderComponentText(textShader));
	textShader->compileShader();

	/*Font * font = new Font("assets/arial.ttf", 50, false);
	textThing = new Label(font, textShader, shader, WrapMode::WORD_WRAP, -1);
	textThing->setText(L"some text");
	uiLayer.childTransform->addChild(textThing);*/
}

SceneSplash::~SceneSplash(){
	shader->safeDelete();
	screenSurface->safeDelete();
	//screenSurfaceShader->safeDelete();
	screenFBO->safeDelete();
}

void SceneSplash::update(Step * _step){
	float b;
	// how much time to show just black at the start
	t += _step->deltaTime;
	float buffer = 0;
	if(t-buffer < 4){
		b = Easing::easeInQuint(std::max(0.f, t-buffer), 0, 1, 4) - std::rand() % 100 / 1000.f;
	}else{
		b = Easing::easeOutQuad(std::max(0.f, t-buffer - 9.5f), 1, -1, 1) - std::rand() % 100 / 1000.f;
	}

	float thing = std::rand() % 50 / 10000.f;

	if (t-buffer <= 7){
		screenSurface->vertices.at(0).v = thing + Easing::easeOutCubic(std::min(6.f, t-buffer), 0, -50, 6) + 1;
		screenSurface->vertices.at(1).v = thing + Easing::easeOutCubic(std::min(6.f, t-buffer), 0, -50, 6) + 1;
		screenSurface->vertices.at(2).v = thing + Easing::easeOutCubic(std::min(6.f, t-buffer), 0, -50, 6);
		screenSurface->vertices.at(3).v = thing + Easing::easeOutCubic(std::min(6.f, t-buffer), 0, -50, 6);
	}else{
		screenSurface->vertices.at(0).v = thing + Easing::easeInQuint(std::max(0.f, t-buffer - 8), 0, -25, 2.5) + 1;
		screenSurface->vertices.at(1).v = thing + Easing::easeInQuint(std::max(0.f, t-buffer - 8), 0, -25, 2.5) + 1;
		screenSurface->vertices.at(2).v = thing + Easing::easeInQuint(std::max(0.f, t-buffer - 8), 0, -25, 2.5);
		screenSurface->vertices.at(3).v = thing + Easing::easeInQuint(std::max(0.f, t-buffer - 8), 0, -25, 2.5);
	}
	screenSurface->dirty = true;

	hsvComponent->setValue(b);

	if(t-buffer > 10.5){
		game->switchScene(nextScene, true);
	}


	if(keyboard->keyJustUp(GLFW_KEY_ENTER)){
		game->switchScene(nextScene, true);
	}
	
	float speed = 1;
	MousePerspectiveCamera * cam = dynamic_cast<MousePerspectiveCamera *>(activeCamera);
	if(cam != nullptr){
		speed = cam->speed;
	}
	// camera controls
	if (keyboard->keyDown(GLFW_KEY_UP)){
		activeCamera->parents.at(0)->translate((activeCamera->forwardVectorRotated) * speed);
	}
	if (keyboard->keyDown(GLFW_KEY_DOWN)){
		activeCamera->parents.at(0)->translate((activeCamera->forwardVectorRotated) * -speed);
	}
	if (keyboard->keyDown(GLFW_KEY_LEFT)){
		activeCamera->parents.at(0)->translate((activeCamera->rightVectorRotated) * -speed);
	}
	if (keyboard->keyDown(GLFW_KEY_RIGHT)){
		activeCamera->parents.at(0)->translate((activeCamera->rightVectorRotated) * speed);
	}

	Scene::update(_step);

	glm::uvec2 sd = sweet::getScreenDimensions();
	uiLayer.resize(0.f, sd.x, 0.f, sd.y);
	uiLayer.update(_step);
	
	mouseIndicator->parents.at(0)->translate(mouse->mouseX(), mouse->mouseY(), 0.f, false);
	//textThing->parents.at(0)->translate(game->viewPortWidth*0.5f, game->viewPortHeight*0.8f, 0, false);
}

void SceneSplash::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	_renderOptions->clear();
	screenFBO->resize(game->viewPortWidth, game->viewPortHeight);
	//Bind frameBuffer
	screenFBO->bindFrameBuffer();
	//render the scene to the buffer
	Scene::render(_matrixStack, _renderOptions);
	//Render the buffer to the render surface
	screenSurface->render(screenFBO->getTextureId());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	uiLayer.render(_matrixStack, _renderOptions);
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