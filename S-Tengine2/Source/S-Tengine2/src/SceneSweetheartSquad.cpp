#pragma once

#include <SceneSweetheartSquad.h>

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
#include <shader\ShaderComponentPhong.h>
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

#include <BulletWorld.h>
#include <BulletMeshEntity.h>

SceneSweetheartSquad::SceneSweetheartSquad(Game * _game) :
	Scene(_game),
	diffuseShader(new ComponentShaderBase(true)),
	phongShader(new ComponentShaderBase(true)),
	screenSurfaceShader(new Shader("assets/engine basics/DefaultRenderSurface", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader)),
	screenFBO(new StandardFrameBuffer(true)),
	hsvComponent(new ShaderComponentHsv(diffuseShader, 0, 1, 1)),
	uiLayer(0,0,0,0),
	t(0.f)
{
	diffuseShader->addComponent(new ShaderComponentMVP(diffuseShader));
	diffuseShader->addComponent(new ShaderComponentTexture(diffuseShader));
	diffuseShader->addComponent(new ShaderComponentDiffuse(diffuseShader));
	diffuseShader->addComponent(hsvComponent);
	diffuseShader->compileShader();
	phongShader->addComponent(new ShaderComponentTexture(phongShader));
	phongShader->addComponent(new ShaderComponentPhong(phongShader));
	phongShader->compileShader();

	//Set up cameras
	PerspectiveCamera * fc = new PerspectiveCamera();
	childTransform->addChild(fc);
	cameras.push_back(fc);
	fc->farClip = 1000.f;
	fc->nearClip = 0.001f;
	fc->childTransform->rotate(90, 0, 1, 0, kWORLD);
	fc->parents.at(0)->translate(0.1f, -5.f, 18.f);
	fc->yaw = 90.0f;
	fc->pitch = 15.0f;
	fc->lastOrientation = fc->calcOrientation();
	fc->childTransform->setOrientation(fc->lastOrientation);
	activeCamera = fc;

	Texture * c = new Texture("assets/engine basics/cursor.png", true, false);
	c->load();
	ResourceManager::resources.push_back(c);
	Texture * logoTex = new Texture("assets/engine basics/sweetheartsquad-logo-tex.png", true, false);
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
	
	logo = new Entity();
	childTransform->addChild(logo)->scale(10);
	logo->parents.at(0)->translate(0, 10, 0);

	logo1 = new MeshEntity(Resource::loadMeshFromObj("assets/engine basics/sweetheartsquad-logo-noheart.obj").at(0));
	logo1->mesh->pushTexture2D(logoTex);
	logo1->setShader(diffuseShader, true);

	logo2 = new MeshEntity(Resource::loadMeshFromObj("assets/engine basics/sweetheartsquad-logo-heartonly.obj").at(0));
	logo2->mesh->pushTexture2D(logoTex);
	logo2->setShader(diffuseShader, true);
	
	logo->childTransform->addChild(logo1, false);
	logo->childTransform->addChild(logo2, false);
	logo->childTransform->translate(0, -0.9f, 0);



	ComponentShaderBase * textShader = new ComponentShaderBase(true);
	textShader->addComponent(new ShaderComponentText(textShader));
	textShader->compileShader();

	/*Font * font = new Font("assets/arial.ttf", 50, false);
	textThing = new Label(font, textShader, shader, WrapMode::WORD_WRAP, -1);
	textThing->setText(L"some text");
	uiLayer.childTransform->addChild(textThing);*/

	PointLight * light = new PointLight(glm::vec3(1,1,1), 0.5f, 0.001f, -1);
	lights.push_back(light);
	childTransform->addChild(light)->translate(0, 10, 0);
}

SceneSweetheartSquad::~SceneSweetheartSquad(){
	diffuseShader->safeDelete();
	phongShader->safeDelete();
	screenSurface->safeDelete();
	//screenSurfaceShader->safeDelete();
	screenFBO->safeDelete();
}

void SceneSweetheartSquad::update(Step * _step){
	t += _step->deltaTime * 2;
	// how much time to show just black at the start
	float buffer = 1;
	float a = 0.f, b = 1.f, c = 0.f;
		a = Easing::easeOutElastic(std::max(0.f, t - buffer), -30, 30, 3.0f, 1.2f, -1.f, 1.02f);
		b = Easing::easeInOutQuint(std::max(0.f, t - buffer), 0, 1, 0.75f);
		
	logo->parents.at(0)->setOrientation(glm::quat(glm::vec3(glm::radians(a*3.f + c),0,0)));
	hsvComponent->setValue(std::min(1.f, b));
	hsvComponent->setSaturation(std::min(1.f, b));

	if(t - buffer > 4){
		c = Easing::easeInBack(std::max(0.f, t - buffer - 4), 0, -10, 1.5, 0.75f);
	}
	logo->parents.at(0)->translate(0, a + c + 10, (a + c)/3.f, false);


	if(t > 8){
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

void SceneSweetheartSquad::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
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

void SceneSweetheartSquad::load(){
	Scene::load();	

	screenSurface->load();
	screenFBO->load();
	uiLayer.load();
}

void SceneSweetheartSquad::unload(){
	uiLayer.unload();
	screenFBO->unload();
	screenSurface->unload();

	Scene::unload();	
}