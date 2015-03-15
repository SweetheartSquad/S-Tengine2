#pragma once

#include <RaidTheCastle.h>
#include <RaidTheCastleContactListener.h>
#include <Castle.h>
#include <PuppetGame.h>
#include <PuppetCharacter.h>
#include <FollowCamera.h>
#include <Behaviour.h>
#include <BehaviourFollow.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>
#include <Boulder.h>
#include <Catapult.h>
#include <Box2DSprite.h>
#include <Box2DWorld.h>
#include <MeshEntity.h>
#include <MeshInterface.h>
#include <MeshFactory.h>
#include <shader/BaseComponentShader.h>
#include <keyboard.h>
#include <Texture.h>
#include <PuppetCharacterCastleChampion.h>
#include <PuppetCharacterKnight.h>
#include <PuppetTexturePack.h>
#include <PuppetController.h>

#include <glfw\glfw3.h>


RaidTheCastle::RaidTheCastle(PuppetGame* _game):
	PuppetScene(_game, 10),
	castle(new Castle(world, PuppetGame::kSTRUCTURE, PuppetGame::kITEM, 30)),
	catapult(new Catapult(world, PuppetGame::kSTRUCTURE, PuppetGame::kITEM | PuppetGame::kPLAYER, -10)),
	champion(new PuppetCharacterCastleChampion(world, PuppetGame::kPLAYER, -1, -20)),
	playerCharacter(new PuppetCharacterKnight(false, 0, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -1)),
	playerCharacter2(new PuppetCharacterKnight(false, 1, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -2)),
	playerCharacter3(new PuppetCharacterKnight(false, 2, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -3)),
	playerCharacter4(new PuppetCharacterKnight(false, 3, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -4))
{
	cl = new RaidTheCastleContactListener(this);

	TextureSampler * splashMessageTextureSampler = RaidTheCastleResourceManager::splashMessage;
	splashMessage = new Sprite(nullptr, new Transform());
	splashMessage->transform->scale(glm::vec3(3, 3, 0));
	splashMessage->mesh->pushTexture2D(splashMessageTextureSampler->texture);
	splashMessage->setShader(shader, true);
	splashMessage->transform->scale(-1, 1, 1);

	playerCharacter->setShader(shader, true);
	addChild(playerCharacter, 1);
	playerCharacter->addToLayeredScene(this, 1);
	playerCharacter->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter->translateComponents(glm::vec3(20.0f, 35, 0.f));
	puppetController->puppetCharacter = playerCharacter;
	players.push_back(playerCharacter);

	playerCharacter2->setShader(shader, true);
	addChild(playerCharacter2, 1);
	playerCharacter2->addToLayeredScene(this, 1);
	playerCharacter2->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter2->translateComponents(glm::vec3(40.0f, 35, 0.f));
	puppetController2->puppetCharacter = playerCharacter2;
	players.push_back(playerCharacter2);

	playerCharacter3->setShader(shader, true);
	addChild(playerCharacter3, 1);
	playerCharacter3->addToLayeredScene(this, 1);
	playerCharacter3->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter3->translateComponents(glm::vec3(60.0f, 35, 0.f));
	puppetController3->puppetCharacter = playerCharacter3;
	players.push_back(playerCharacter3);

	playerCharacter4->setShader(shader, true);
	addChild(playerCharacter4, 1);
	playerCharacter4->addToLayeredScene(this, 1);
	playerCharacter4->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter4->translateComponents(glm::vec3(80.0f, 35, 0.f));
	puppetController4->puppetCharacter = playerCharacter4;
	players.push_back(playerCharacter4);

	champion->setShader(shader, true);
	addChild(champion, 0);
	champion->addToLayeredScene(this, 1);
	champion->head->maxVelocity = b2Vec2(10, 10);
	champion->translateComponents(glm::vec3(100.f, 35, 0));

	gameCam->addTarget(playerCharacter->torso);
	gameCam->addTarget(playerCharacter2->torso);
	gameCam->addTarget(playerCharacter3->torso);
	gameCam->addTarget(playerCharacter4->torso);

	castle->setShader(shader, true);
	castle->addToLayeredScene(this, 0);
	addChild(castle, 0);

	castle->translateComponents(glm::vec3(170, 0, 0));

	catapult->setShader(shader, true);
	catapult->addToLayeredScene(this, 1);
	addChild(catapult, 1);

	catapult->translateComponents(glm::vec3(0,0,0));


	playerCharacter4->behaviourManager.addBehaviour(new BehaviourAttack(playerCharacter4, 10, PuppetGame::kPLAYER));
	playerCharacter4->behaviourManager.addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), playerCharacter4, 10));
	playerCharacter4->ai = true;

	playerCharacter3->behaviourManager.addBehaviour(new BehaviourAttack(playerCharacter3, 10, PuppetGame::kPLAYER));
	playerCharacter3->behaviourManager.addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), playerCharacter4, 10));
	playerCharacter3->ai = true;

	//playerCharacter2->behaviourManager.addBehaviour(new BehaviourAttack(playerCharacter2, 10, PuppetGame::kPLAYER));
	//playerCharacter2->behaviourManager.addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), playerCharacter4, 10));
	//playerCharacter2->ai = true;

	//gameCam->addTarget(castle->rootComponent);
	//gameCam->addTarget(catapult->rootComponent);

	for(PuppetCharacter * p : players){
		TextureSampler * weaponTex = RaidTheCastleResourceManager::getRandomWeapon();
		Item * weapon = p->itemToPickup = new Item(false, world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kBOUNDARY | PuppetGame::kGROUND, p->groupIndex, 0, 0, -weaponTex->height/2.5f);

		weapon->rootComponent = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), weaponTex->width, weaponTex->height, weaponTex->texture, p->componentScale);
		weapon->components.push_back(&weapon->rootComponent);
	
		b2Filter sf;
		sf.categoryBits = weapon->categoryBits;
		if(weapon->maskBits != (int16)-1){
			sf.maskBits = weapon->maskBits;
		}else{
			sf.maskBits = 0;
		}
		sf.groupIndex = weapon->groupIndex;

		for(Box2DSprite ** c : weapon->components){
			(*c)->createFixture(sf);
			(*c)->body->GetFixtureList()->SetDensity(0.01f);
			(*c)->body->ResetMassData();
		}

		weapon->setUserData(weapon);
		addChild(weapon, 1);
		weapon->addToLayeredScene(this, 1);
		weapon->setShader(shader, true);
	}

	playRandomBackgroundMusic();
	loadCatapult();
}

RaidTheCastle::~RaidTheCastle(){
}

void RaidTheCastle::update(Step* _step){
	PuppetScene::update(_step);
	if(keyboard->keyDown(GLFW_KEY_B)){
		champion->control = 0;
		playerCharacter->control = 0;
		playerCharacter2->control = 0;
		//playerCharacter->behaviourManager.behaviours.at(0)->targets.clear();
		//playerCharacter->behaviourManager.behaviours.at(0)->active = false;
	}

	/*if(keyboard->keyDown(GLFW_KEY_F)){
		catapult->fireCatapult();
	}*/
	if(catapult->fireBoulder){
		catapult->fireBoulder = false;
		if(catapult->boulderLoaded && catapult->boulderJoint != nullptr){
			world->b2world->DestroyJoint(catapult->boulderJoint);
			catapult->boulderJoint = nullptr;
			catapult->boulder->catapult = nullptr;
			((FollowCamera *)gameCam)->addTarget(catapult->boulder->rootComponent);
			catapult->boulder->playerWhoFired = catapult->playerWhoFired;
			catapult->boulder = nullptr;
			catapult->playerWhoFired = nullptr;
			
			// set the item's group index to zero so that it can collide normally
			/*for(Box2DSprite ** bs : catapult->boulder->components){
				b2Filter b1 = (*bs)->body->GetFixtureList()->GetFilterData();
				b1.groupIndex = 0;
				(*bs)->body->GetFixtureList()->SetFilterData(b1);
				(*bs)->body->GetFixtureList()->Refilter();
			}*/
		}
		catapult->boulderLoaded = false;
	}
	if(catapult->ready && !catapult->boulderLoaded){
		loadCatapult();
	}

	if(keyboard->keyJustDown(GLFW_KEY_W)){
		playerCharacter->jump();
	}if(keyboard->keyDown(GLFW_KEY_A)){
		playerCharacter->targetRoll = glm::radians(-90.f);
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		playerCharacter->targetRoll = glm::radians(90.f);
	}
	if(keyboard->keyJustDown(GLFW_KEY_T)){
		playerCharacter->action();
	}
}

void RaidTheCastle::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	PuppetScene::render(_matrixStack, renderOptions);
}

void RaidTheCastle::load(){
	PuppetScene::load();
}

void RaidTheCastle::unload(){
	PuppetScene::unload();
}

void RaidTheCastle::loadCatapult(){
	Boulder * boulder = new Boulder(world, PuppetGame::kITEM, PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kGROUND, catapult->groupIndex);
	boulder->setShader(shader, true);
	addChild(boulder, 1);
	boulder->addToLayeredScene(this, 1);
	items.push_back(boulder);

	b2Vec2 armPos = b2Vec2(-catapult->arm->getCorrectedWidth() * 0.8f, catapult->base->getCorrectedHeight() * 0.9f);
	// snap to capatult base
	boulder->snapComponents(catapult->base);
	// translate to arm position from base
	boulder->translateComponents(glm::vec3(armPos.x*2, armPos.y, 0));
	//boulder->translateComponents(glm::vec3(-21, 6, 0)); // this is hard-coded, should not be
	catapult->cooldownCnt = 0.f;
	catapult->ready = true;
	catapult->boulderLoaded = true;
	catapult->boulder = boulder;

	// axle
	b2WeldJointDef abpj;
	abpj.bodyA = catapult->arm->body;
	abpj.bodyB = boulder->boulder->body;
	abpj.localAnchorA.Set(-0.7f * catapult->arm->getCorrectedWidth(), 0.0f * catapult->arm->getCorrectedHeight());
	abpj.localAnchorB.Set(0.0f * boulder->boulder->getCorrectedWidth(), 0.f * boulder->boulder->getCorrectedHeight());
	abpj.collideConnected = false;
	abpj.referenceAngle = 0.f;
	catapult->boulderJoint = (b2WeldJoint *)world->b2world->CreateJoint(&abpj);
	boulder->catapult = catapult;
}