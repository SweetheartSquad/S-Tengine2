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
#include "RaidTheCastleContactListener.h"
#include "Game.h"
#include <Arduino.h>
#include <AccelerometerParser.h>
#include <Accelerometer.h>

#include <PuppetCharacter.h>
#include <PuppetController.h>

PuppetScene::PuppetScene(Game * _game):
	LayeredScene(_game, 3),
	cl(new RaidTheCastleContactListener),
	world(new Box2DWorld(b2Vec2(0, -9.8f * 2))),
	drawer(new Box2DDebugDraw(this, world)),
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
	
	background->setShader(shader, true);
	background->transform->translate(0.0f, 25.f, -10.0f);
	background->transform->scale(125, 25, 1);
	background->mesh->pushTexture2D(new Texture("../assets/hurly-burly/Sky.png", 1024, 1024, true, true));
	background->mesh->uvEdgeMode = GL_REPEAT;
	background->mesh->dirty = true;

	addChild(background, 0);

	
	ground->setShader(shader, true);
	ground->setTranslationPhysical(0, 0, 0);
	ground->transform->rotate(90.f, 1, 0, 0, kOBJECT);
	ground->transform->rotate(90.f, 0, 0, 1, kOBJECT);
	ground->transform->scale(250, 250, 1);
	ground->mesh->uvEdgeMode = GL_REPEAT;
	ground->mesh->pushTexture2D(new Texture("../assets/hurly-burly/StageFloor.png", 1024, 1024, true, true));
	ground->body->SetTransform(b2Vec2(0, -250), 0);
	ground->mesh->vertices.at(0).z -= 250;
	ground->mesh->vertices.at(1).z -= 250;
	ground->mesh->vertices.at(2).z -= 250;
	ground->mesh->vertices.at(3).z -= 250;
	ground->mesh->dirty = true;

	//Set UVs so the texture isn't stetched
	ground->mesh->setUV(0, 0.0,  0.0);
	ground->mesh->setUV(1, 0.0,  40.0);
	ground->mesh->setUV(2, 40.0, 40.0);
	ground->mesh->setUV(3, 40.0, 0.0);

	world->addToWorld(ground, 2);
	addChild(ground, 0);

	Texture * treeTex1 = new Texture("../assets/hurly-burly/Foliage/Tree1-ds.png", 1024, 1024, true, true);
	Texture * treeTex2 = new Texture("../assets/hurly-burly/Foliage/Tree2-ds.png", 1024, 1024, true, true);
	Texture * bushTex1 = new Texture("../assets/hurly-burly/Foliage/Bush1-ds.png", 1024, 1024, true, true);
	Texture * bushTex2 = new Texture("../assets/hurly-burly/Foliage/Bush2-ds.png", 1024, 1024, true, true);

	int numFoliage = std::rand()%500/50 + 10;
	for(signed long int i = 0; i < numFoliage; ++i){
		float height = std::rand()%500/50.f+5.f;
		MeshEntity * foliage = new MeshEntity(MeshFactory::getPlaneMesh());
		foliage->setShader(shader, true);
		foliage->transform->translate((std::rand()%500/10.f)-25.f, height, max(-9, -(float)(numFoliage-i)/numFoliage)*8.f - 1.f);
		foliage->transform->scale(height, height, 1);
		int tex = i % 4;
		switch(tex){
			case 0:
				foliage->mesh->pushTexture2D(treeTex1); break;
			case 1:
				foliage->mesh->pushTexture2D(treeTex2); break;
			case 2:
				foliage->mesh->pushTexture2D(bushTex1); break;
			case 3:
				foliage->mesh->pushTexture2D(bushTex2); break;
			default:
				break;
		}
		addChild(foliage, 0);
	}

	playerCharacter->setShader(shader, true);
	addChild(playerCharacter, 1);
	playerCharacter->addToLayeredScene(this, 1);
	playerCharacter->head->maxVelocity = b2Vec2(10, 10);

	TestCharacter * michael = new TestCharacter(world, false, PLAYER, STRUCTURE | ITEM | PLAYER);
	michael->setShader(shader, true);
	addChild(michael, 1);
	michael->addToLayeredScene(this, 1);
	michael->translateComponents(glm::vec3(1,0,0));
	
	catapult = new Catapult(world, STRUCTURE, PLAYER);
	catapult->setShader(shader, true);
	addChild(catapult, 1);
	catapult->addToLayeredScene(this, 1);

	catapult->translateComponents(glm::vec3(-10,0,0));

	//Arduino 
	arduino = new AccelerometerParser("COM4");
	Accelerometer * acc = new Accelerometer(arduino);
	arduino->addAccelerometer(acc);
	
	puppetController = new PuppetController(acc, playerCharacter);
	perspectiveCamera = new PerspectiveCamera(playerCharacter->torso, glm::vec3(0, 8.f, 0), 0, 0);
	
	//Set up cameras
	perspectiveCamera->farClip = 1000.f;
	perspectiveCamera->transform->rotate(90, 0, 1, 0, kWORLD);
	perspectiveCamera->transform->translate(5.0f, 1.5f, 22.5f);
	perspectiveCamera->yaw = 90.0f;
	perspectiveCamera->pitch = -10.0f;
	
	mouseCamera = new MousePerspectiveCamera();

	mouseCamera->farClip = 1000.f;
	mouseCamera->transform->rotate(90, 0, 1, 0, kWORLD);
	mouseCamera->transform->translate(5.0f, 1.5f, 22.5f);
	mouseCamera->yaw = 90.0f;
	mouseCamera->pitch = -10.0f;

	camera =  perspectiveCamera;

	
	world->b2world->SetDebugDraw(drawer);
	//drawer->AppendFlags(b2Draw::e_aabbBit);
	drawer->AppendFlags(b2Draw::e_shapeBit);
	drawer->AppendFlags(b2Draw::e_centerOfMassBit);
	drawer->AppendFlags(b2Draw::e_jointBit);
	addChild(drawer, 2);
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
		if(playerCharacter->torso->body->GetPosition().y < 8){
			float t = playerCharacter->torso->body->GetAngle();
			playerCharacter->torso->applyLinearImpulseUp(50*(1-sin(t)));
			if(playerCharacter->torso->body->GetAngle() > 0){
				playerCharacter->torso->applyLinearImpulseLeft(150*(1-cos(t)));
			}else{
				playerCharacter->torso->applyLinearImpulseRight(150*(1-cos(t)));
			}
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
	if(keyboard->keyJustUp(GLFW_KEY_2)){
		drawer->drawing = !drawer->drawing;
		/*if (drawer->drawing){
			drawer->load();
		}else{
			drawer->unload();
		}*/
	}
}

void PuppetScene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	LayeredScene::render(_matrixStack, _renderStack);
}