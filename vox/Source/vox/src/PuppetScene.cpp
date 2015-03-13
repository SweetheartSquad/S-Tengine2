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
#include <Item.h>
#include <Arduino.h>
#include <AccelerometerParser.h>
#include <Accelerometer.h>
#include <FollowCamera.h>
#include <PuppetCharacter.h>
#include <PuppetController.h>
#include <RandomGround.h>

#include "RaidTheCastle.h"
#include <PuppetResourceManager.h>

PuppetScene::PuppetScene(PuppetGame * _game, float seconds):
	LayeredScene(_game, 3),
	duration(seconds),
	currentTime(0),
	countDown(5),
	displayingSplash(false),
	splashMessage(nullptr),
	splashDuration(3.f),
	cl(nullptr),
	world(new Box2DWorld(b2Vec2(0.f, -98.0f))),
	drawer(new Box2DDebugDraw(this, world)),
	ground(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody, true)),
	background(new MeshEntity(MeshFactory::getPlaneMesh())),
	shader(new BaseComponentShader()),
	soundManager(new SoundManager()),
	countdownSoundManager(new SoundManager()),
	mouseCam(false),
	randomGround(new RandomGround(world, 100, 0.4f, PuppetResourceManager::ground1, 1, 1))
{
	world->b2world->SetContactListener(cl);
	shader->components.push_back(new TextureShaderComponent());
	shader->compileShader();
	renderOptions->alphaSorting = true;
	
	countdownSoundManager->addNewSound("1", "../assets/hurly-burly/audio/HighCountdown_One.ogg");
	countdownSoundManager->addNewSound("2", "../assets/hurly-burly/audio/HighCountdown_Two.ogg");
	countdownSoundManager->addNewSound("3", "../assets/hurly-burly/audio/HighCountdown_Three.ogg");
	countdownSoundManager->addNewSound("4", "../assets/hurly-burly/audio/HighCountdown_Four.ogg");
	countdownSoundManager->addNewSound("5", "../assets/hurly-burly/audio/HighCountdown_5.ogg");


	background->setShader(shader, true);
	background->transform->translate(0.0f, 50.f, -10.0f);
	background->transform->scale(125 * 5, 50, 1);
	background->mesh->pushTexture2D(PuppetResourceManager::sky);
	background->mesh->uvEdgeMode = GL_REPEAT;
	background->mesh->dirty = true;

	ground->setShader(shader, true);
	ground->setTranslationPhysical(0, 0, 0);
	ground->transform->rotate(90.f, 1, 0, 0, kOBJECT);
	ground->transform->rotate(90.f, 0, 0, 1, kOBJECT);
	ground->transform->scale(25, 250, 1);
	ground->mesh->uvEdgeMode = GL_REPEAT;
	ground->mesh->pushTexture2D(PuppetResourceManager::stageFloor);
	ground->body->SetTransform(b2Vec2(0, -250), 0);
	ground->mesh->vertices.at(0).z -= 250;
	ground->mesh->vertices.at(1).z -= 250;
	ground->mesh->vertices.at(2).z -= 250;
	ground->mesh->vertices.at(3).z -= 250;
	ground->mesh->dirty = true;

	//Set UVs so the texture isn't stretched
	ground->mesh->setUV(0, 0.0,  0.0);
	ground->mesh->setUV(1, 0.0,  4.0);
	ground->mesh->setUV(2, 40.0, 4.0);
	ground->mesh->setUV(3, 40.0, 0.0);

	world->addToWorld(ground, 2);
	addChild(ground, 0);
	ground->body->GetFixtureList()->SetFriction(1);
	ground->body->GetFixtureList()->SetRestitution(0);

	int timeOfDayOptions = 4;
	int timeOfDay = std::rand()%timeOfDayOptions;
	background->mesh->setUV(0, (float)timeOfDay/timeOfDayOptions, 0);
	background->mesh->setUV(1, (float)(timeOfDay+1)/timeOfDayOptions, 0);
	background->mesh->setUV(2, (float)(timeOfDay+1)/timeOfDayOptions, 1);
	background->mesh->setUV(3, (float)timeOfDay/timeOfDayOptions, 1);

	addChild(background, 0);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f * std::abs(ground->transform->scaleVector.x), 1.0f * std::abs(ground->transform->scaleVector.y));	
	b2Fixture * groundFixture = ground->getNewFixture(dynamicBox, 1.0f);
	groundFixture->SetSensor(false);
	groundFixture->SetUserData(this);
	b2Filter sf;
	sf.categoryBits = PuppetGame::kGROUND;
	sf.maskBits = PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER;
	groundFixture->SetFilterData(sf);

	/*Texture * treeTex1 = new Texture("../assets/hurly-burly/Foliage/Tree1-ds.png", 1024, 1024, true, true);
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
	}*/

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
	
	puppetController =  new PuppetController(acc, nullptr);
	puppetController2 = new PuppetController(acc2, nullptr);
	puppetController3 = new PuppetController(acc3, nullptr);
	puppetController4 = new PuppetController(acc4, nullptr);
	
	world->b2world->SetDebugDraw(drawer);
	//drawer->AppendFlags(b2Draw::e_aabbBit);
	drawer->AppendFlags(b2Draw::e_shapeBit);
	drawer->AppendFlags(b2Draw::e_centerOfMassBit);
	drawer->AppendFlags(b2Draw::e_jointBit);
	//drawer->AppendFlags(b2Draw::e_pairBit);
	addChild(drawer, 2);

	randomGround->setShader(shader, true);
	randomGround->setTranslationPhysical(0.0f, 0.0f, 0.0f);
	//randomGround->mesh->uvEdgeMode = GL_REPEAT;

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
	gameCam->farClip = 1000.f;
	gameCam->transform->rotate(90, 0, 1, 0, kWORLD);
	gameCam->transform->translate(5.0f, 1.5f, 22.5f);
	gameCam->minimumZoom = 22.5f;
	gameCam->yaw = 90.0f;
	gameCam->pitch = -10.0f;
	camera = gameCam;

	TextureSampler * countDown1TextureSampler = PuppetResourceManager::countDown1;
	TextureSampler * countDown2TextureSampler = PuppetResourceManager::countDown2;
	TextureSampler * countDown3TextureSampler = PuppetResourceManager::countDown3;
	TextureSampler * countDown4TextureSampler = PuppetResourceManager::countDown4;
	TextureSampler * countDown5TextureSampler = PuppetResourceManager::countDown5;


	Sprite * countDown1 = new Sprite(nullptr, new Transform());
	Sprite * countDown2 = new Sprite(nullptr, new Transform());
	Sprite * countDown3 = new Sprite(nullptr, new Transform());
	Sprite * countDown4 = new Sprite(nullptr, new Transform());
	Sprite * countDown5 = new Sprite(nullptr, new Transform());

	countDown1->transform->scale(glm::vec3(3, 3, 0));
	countDown2->transform->scale(glm::vec3(3, 3, 0));
	countDown3->transform->scale(glm::vec3(3, 3, 0));
	countDown4->transform->scale(glm::vec3(3, 3, 0));
	countDown5->transform->scale(glm::vec3(3, 3, 0));
	
	countDown1->mesh->pushTexture2D(countDown1TextureSampler->texture);
	countDown2->mesh->pushTexture2D(countDown2TextureSampler->texture);
	countDown3->mesh->pushTexture2D(countDown3TextureSampler->texture);
	countDown4->mesh->pushTexture2D(countDown4TextureSampler->texture);
	countDown5->mesh->pushTexture2D(countDown5TextureSampler->texture);

	countDownNumbers.push_back(countDown1);
	countDownNumbers.push_back(countDown2);
	countDownNumbers.push_back(countDown3);
	countDownNumbers.push_back(countDown4);
	countDownNumbers.push_back(countDown5);
	
	for(Sprite * n : countDownNumbers){
		n->setShader(shader, true);
		n->transform->scale(-1, 1, 1);
	}
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

	if(splashMessage != nullptr){
		if(currentTime < splashDuration){
			if(displayingSplash){
				// the factor of 15 is only there because I can't load this thing at the correct size...
				float scale = 15 * (splashDuration - currentTime)/splashDuration;
				splashMessage->transform->scaleVector = glm::vec3(-scale, scale, 1);
			}else{
				addChild(splashMessage, 2);
				displayingSplash = true;
			}

			splashMessage->transform->translationVector.x = gameCam->transform->translationVector.x;
			splashMessage->transform->translationVector.y = gameCam->transform->translationVector.y;
			splashMessage->transform->translationVector.z = 0;

		}else{
			// Remove previous number from scene
			// Just copying destroyItem stuff for now
			for(signed long int j = children.size()-1; j >= 0; --j){
				if(children.at(j) == splashMessage){
					children.erase(children.begin() + j);
				}
			}
			for(std::vector<Entity *> * layer : layers){
				for(signed long int j = layer->size()-1; j >= 0; --j){
					if(layer->at(j) == splashMessage){
						layer->erase(layer->begin() + j);
					}
				}
			}
		}
	}

	for(Sprite * n : countDownNumbers){
		n->transform->translationVector.x = gameCam->transform->translationVector.x;
		n->transform->translationVector.y = gameCam->transform->translationVector.y;
		n->transform->translationVector.z = 0;
	}
	
	// destroy used up items
	for(signed long int i = items.size()-1; i >= 0; --i){
		Item * item = items.at(i);
		
		if(item->destroy){
			destroyItem(item);
			items.erase(items.begin() + i);
		}
	}
	

	arduino->update(_step);
	puppetController->update(_step);
	puppetController2->update(_step);
	puppetController3->update(_step);
	puppetController4->update(_step);

	world->update(_step);

	if(this == game->currentScene){
		currentTime += _step->deltaTime;
		if(currentTime > duration){
			//complete();
		}else if (currentTime > duration - 5){
			if(duration - currentTime < countDown){
				doCountDown();	
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
	//std::cout << "update\n";
}

void PuppetScene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	LayeredScene::render(_matrixStack, _renderStack);
}

void PuppetScene::complete(){
	// select new scene, based on if this is a victory scene or a gameplay scene?
	// switch to next scene
	// delete this scene

	// temporary stuff
	game->scenes.insert(std::make_pair("Raid the Castle2", new RaidTheCastle(static_cast<PuppetGame *>(game))));
	Scene * oldScene = game->currentScene;
	game->switchScene("Raid the Castle2");
}

void PuppetScene::destroyItem(Item * _item){
	// remove the item from the children list
	for(signed long int j = children.size()-1; j >= 0; --j){
		if(children.at(j) == _item){
			children.erase(children.begin() + j);
		}else{
			for(signed long int k = _item->components.size()-1; k >= 0; --k){
				if(children.at(j) == *_item->components.at(k)){
					children.erase(children.begin() + j);
				}
			}
		}
	}
	for(std::vector<Entity *> * layer : layers){
		for(signed long int j = layer->size()-1; j >= 0; --j){
			if(layer->at(j) == _item){
				layer->erase(layer->begin() + j);
			}else{
				for(signed long int k = _item->components.size()-1; k >= 0; --k){
					if(layer->at(j) == *_item->components.at(k)){
						layer->erase(layer->begin() + j);
					}
				}
			}
		}
	}
	// remove from follow cam
	gameCam->removeTarget(_item);
	for(signed long int j = _item->components.size()-1; j >= 0; --j){
		gameCam->removeTarget(*_item->components.at(j));
	}

	//maybe use something like children.erase(std::remove(children.begin(), children.end(), item), children.end());

	delete _item;
}

void PuppetScene::doCountDown(){
	// Remove previous number
	if (countDown <= countDownNumbers.size() - 1){
		// Remove previous number from scene
		// Just copying destroyItem stuff for now
		for(signed long int j = children.size()-1; j >= 0; --j){
			if(children.at(j) == countDownNumbers.at(countDown)){
				children.erase(children.begin() + j);
			}
		}
		for(std::vector<Entity *> * layer : layers){
			for(signed long int j = layer->size()-1; j >= 0; --j){
				if(layer->at(j) == countDownNumbers.at(countDown)){
					layer->erase(layer->begin() + j);
				}
			}
		}
	}
	
	// Decrease countdown
	-- countDown;
	
	// Display countdown
	std::cout << "=========================" << std::endl;
	std::cout << countDown << std::endl;
	std::cout << "idx: " << countDown << std::endl;
	std::cout << "=========================" << std::endl;

	countdownSoundManager->play(std::to_string(countDown + 1));

	// Add new number to scene
	addChild(countDownNumbers.at(countDown), 2);
}