#pragma once

#include "PuppetScene.h"

#include <Texture.h>
#include <TextureSampler.h>
#include "Sprite.h"
#include "shader/BaseComponentShader.h"
#include "shader/ShaderComponentTexture.h"
#include "shader/ShaderComponentHsv.h"
#include "shader/ShaderComponentTint.h"
#include "shader/ShaderComponentAlpha.h"
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
#include <SlayTheDragon.h>

#include <PuppetResourceManager.h>
#include <NumberUtils.h>
#include <Resource.h>
#include <Easing.h>
#include <VictoryScene.h>

#include <ParticleSystem.h>
#include <SlayTheDragon.h>
#include <ItemGold.h>

class SlayTheDragon;

PuppetScene::PuppetScene(PuppetGame * _game, float seconds, float _width, float _height, float _size):
	LayeredScene(_game, 3),
	sceneHeight(_height),
	sceneWidth(_width),
	duration(seconds),
	currentTime(0),
	countDown(0),
	displayingSplash(false),
	splashSoundPlayed(false),
	splashMessage(nullptr),
	splashDuration(3.f),
	cl(nullptr),
	world(new Box2DWorld(b2Vec2(0.f, -98.0f))),
	drawer(nullptr),
	ground(new MeshEntity(Resource::loadMeshFromObj("../assets/hurly-burly/stage.vox"))),
	background(new MeshEntity(MeshFactory::getPlaneMesh())),
	shader(new BaseComponentShader()),
	soundManager(new SoundManager(-1)),
	backgroundSoundManager(new SoundManager(-1)),
	countdownSoundManager(new SoundManager(-1)),
	mouseCam(false),
	randomGround(new RandomGround(world, 100, 0.4f, PuppetResourceManager::paper->texture, 3, 1)),
	victoryTriggered(false),
	sceneStart(vox::step.time)
{

	world->b2world->SetContactListener(cl);
	shader->components.push_back(new ShaderComponentTexture(shader));
	shader->components.push_back(new ShaderComponentHsv(shader, 0.f, 1.25f, 1.4f));
	shader->components.push_back(new ShaderComponentTint(shader, 0.f, 0.f, 0.f));
	shader->components.push_back(new ShaderComponentAlpha(shader, 1.f));
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
	
	boundaries.push_back(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody));
	boundaries.push_back(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody));
	boundaries.push_back(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody));
	boundaries.push_back(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody));

	boundaries.at(0)->transform->scale(_size, sceneHeight/2.f, _size);
	boundaries.at(1)->transform->scale(_size, sceneHeight/2.f, _size);
	boundaries.at(2)->transform->scale(sceneWidth/2.f, _size, _size);
	boundaries.at(3)->transform->scale(sceneWidth/2.f, _size, _size);

	boundaries.at(0)->setTranslationPhysical(sceneWidth, sceneHeight/2.f, 0);
	boundaries.at(1)->setTranslationPhysical(0, sceneHeight/2.f, 0);
	boundaries.at(2)->setTranslationPhysical(sceneWidth/2.f, sceneHeight-_size, 0);
	boundaries.at(3)->setTranslationPhysical(sceneWidth/2.f, -_size, 0);
	
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

	int timeOfDayOptions = PuppetResourceManager::sky->width;
	int timeOfDay = std::rand()%timeOfDayOptions;
	background->mesh->setUV(0, (float)timeOfDay / timeOfDayOptions, 0);
	background->mesh->setUV(1, (float)timeOfDay / timeOfDayOptions, 0);
	background->mesh->setUV(2, (float)timeOfDay / timeOfDayOptions, 1);
	background->mesh->setUV(3, (float)timeOfDay / timeOfDayOptions, 1);

	addChild(background, 0);

	/*b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f * std::abs(ground->transform->scaleVector.x), 1.0f * std::abs(ground->transform->scaleVector.y));	
	b2Fixture * groundFixture = ground->getNewFixture(dynamicBox, 1.0f);
	groundFixture->SetSensor(false);
	groundFixture->SetUserData(this);
	*/

	drawer = new Box2DDebugDraw(this, world);
	world->b2world->SetDebugDraw(drawer);
	//drawer->AppendFlags(b2Draw::e_aabbBit);
	drawer->AppendFlags(b2Draw::e_shapeBit);
	drawer->AppendFlags(b2Draw::e_centerOfMassBit);
	drawer->AppendFlags(b2Draw::e_jointBit);
	//drawer->AppendFlags(b2Draw::e_pairBit);
	addChild(drawer, 2);

	//randomGround->mesh->uvEdgeMode = GL_REPEAT;

	//world->addToWorld(randomGround);
	
	//Set up cameras
	mouseCamera = new MousePerspectiveCamera();
	mouseCamera->farClip = 1000.f;
	mouseCamera->transform->rotate(90, 0, 1, 0, kWORLD);
	mouseCamera->transform->translate(5.0f, 1.5f, 22.5f);
	mouseCamera->yaw = 90.0f;
	mouseCamera->pitch = -10.0f;
	mouseCamera->speed = 1;

	gameCam = new FollowCamera(15, glm::vec3(0, 0, 0), 0, 0);
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

    particleSystem = new ParticleSystem(PuppetResourceManager::dustParticle, world, 0, 0, 0);
    particleSystem->addToLayeredScene(this, 1);
    addChild(particleSystem, 1);
	particleSystem->setShader(shader, true);
	particleSystem->emissionRate = -1;
	particleSystem->emissionAmount = 0;
}

PuppetScene::~PuppetScene(){
	delete soundManager;
	delete countdownSoundManager;
	delete backgroundSoundManager;
}

void PuppetScene::load(){
	Scene::load();

	for(Sprite * s : countDownNumbers){
		s->load();
	}
}

void PuppetScene::unload(){
	Scene::unload();
	
	for(Sprite * s : countDownNumbers){
		s->unload();
	}
}

void PuppetScene::update(Step * _step){
	// player controls
	if (players.size() > 0){
		if (keyboard->keyJustDown(GLFW_KEY_W)){
			players.at(0)->jump();
		}if (keyboard->keyDown(GLFW_KEY_A)){
			players.at(0)->targetRoll = glm::radians(-45.f);
		}
		if (keyboard->keyDown(GLFW_KEY_D)){
			players.at(0)->targetRoll = glm::radians(45.f);
		}
		if (keyboard->keyJustDown(GLFW_KEY_T)){
			players.at(0)->action();
			players.at(3)->action();
			players.at(2)->action();
			players.at(1)->action();
		}

		if (keyboard->keyDown(GLFW_KEY_B)){
			for (PuppetCharacter * p : players){
				p->control = 0;
			}
		}
	}
	// camera controls
	if (keyboard->keyDown(GLFW_KEY_UP)){
		camera->transform->translate((camera->forwardVectorRotated) * static_cast<MousePerspectiveCamera *>(camera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_DOWN)){
		camera->transform->translate((camera->forwardVectorRotated) * -static_cast<MousePerspectiveCamera *>(camera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_LEFT)){
		camera->transform->translate((camera->rightVectorRotated) * -static_cast<MousePerspectiveCamera *>(camera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_RIGHT)){
		camera->transform->translate((camera->rightVectorRotated) * static_cast<MousePerspectiveCamera *>(camera)->speed);
	}
	

	if(keyboard->keyDown(GLFW_KEY_G)){
		Item * g = new ItemGold(world);
		g->addToLayeredScene(this, 1);
		g->setShader(shader, true);
        addChild(g, 1);
		g->translateComponents(glm::vec3(vox::NumberUtils::randomFloat(0, sceneWidth), vox::NumberUtils::randomFloat(0, sceneHeight), 0));
	}


	Scene::update(_step);

    //Box2DSprite * test = new Box2DSprite(world);
    //test->setShader(shader, this);
    //particleSystem->addComponent(test);
    //particleSystem->addParticle();


	if(splashMessage != nullptr){
		if(currentTime < splashDuration){
			if(displayingSplash){
				// the factor of 15 is only there because I can't load this thing at the correct size...
				//float scale = Easing::easeOutBack(splashDuration - currentTime, 0, 10, splashDuration);
				float easeTime = splashDuration - currentTime;
				float scale = (easeTime < splashDuration / 2.f) ? Easing::easeOutCubic(easeTime, 0, 10, splashDuration / 2.f) : Easing::easeInElastic(easeTime - splashDuration / 2.f, 10, -10, splashDuration / 2.f);
				splashMessage->transform->scale(glm::vec3(-scale, scale, 1), false);
			}else{
				addChild(splashMessage, 2);
				displayingSplash = true;
			}

			splashMessage->transform->translate(gameCam->transform->getTranslationVector(), false);
			splashMessage->transform->translate(glm::vec3(0,0,-10));
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
		n->transform->translate(gameCam->transform->getTranslationVector(), false);
		n->transform->translate(glm::vec3(0,0,-10));
	}
	
	// destroy used up items
	for(signed long int i = items.size()-1; i >= 0; --i){
		Item * item = items.at(i);
		
		if (item->destroy){
			for (signed long int j = 0; j < std::rand() % 5 + 1; ++j){
				particleSystem->addParticle(item->rootComponent->getPos(false), PuppetResourceManager::dustParticle);
			}
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
					countDownNumbers.at(countDown)->transform->scale(glm::vec3(-scale, scale, 1.f), false);
				}else{
					float scale = Easing::easeInCirc(displayTime-0.5f, 5.f, -5.f, 0.5f);
					countDownNumbers.at(countDown)->transform->scale(glm::vec3(-scale, scale, 1.f), false);
				}
			}
		}
	}


	// camera control
	if (keyboard->keyJustUp(GLFW_KEY_1)){
		mouseCam = !mouseCam;
		if (!mouseCam){
			camera = gameCam;
		}else{
			camera = mouseCamera;
		}
	}

	
	// trigger/speed-up countdown
	if (keyboard->keyJustUp(GLFW_KEY_ENTER)){
		if(currentTime < duration - countDownNumbers.size()){
			currentTime = duration - countDownNumbers.size();
			countDown = countDownNumbers.size();
		}else{
			currentTime += 1;
		}
    }
    PuppetGame * pg = static_cast<PuppetGame *>(game);
    if (keyboard->keyJustUp(GLFW_KEY_8)){
        if (game->currentSceneKey != "Raid The Castle"){
            pg->puppetControllers.at(0)->unassign();
            pg->puppetControllers.at(1)->unassign();
            pg->puppetControllers.at(2)->unassign();
            pg->puppetControllers.at(3)->unassign();
            game->scenes.insert(std::make_pair("Raid The Castle", new RaidTheCastle((PuppetGame *)game)));
            complete("Raid The Castle");
        }
    }
    if (keyboard->keyJustUp(GLFW_KEY_9)){
        if (game->currentSceneKey != "Rapunzel"){
            pg->puppetControllers.at(0)->unassign();
            pg->puppetControllers.at(1)->unassign();
            pg->puppetControllers.at(2)->unassign();
            pg->puppetControllers.at(3)->unassign();
            game->scenes.insert(std::make_pair("Rapunzel", new Rapunzel((PuppetGame *)game)));
            complete("Rapunzel");
        }
    }
    if (keyboard->keyJustUp(GLFW_KEY_0)){
        if (game->currentSceneKey != "Slay The Dragon"){
            pg->puppetControllers.at(0)->unassign();
            pg->puppetControllers.at(1)->unassign();
            pg->puppetControllers.at(2)->unassign();
            pg->puppetControllers.at(3)->unassign();
            game->scenes.insert(std::make_pair("Slay The Dragon", new SlayTheDragon((PuppetGame *)game)));
            complete("Slay The Dragon");
        }
    }

	
	
	if(keyboard->keyJustUp(GLFW_KEY_F11)){
		game->toggleFullScreen();
	}
	if(keyboard->keyJustUp(GLFW_KEY_2)){
		if(drawer != nullptr){
			drawer->drawing = !drawer->drawing;
		}
	}

	bool everyonesDead = true;
	for(auto p : players){
		if(!p->dead){
			everyonesDead = false;
			break;
		}
	}
	if(everyonesDead){
		triggerVictoryState();
	}
}

void PuppetScene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	LayeredScene::render(_matrixStack, _renderOptions);
}

void PuppetScene::triggerVictoryState(){
	if(!victoryTriggered){
		if(currentTime < duration - 1){
			currentTime = duration - 1;
			countDown = 1;
			doCountDown();
		}
	}
}
void PuppetScene::complete(std::string _switchTo){
    PuppetGame * pg = static_cast<PuppetGame *>(game);
    if (_switchTo != ""){
        pg->switchScene(_switchTo, true);
    } else{
	    if(dynamic_cast<VictoryScene *>(this) != nullptr){
		    pg->puppetControllers.at(0)->unassign();
		    pg->puppetControllers.at(1)->unassign();
		    pg->puppetControllers.at(2)->unassign();
		    pg->puppetControllers.at(3)->unassign();

		    pg->loadRandomScene();
	    }else{
		    pg->scenes.insert(std::make_pair("Victory", new VictoryScene(pg, players)));
		    pg->switchScene("Victory", true);
	    }
    }
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
		// make things get
		static_cast<ShaderComponentHsv *>(shader->components.at(1))->setSaturation(static_cast<ShaderComponentHsv *>(shader->components.at(1))->getSaturation() + 0.2f);
		static_cast<ShaderComponentHsv *>(shader->components.at(1))->setValue(static_cast<ShaderComponentHsv *>(shader->components.at(1))->getValue() + 0.2f);
		
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

void PuppetScene::populateBackground(){
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
		randomGround->setShader(shader, true);
		if(i == 3){
			randomGround->setTranslationPhysical(0.0f, 0.0f, max(-9, -(float)(numFoliage-i)/numFoliage)*8.f - 1.f);
			addChild(randomGround, 0);
		}
	}
	addChild(ground, 0);
}

void PuppetScene::populateClouds(){
	int numClouds = 60;
	for(signed long int i = 0; i < numClouds; ++i){
		float height = std::rand()%500/50.f+5.f;
		MeshEntity * cloud = new MeshEntity(MeshFactory::getPlaneMesh());
		cloud->setShader(shader, true);
		cloud->transform->translate((std::rand()%500/3.f)-25.f, height, max(-9, -(float)(numClouds-i)/numClouds)*8.f - 1.f);
		cloud->transform->scale(height, height, 1);
		int tex = i % 4;
		switch(tex){
			case 0:
				cloud->mesh->pushTexture2D(PuppetResourceManager::cloud1); break;
			case 1:
				cloud->mesh->pushTexture2D(PuppetResourceManager::cloud2); break;
			case 2:
				cloud->mesh->pushTexture2D(PuppetResourceManager::cloud3); break;
			case 3:
				cloud->mesh->pushTexture2D(PuppetResourceManager::cloud4); break;
			default:
				break;
		}
		addChild(cloud, 0);
		randomGround->setShader(shader, true);
		if(i == 3){
			randomGround->setTranslationPhysical(0.0f, 0.0f, max(-9, -(float)(numClouds-i)/numClouds)*8.f - 1.f);
			addChild(randomGround, 0);
		}
	}
}