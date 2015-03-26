#pragma once

#include <RaidTheCastle.h>
#include <RaidTheCastleContactListener.h>
#include <RaidTheCastleResourceManager.h>
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
#include <TextureSampler.h>
#include <PuppetCharacterCastleChampion.h>
#include <PuppetCharacterKnight.h>
#include <PuppetTexturePack.h>
#include <PuppetController.h>
#include <Item.h>
#include <ItemFlail.h>
#include <ItemSimpleWeapon.h>
#include <ItemProjectileWeapon.h>
#include <SoundManager.h>
#include <glfw\glfw3.h>


RaidTheCastle::RaidTheCastle(PuppetGame* _game):
	PuppetScene(_game, 30),
	castle(new Castle(world, PuppetGame::kSTRUCTURE, PuppetGame::kITEM, 30)),
	catapult(new Catapult(world, PuppetGame::kSTRUCTURE, PuppetGame::kITEM | PuppetGame::kPLAYER, -10)),
	champion(new PuppetCharacterCastleChampion(world, PuppetGame::kPLAYER, -1, -20)),
	playerCharacter1(new PuppetCharacterKnight(false, 0, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -1)),
	playerCharacter2(new PuppetCharacterKnight(false, 1, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -2)),
	playerCharacter3(new PuppetCharacterKnight(false, 2, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -3)),
	playerCharacter4(new PuppetCharacterKnight(false, 3, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -4))
{
	populateBackground();
	cl = new RaidTheCastleContactListener(this);

	TextureSampler * splashMessageTextureSampler = RaidTheCastleResourceManager::splashMessage;
	splashMessage = new Sprite(nullptr, new Transform());
	splashMessage->transform->scale(glm::vec3(3, 3, 0));
	splashMessage->mesh->pushTexture2D(splashMessageTextureSampler->texture);
	splashMessage->setShader(shader, true);
	splashMessage->transform->scale(-1, 1, 1);

	players.push_back(playerCharacter1);
	players.push_back(playerCharacter2);
	players.push_back(playerCharacter3);
	players.push_back(playerCharacter4);

	playerCharacter1->setShader(shader, true);
	addChild(playerCharacter1, 1);
	playerCharacter1->addToLayeredScene(this, 1);
	playerCharacter1->rootComponent->maxVelocity = b2Vec2(10, NO_VELOCITY_LIMIT);
	static_cast<PuppetGame *>(game)->puppetControllers.at(0)->setPuppetCharacter(playerCharacter1);

	playerCharacter2->setShader(shader, true);
	addChild(playerCharacter2, 1);
	playerCharacter2->addToLayeredScene(this, 1);
	playerCharacter2->rootComponent->maxVelocity = b2Vec2(10, NO_VELOCITY_LIMIT);
	static_cast<PuppetGame *>(game)->puppetControllers.at(1)->setPuppetCharacter(playerCharacter2);

	playerCharacter3->setShader(shader, true);
	addChild(playerCharacter3, 1);
	playerCharacter3->addToLayeredScene(this, 1);
	playerCharacter3->rootComponent->maxVelocity = b2Vec2(10, NO_VELOCITY_LIMIT);
	static_cast<PuppetGame *>(game)->puppetControllers.at(2)->setPuppetCharacter(playerCharacter3);

	playerCharacter4->setShader(shader, true);
	addChild(playerCharacter4, 1);
	playerCharacter4->addToLayeredScene(this, 1);
	playerCharacter4->rootComponent->maxVelocity = b2Vec2(10, NO_VELOCITY_LIMIT);
	static_cast<PuppetGame *>(game)->puppetControllers.at(3)->setPuppetCharacter(playerCharacter4);

	champion->setShader(shader, true);
	addChild(champion, 0);
	champion->addToLayeredScene(this, 1);
	champion->rootComponent->maxVelocity = b2Vec2(10, NO_VELOCITY_LIMIT);
	
	champion->itemToPickup = new ItemFlail(world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kGROUND, champion->groupIndex, 0, 0, -RaidTheCastleResourceManager::itemFlailGrip->height/2.f);
	addChild(champion->itemToPickup, 1);
	champion->itemToPickup->addToLayeredScene(this, 1);
	champion->itemToPickup->setShader(shader, true);

	gameCam->addTarget(playerCharacter1->torso);
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
	
	playerCharacter3->behaviourManager.addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), playerCharacter3, 10));
	playerCharacter3->behaviourManager.addBehaviour(new BehaviourAttack(playerCharacter3, 3, PuppetGame::kPLAYER));
	playerCharacter3->ai = true;

	playerCharacter4->behaviourManager.addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), playerCharacter4, 10));
	playerCharacter4->behaviourManager.addBehaviour(new BehaviourAttack(playerCharacter4, 3, PuppetGame::kPLAYER));
	playerCharacter4->ai = true;

	//gameCam->addTarget(castle->rootComponent);
	//gameCam->addTarget(catapult->rootComponent);

	/*for(PuppetCharacter * p : players){
		TextureSampler * weaponTex = RaidTheCastleResourceManager::getRandomWeapon();
		ItemSimpleWeapon * weapon = new ItemSimpleWeapon(weaponTex, false, world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kBOUNDARY | PuppetGame::kGROUND, p->groupIndex, 0, 0, -weaponTex->height);
		weapon->addToLayeredScene(this, 1);
		weapon->setShader(shader, true);
		p->itemToPickup = weapon;
	}*/
	for(PuppetCharacter * p : players){
		TextureSampler * weaponTex = RaidTheCastleResourceManager::getRandomWeapon();
		TextureSampler * projTex = RaidTheCastleResourceManager::getRandomWeapon();
		
		//ItemProjectileWeapon * weapon = new ItemProjectileWeapon(projTex, weaponTex, world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kBOUNDARY | PuppetGame::kGROUND, p->groupIndex, 0, 0, -weaponTex->height);
        ItemSimpleWeapon * weapon = new ItemSimpleWeapon(weaponTex, false, world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kBOUNDARY | PuppetGame::kGROUND, p->groupIndex, 0, 0, -weaponTex->height);

        weapon->addToLayeredScene(this, 1);
		weapon->setShader(shader, true);
		p->itemToPickup = weapon;
        addChild(weapon, 1);
	}
	
	playerCharacter1->translateComponents(glm::vec3(20.0f, 35, 0.f));
	playerCharacter2->translateComponents(glm::vec3(40.0f, 35, 0.f));
	playerCharacter3->translateComponents(glm::vec3(60.0f, 35, 0.f));
	playerCharacter4->translateComponents(glm::vec3(80.0f, 35, 0.f));
	champion->translateComponents(glm::vec3(100.f, 35, 0));

	playRandomBackgroundMusic();
	loadCatapult();
}

RaidTheCastle::~RaidTheCastle(){
}

void RaidTheCastle::update(Step* _step){
	PuppetScene::update(_step);
	/*if(keyboard->keyDown(GLFW_KEY_F)){
		catapult->fireCatapult();
	}*/

	if(!splashSoundPlayed){
		PuppetResourceManager::splashSounds->play("RaidTheCastle");
		splashSoundPlayed = true;
	}

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