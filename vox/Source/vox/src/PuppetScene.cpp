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
#include "FollowCamera.h"
#include "MousePerspectiveCamera.h"
#include "BitmapFont.h"
#include "TestCharacter.h"
#include "CharacterComponent.h"
#include "RaidTheCastleContactListener.h"
#include "Game.h"
#include "PuppetGame.h"
#include <Arduino.h>
#include <AccelerometerParser.h>
#include <Accelerometer.h>

#include <PuppetCharacter.h>
#include <PuppetController.h>
#include <RandomGround.h>

#include "RaidTheCastle.h"

PuppetScene::PuppetScene(PuppetGame * _game, float seconds):
	LayeredScene(_game, 3),
	time(seconds * 6000),
	cl(new RaidTheCastleContactListener(this)),
	world(new Box2DWorld(b2Vec2(0.f, -98.0f))),
	drawer(new Box2DDebugDraw(this, world)),
	playerCharacter(new PuppetCharacter(world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR, -1, false)),
	playerCharacter2(new PuppetCharacter(world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR, -2, false)),
	playerCharacter3(new PuppetCharacter(world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR, -3, false)),
	playerCharacter4(new PuppetCharacter(world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR, -4, false)),
	ground(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody, false)),
	background(new MeshEntity(MeshFactory::getPlaneMesh())),
	shader(new BaseComponentShader()),
	soundManager(new SoundManager()),
	mouseCam(false),
	randomGround(new RandomGround(world, 100, 0.4f))
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

	int timeOfDayOptions = 4;
	int timeOfDay = std::rand()%timeOfDayOptions;
	background->mesh->setUV(0, (float)timeOfDay/timeOfDayOptions, 0);
	background->mesh->setUV(1, (float)(timeOfDay+1)/timeOfDayOptions, 0);
	background->mesh->setUV(2, (float)(timeOfDay+1)/timeOfDayOptions, 1);
	background->mesh->setUV(3, (float)timeOfDay/timeOfDayOptions, 1);

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

	//Set UVs so the texture isn't stretched
	ground->mesh->setUV(0, 0.0,  0.0);
	ground->mesh->setUV(1, 0.0,  40.0);
	ground->mesh->setUV(2, 40.0, 40.0);
	ground->mesh->setUV(3, 40.0, 0.0);

	world->addToWorld(ground, 2);
	addChild(ground, 0);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f * std::abs(ground->transform->scaleVector.x), 1.0f * std::abs(ground->transform->scaleVector.y));	
	b2Fixture * groundFixture = ground->getNewFixture(dynamicBox, 1.0f);
	groundFixture->SetSensor(false);
	groundFixture->SetUserData(this);
	b2Filter sf;
	sf.categoryBits = PuppetGame::kGROUND;
	sf.maskBits = PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER;
	groundFixture->SetFilterData(sf);

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
	playerCharacter->translateComponents(glm::vec3(0.0f, 5.f, 0.f));

	playerCharacter2->setShader(shader, true);
	addChild(playerCharacter2, 1);
	playerCharacter2->addToLayeredScene(this, 1);
	playerCharacter2->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter2->translateComponents(glm::vec3(5.0f, 5.f, 0.f));

	playerCharacter3->setShader(shader, true);
	addChild(playerCharacter3, 1);
	playerCharacter3->addToLayeredScene(this, 1);
	playerCharacter3->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter3->translateComponents(glm::vec3(10.0f, 5.f, 0.f));

	playerCharacter4->setShader(shader, true);
	addChild(playerCharacter4, 1);
	playerCharacter4->addToLayeredScene(this, 1);
	playerCharacter4->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter4->translateComponents(glm::vec3(15.0f, 5.f, 0.f));

	michael = new TestCharacter(world, false, PuppetGame::kPLAYER, PuppetGame::kSTRUCTURE | /*PuppetGame::kITEM | */PuppetGame::kPLAYER, -5);
	michael->setShader(shader, true);
	addChild(michael, 1);
	michael->addToLayeredScene(this, 1);

	michael->translateComponents(glm::vec3(1,50,0));

	//Arduino 
	arduino = new AccelerometerParser("COM4");

	Accelerometer * acc = new Accelerometer(arduino);
	arduino->addAccelerometer(acc);

	Accelerometer * acc2 = new Accelerometer(arduino);
	arduino->addAccelerometer(acc2);

	Accelerometer * acc3 = new Accelerometer(arduino);
	arduino->addAccelerometer(acc3);
	
	Accelerometer * acc4 = new Accelerometer(arduino);
	arduino->addAccelerometer(acc4);
	
	puppetController =  new PuppetController(acc, playerCharacter);
	puppetController2 = new PuppetController(acc2, playerCharacter2);
	puppetController3 = new PuppetController(acc3, playerCharacter3);
	puppetController4 = new PuppetController(acc4, playerCharacter4);


	
	
	
	world->b2world->SetDebugDraw(drawer);
	//drawer->AppendFlags(b2Draw::e_aabbBit);
	drawer->AppendFlags(b2Draw::e_shapeBit);
	drawer->AppendFlags(b2Draw::e_centerOfMassBit);
	drawer->AppendFlags(b2Draw::e_jointBit);
	//drawer->AppendFlags(b2Draw::e_pairBit);
	addChild(drawer, 2);

	randomGround->setShader(shader, true);
	randomGround->setTranslationPhysical(20.0f, 0.0f, 0.0f);
	//randomGround->mesh->uvEdgeMode = GL_REPEAT;
	randomGround->mesh->pushTexture2D(new Texture("../assets/paper.png", 512, 512, true, true));

	world->addToWorld(randomGround);
	addChild(randomGround, 1);
	
	//Set up cameras
	mouseCamera = new MousePerspectiveCamera();
	mouseCamera->farClip = 1000.f;
	mouseCamera->transform->rotate(90, 0, 1, 0, kWORLD);
	mouseCamera->transform->translate(5.0f, 1.5f, 22.5f);
	mouseCamera->yaw = 90.0f;
	mouseCamera->pitch = -10.0f;

	gameCam = new FollowCamera(glm::vec3(0, 0, 0), 0, 0);
	gameCam->addTarget(playerCharacter->torso);
	gameCam->addTarget(playerCharacter2->torso);
	gameCam->addTarget(playerCharacter3->torso);
	gameCam->addTarget(playerCharacter4->torso);
	gameCam->addTarget(michael->torso);
	gameCam->farClip = 1000.f;
	gameCam->transform->rotate(90, 0, 1, 0, kWORLD);
	gameCam->transform->translate(5.0f, 1.5f, 22.5f);
	gameCam->minimumZoom = 22.5f;
	gameCam->yaw = 90.0f;
	gameCam->pitch = -10.0f;
	camera = gameCam;
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

	time -= _step->deltaTimeCorrection;
	if(time <= 0 && !game->switchingScene){
		/*game->scenes.at(game->newScene)->unload();
		game->scenes.erase(game->currentScene);
		game->scenes.insert(std::make_pair("Raid the Castle", new RaidTheCastle(game)));
		game->switchScene(game->scenes.at(game->scenes.at());*/
	}

	arduino->update(_step);
	puppetController->update(_step);
	puppetController2->update(_step);
	puppetController3->update(_step);
	puppetController4->update(_step);

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
			camera = gameCam;
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
	if(keyboard->keyJustDown(GLFW_KEY_T)){
		playerCharacter->action();
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