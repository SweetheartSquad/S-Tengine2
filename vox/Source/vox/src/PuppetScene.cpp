#pragma once

#include "PuppetScene.h"

#include "Texture.h"
#include "Sprite.h"
#include "shader/BaseComponentShader.h"
#include "shader/TextureShaderComponent.h"
#include "Keyboard.h"
#include "SoundManager.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "ControllableOrthographicCamera.h"
#include "RenderOptions.h"
#include "MeshEntity.h"

#include <array>
#include <Box2D/Box2D.h>
#include <Box2DDebugDraw.h>
#include "Box2DMeshEntity.h"
#include "MeshFactory.h"
#include "PerspectiveCamera.h"
#include "MousePerspectiveCamera.h"
#include "BitmapFont.h"
#include "TestCharacter.h"
#include "CharacterComponent.h"
#include "Catapult.h"
#include "GameJamContactListener.h"
#include "Game.h"
#include <Arduino.h>
#include <AccelerometerParser.h>
#include <Accelerometer.h>

#include <PuppetCharacter.h>
#include <PuppetController.h>

PuppetScene::PuppetScene(Game * _game):
	Scene(_game),
	cl(new GameJamContactListener),
	debugDraw(true),
	world(new Box2DWorld(b2Vec2(0, -9.8))),
	playerCharacter(new PuppetCharacter(world, PLAYER, STRUCTURE | ITEM | PLAYER, false)),
	ground(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody)),
	background(new MeshEntity(MeshFactory::getPlaneMesh())),
	shader(new BaseComponentShader()),
	soundManager(new SoundManager()),
	mouseCam(false)
{
	world->b2world->SetContactListener(cl);
	shader->components.push_back(new TextureShaderComponent());
	shader->compileShader();
	renderOptions->alphaSorting = true;
	ground->setShader(shader, true);
	ground->setTranslationPhysical(0, 0, 0);
	ground->transform->rotate(90.f, 1, 0, 0, kOBJECT);
	ground->transform->rotate(90.f, 0, 0, 1, kOBJECT);
	ground->transform->scale(250, 250, 1);
	ground->mesh->dirty = true;
	ground->mesh->uvEdgeMode = GL_REPEAT;
	ground->mesh->pushTexture2D(new Texture("../assets/hurly-burly/StageFloor.png", 1024, 1024, true, true));
	ground->body->SetTransform(b2Vec2(0, -250), 0);

	//Set UVs so the texture isn't stetched
	ground->mesh->setUV(0, 0.0,  0.0);
	ground->mesh->setUV(1, 0.0,  40.0);
	ground->mesh->setUV(2, 40.0, 40.0);
	ground->mesh->setUV(3, 40.0, 0.0);

	world->addToWorld(ground, 2);
	addChild(ground, false);

	background->setShader(shader, true);
	background->transform->translate(0.0f, 250.f, -10.0f);
	background->transform->scale(250, 250, 1);
	background->mesh->pushTexture2D(new Texture("../assets/hurly-burly/Sky.png", 1024, 1024, true, true));
	background->mesh->uvEdgeMode = GL_REPEAT;
	background->mesh->dirty = true;

	addChild(background);
	
	world->b2world->SetDebugDraw(drawer);
	//drawer->AppendFlags(b2Draw::e_aabbBit);
	drawer->AppendFlags(b2Draw::e_shapeBit);
	drawer->AppendFlags(b2Draw::e_centerOfMassBit);
	drawer->AppendFlags(b2Draw::e_jointBit);
	addChild(drawer, false);

	playerCharacter->setShader(shader, true);
	addChild(playerCharacter, true);
	playerCharacter->addToScene(this);
	playerCharacter->head->maxVelocity = b2Vec2(10, 10);

	TestCharacter * michael = new TestCharacter(world, false, PLAYER, STRUCTURE | ITEM | PLAYER);
	michael->setShader(shader, true);
	addChild(michael, true);
	michael->addToScene(this);
	michael->translateComponents(glm::vec3(1,0,0));
	
	Catapult * catapult = new Catapult(world, STRUCTURE, PLAYER);
	catapult->setShader(shader, true);
	addChild(catapult, true);
	catapult->addToScene(this);
	catapult->translateComponents(glm::vec3(1,0,0));

	//Arduino 
	arduino = new AccelerometerParser("COM3");
	Accelerometer * acc = new Accelerometer(arduino);
	arduino->addAccelerometer(acc);
	
	puppetController = new PuppetController(acc, playerCharacter);
	perspectiveCamera = new PerspectiveCamera(playerCharacter->torso, glm::vec3(0, 0, 0), 0, 0);
	
	//Set up cameras
	perspectiveCamera->farClip = 1000.f;
	perspectiveCamera->transform->rotate(90, 0, 1, 0, kWORLD);
	perspectiveCamera->transform->translate(5.0f, 5.f, 15.0f);
	perspectiveCamera->yaw = 90.0f;
	perspectiveCamera->pitch = -10.0f;
	
	mouseCamera = new MousePerspectiveCamera();

	mouseCamera->farClip = 1000.f;
	mouseCamera->transform->rotate(90, 0, 1, 0, kWORLD);
	mouseCamera->transform->translate(5.0f, 5.f, 15.0f);
	mouseCamera->yaw = 90.0f;
	mouseCamera->pitch = -10.0f;

	camera =  perspectiveCamera;
}

PuppetScene::~PuppetScene(){
}

void PuppetScene::load(){
	Scene::load();
}

void PuppetScene::unload(){
	Scene::unload();
}

void PuppetScene::update(Step * _step){
	
	Scene::update(_step);
	world->update(_step);

	arduino->update(_step);
	puppetController->update(_step);

	if(keyboard->keyJustDown(GLFW_KEY_W)){
		if(playerCharacter->head->body->GetPosition().y < 12){
			playerCharacter->head->applyLinearImpulseUp(2500);
		}
	}

	if(keyboard->keyJustUp(GLFW_KEY_1)){
		mouseCam = !mouseCam;
		if(!mouseCam){
			camera = perspectiveCamera;
		}else{
			camera = mouseCamera;			
		}
	}

	if(keyboard->keyDown(GLFW_KEY_A)){
		playerCharacter->head->applyLinearImpulseLeft(25);
		if(playerCharacter->transform->scaleVector.x < 0){
			playerCharacter->transform->scaleX(-1);
		}
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		playerCharacter->head->applyLinearImpulseRight(25);
		if(playerCharacter->transform->scaleVector.x > 0){
			playerCharacter->transform->scaleX(-1);
		}
	}

	// camera controls
	if(keyboard->keyDown(GLFW_KEY_UP)){
		camera->transform->translate((camera->forwardVectorRotated) * static_cast<MousePerspectiveCamera *>(camera)->speed);
	}
	if(keyboard->keyDown(GLFW_KEY_DOWN)){
		camera->transform->translate((camera->forwardVectorRotated) * -static_cast<MousePerspectiveCamera *>(camera)->speed);	
	}
	if(keyboard->keyDown(GLFW_KEY_LEFT)){
		camera->transform->translate((camera->rightVectorRotated) * -static_cast<MousePerspectiveCamera *>(camera)->speed);		
	}
	if(keyboard->keyDown(GLFW_KEY_RIGHT)){
		camera->transform->translate((camera->rightVectorRotated) * static_cast<MousePerspectiveCamera *>(camera)->speed);	
	}
	if(keyboard->keyJustUp(GLFW_KEY_F11)){
		game->toggleFullScreen();
	}
	if(keyboard->keyJustUp(GLFW_KEY_F1)){
		debugDraw = !debugDraw;
	}

	if(keyboard->keyJustUp(GLFW_KEY_F11)){
		debugDraw = !debugDraw;
	}
}

void PuppetScene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Scene::render(_matrixStack, _renderStack);
}