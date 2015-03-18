#pragma once

#include "PuppetScene.h"

#include "Texture.h"
#include "Sprite.h"
#include "shader/BaseComponentShader.h"
#include "shader/ShaderComponentTexture.h"
#include "shader/ShaderComponentHsv.h"
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

#include <RaidTheCastle.h>
#include <Rapunzel.h>

#include <PuppetResourceManager.h>
#include <NumberUtils.h>
#include <Resource.h>
#include <Easing.h>
#include <VictoryScene.h>

PuppetScene::PuppetScene(PuppetGame * _game, float seconds):
	LayeredScene(_game, 3),
	duration(seconds),
	currentTime(0),
	countDown(0),
	displayingSplash(false),
	splashMessage(nullptr),
	splashDuration(3.f),
	cl(nullptr),
	world(new Box2DWorld(b2Vec2(0.f, -98.0f))),
	drawer(new Box2DDebugDraw(this, world)),
	ground(new MeshEntity(Resource::loadMeshFromObj("../assets/hurly-burly/stage.vox"))),
	background(new MeshEntity(MeshFactory::getPlaneMesh())),
	shader(new BaseComponentShader()),
	soundManager(new SoundManager()),
	backgroundSoundManager(new SoundManager()),
	countdownSoundManager(new SoundManager()),
	mouseCam(false),
	randomGround(new RandomGround(world, 100, 0.4f, PuppetResourceManager::ground1, 1, 1))
{
	world->b2world->SetContactListener(cl);
	shader->components.push_back(new ShaderComponentTexture(shader));
	shader->components.push_back(new ShaderComponentHsv(shader, 0, 1.25, 1.4, 1));
	shader->compileShader();
	renderOptions->alphaSorting = true;
	
	//Add Audio
	countdownSoundManager->addNewSound("0", "../assets/hurly-burly/audio/HighCountdown_Zero.ogg");
	countdownSoundManager->addNewSound("1", "../assets/hurly-burly/audio/silence.ogg");
	countdownSoundManager->addNewSound("2", "../assets/hurly-burly/audio/HighCountdown_One.ogg");
	countdownSoundManager->addNewSound("3", "../assets/hurly-burly/audio/HighCountdown_Two.ogg");
	countdownSoundManager->addNewSound("4", "../assets/hurly-burly/audio/HighCountdown_Three.ogg");
	countdownSoundManager->addNewSound("5", "../assets/hurly-burly/audio/HighCountdown_Four.ogg");
	countdownSoundManager->addNewSound("6", "../assets/hurly-burly/audio/HighCountdown_Five.ogg");

	//Since these are chosen randomly its easiest to just use numbers as the keys and generate a random number
	backgroundSoundManager->addNewSound("1", "../assets/hurly-burly/audio/songs/WesternSong.ogg");
	backgroundSoundManager->addNewSound("2", "../assets/hurly-burly/audio/songs/FastSong.ogg");
	backgroundSoundManager->addNewSound("3", "../assets/hurly-burly/audio/songs/MelodicaSong.ogg");

	background->setShader(shader, true);
	background->transform->translate(0.0f, 50.f, -15.f/2.f);
	background->transform->scale(125 * 5, 50, 1);
	background->mesh->pushTexture2D(PuppetResourceManager::sky);
	background->mesh->uvEdgeMode = GL_REPEAT;
	background->mesh->dirty = true;

	ground->setShader(shader, true);
	//ground->setTranslationPhysical(0, 0, 0);
	//ground->transform->rotate(90.f, 1, 0, 0, kOBJECT);
	//ground->transform->rotate(90.f, 0, 0, 1, kOBJECT);
	ground->transform->scale(1000, 100, 100);
	ground->transform->translate(50.f/2.f, 0, -15.f/2.f);
	ground->mesh->uvEdgeMode = GL_REPEAT;
	ground->mesh->pushTexture2D(PuppetResourceManager::stageFloor);
	for(Vertex & v : ground->mesh->vertices){
		v.u *= 10;
		v.v *= 100;
	}
	ground->mesh->dirty = true;
	
	boundaries.push_back(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody));
	boundaries.push_back(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody));
	boundaries.push_back(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody));
	boundaries.push_back(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody));
	
	float width = 170.f;
	float height = 50.f;
	float size = 3.f;

	boundaries.at(0)->transform->scale(size, height/2.f, size);
	boundaries.at(1)->transform->scale(size, height/2.f, size);
	boundaries.at(2)->transform->scale(width/2.f, size, size);
	boundaries.at(3)->transform->scale(width/2.f, size, size);

	boundaries.at(0)->setTranslationPhysical(width, height/2.f, 0);
	boundaries.at(1)->setTranslationPhysical(0, height/2.f, 0);
	boundaries.at(2)->setTranslationPhysical(width/2.f, height-size, 0);
	boundaries.at(3)->setTranslationPhysical(width/2.f, -size, 0);
	
	/*addChild(boundaries.at(0), 0);
	addChild(boundaries.at(1), 0);
	addChild(boundaries.at(2), 0);
	addChild(boundaries.at(3), 0);*/
	
	world->addToWorld(boundaries.at(0));
	world->addToWorld(boundaries.at(1));
	world->addToWorld(boundaries.at(2));
	world->addToWorld(boundaries.at(3));

	b2Filter sf;
	sf.categoryBits = PuppetGame::kBOUNDARY;
	sf.maskBits = -1;
	boundaries.at(0)->body->GetFixtureList()->SetFilterData(sf);
	boundaries.at(1)->body->GetFixtureList()->SetFilterData(sf);
	boundaries.at(2)->body->GetFixtureList()->SetFilterData(sf);
	sf.categoryBits = PuppetGame::kBOUNDARY | PuppetGame::kGROUND;
	boundaries.at(3)->body->GetFixtureList()->SetFilterData(sf);
	boundaries.at(3)->body->GetFixtureList()->SetFriction(1);
	boundaries.at(3)->body->GetFixtureList()->SetRestitution(0);

	//ground->body->SetTransform(b2Vec2(0, -250), 0);
	/*ground->mesh->vertices.at(0).z -= 250;
	ground->mesh->vertices.at(1).z -= 250;
	ground->mesh->vertices.at(2).z -= 250;
	ground->mesh->vertices.at(3).z -= 250;
	ground->mesh->dirty = true;*/

	//Set UVs so the texture isn't stretched
	/*ground->mesh->setUV(0, 0.0,  0.0);
	ground->mesh->setUV(1, 0.0,  4.0);
	ground->mesh->setUV(2, 40.0, 4.0);
	ground->mesh->setUV(3, 40.0, 0.0);*/

	//world->addToWorld(ground, 2);
	addChild(ground, 0);

	int timeOfDayOptions = 4;
	int timeOfDay = std::rand()%timeOfDayOptions;
	background->mesh->setUV(0, (float)timeOfDay/timeOfDayOptions, 0);
	background->mesh->setUV(1, (float)(timeOfDay+1)/timeOfDayOptions, 0);
	background->mesh->setUV(2, (float)(timeOfDay+1)/timeOfDayOptions, 1);
	background->mesh->setUV(3, (float)timeOfDay/timeOfDayOptions, 1);

	addChild(background, 0);

	/*b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f * std::abs(ground->transform->scaleVector.x), 1.0f * std::abs(ground->transform->scaleVector.y));	
	b2Fixture * groundFixture = ground->getNewFixture(dynamicBox, 1.0f);
	groundFixture->SetSensor(false);
	groundFixture->SetUserData(this);
	*/

	Texture * treeTex1 = PuppetResourceManager::tree1;
	Texture * treeTex2 = PuppetResourceManager::tree2;
	Texture * bushTex1 = PuppetResourceManager::bush1;
	Texture * bushTex2 = PuppetResourceManager::bush2;
	
	int numFoliage = 60;
	for(signed long int i = 0; i < numFoliage; ++i){
		float height = std::rand()%500/50.f+5.f;
		MeshEntity * foliage = new MeshEntity(MeshFactory::getPlaneMesh());
		foliage->setShader(shader, true);
		foliage->transform->translate((std::rand()%500/3.f)-25.f, height, max(-9, -(float)(numFoliage-i)/numFoliage)*8.f - 1.f);
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

		if(i == 3){
			randomGround->setTranslationPhysical(0.0f, 0.0f, max(-9, -(float)(numFoliage-i)/numFoliage)*8.f - 1.f);
			addChild(randomGround, 0);
		}
	}
	
	world->b2world->SetDebugDraw(drawer);
	//drawer->AppendFlags(b2Draw::e_aabbBit);
	drawer->AppendFlags(b2Draw::e_shapeBit);
	drawer->AppendFlags(b2Draw::e_centerOfMassBit);
	drawer->AppendFlags(b2Draw::e_jointBit);
	//drawer->AppendFlags(b2Draw::e_pairBit);
	addChild(drawer, 2);

	randomGround->setShader(shader, true);
	//randomGround->mesh->uvEdgeMode = GL_REPEAT;

	//world->addToWorld(randomGround);
	
	//Set up cameras
	mouseCamera = new MousePerspectiveCamera();
	mouseCamera->farClip = 1000.f;
	mouseCamera->transform->rotate(90, 0, 1, 0, kWORLD);
	mouseCamera->transform->translate(5.0f, 1.5f, 22.5f);
	mouseCamera->yaw = 90.0f;
	mouseCamera->pitch = -10.0f;

	gameCam = new FollowCamera(10, glm::vec3(0, 0, 0), 0, 0);
	gameCam->farClip = 1000.f;
	gameCam->transform->rotate(90, 0, 1, 0, kWORLD);
	gameCam->transform->translate(5.0f, 1.5f, 22.5f);
	gameCam->minimumZoom = 22.5f;
	gameCam->yaw = 90.0f;
	gameCam->pitch = -10.0f;
	camera = gameCam;
	
	Sprite * countDown0 = new Sprite(nullptr, new Transform());
	Sprite * countDownWait = new Sprite(nullptr, new Transform());
	Sprite * countDown1 = new Sprite(nullptr, new Transform());
	Sprite * countDown2 = new Sprite(nullptr, new Transform());
	Sprite * countDown3 = new Sprite(nullptr, new Transform());
	Sprite * countDown4 = new Sprite(nullptr, new Transform());
	Sprite * countDown5 = new Sprite(nullptr, new Transform());
	
	countDown0->transform->scale(glm::vec3(3, 3, 0));
	countDown1->transform->scale(glm::vec3(3, 3, 0));
	countDown2->transform->scale(glm::vec3(3, 3, 0));
	countDown3->transform->scale(glm::vec3(3, 3, 0));
	countDown4->transform->scale(glm::vec3(3, 3, 0));
	countDown5->transform->scale(glm::vec3(3, 3, 0));
	
	countDown0->mesh->pushTexture2D(PuppetResourceManager::countDown0->texture);
	countDownWait->mesh->pushTexture2D(PuppetResourceManager::blank);
	countDown1->mesh->pushTexture2D(PuppetResourceManager::countDown1->texture);
	countDown2->mesh->pushTexture2D(PuppetResourceManager::countDown2->texture);
	countDown3->mesh->pushTexture2D(PuppetResourceManager::countDown3->texture);
	countDown4->mesh->pushTexture2D(PuppetResourceManager::countDown4->texture);
	countDown5->mesh->pushTexture2D(PuppetResourceManager::countDown5->texture);
	
	countDownNumbers.push_back(countDown0);
	countDownNumbers.push_back(countDownWait);
	countDownNumbers.push_back(countDown1);
	countDownNumbers.push_back(countDown2);
	countDownNumbers.push_back(countDown3);
	countDownNumbers.push_back(countDown4);
	countDownNumbers.push_back(countDown5);
	countDown = countDownNumbers.size();
	
	for(Sprite * n : countDownNumbers){
		n->setShader(shader, true);
		n->transform->scale(-1, 1, 1);
	}
}

PuppetScene::~PuppetScene(){
	delete soundManager;
	delete countdownSoundManager;
	delete backgroundSoundManager;
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
				float scale = Easing::easeOutBack(splashDuration - currentTime, 0, 10, splashDuration);
				splashMessage->transform->scaleVector = glm::vec3(-scale, scale, 1);
			}else{
				addChild(splashMessage, 2);
				displayingSplash = true;
			}

			splashMessage->transform->translationVector.x = gameCam->transform->translationVector.x;
			splashMessage->transform->translationVector.y = gameCam->transform->translationVector.y;
			splashMessage->transform->translationVector.z = gameCam->transform->translationVector.z - 10;

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
		n->transform->translationVector.z = gameCam->transform->translationVector.z - 10;
	}
	
	// destroy used up items
	for(signed long int i = items.size()-1; i >= 0; --i){
		Item * item = items.at(i);
		
		if(item->destroy){
			destroyItem(item);
			items.erase(items.begin() + i);
		}
	}
	
	world->update(_step);

	if(this == game->currentScene){
		currentTime += _step->deltaTime;
		if(currentTime > duration){
			complete();
		}else if (currentTime > duration - countDownNumbers.size()){
			if(duration - currentTime < countDown){
				doCountDown();	
			}
			if(countDown < countDownNumbers.size()){
				float displayTime = fmod(currentTime, 1.f);
				if(displayTime < 0.5f){
					float scale = Easing::easeOutElastic(displayTime, 0.f, 5.f, 0.5f);
					countDownNumbers.at(countDown)->transform->scaleVector = glm::vec3(-scale, scale, 1.f);
				}else{
					float scale = Easing::easeInCirc(displayTime-0.5f, 5.f, -5.f, 0.5f);
					countDownNumbers.at(countDown)->transform->scaleVector = glm::vec3(-scale, scale, 1.f);
				}
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

	if(dynamic_cast<RaidTheCastle *>(this) != nullptr){
		//game->scenes.insert(std::make_pair("Rapunzel", new Rapunzel(static_cast<PuppetGame *>(game))));
		//game->switchScene("Rapunzel", true);
	}else{
		//game->scenes.insert(std::make_pair("Raid the Castle", new RaidTheCastle(static_cast<PuppetGame *>(game))));
		//game->switchScene("Raid the Castle", true);
	}

	game->scenes.insert(std::make_pair("Victory", new VictoryScene(static_cast<PuppetGame *>(game), players)));
	game->switchScene("Victory", true);

	//Scene * oldScene = game->currentScene;
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

	countdownSoundManager->play(std::to_string(countDown));

	// Add new number to scene
	addChild(countDownNumbers.at(countDown), 2);
}

void PuppetScene::playRandomBackgroundMusic(){
	backgroundSoundManager->playRandomSound();
}