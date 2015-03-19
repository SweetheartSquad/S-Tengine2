#pragma once

#include <SlayTheDragon.h>
#include <SlayTheDragonContactListener.h>
#include <SlayTheDragonResourceManager.h>
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
#include <PuppetCharacterDragon.h>
#include <PuppetCharacterArcher.h>
#include <PuppetTexturePack.h>
#include <PuppetController.h>
#include <Item.h>
#include <ItemFlail.h>
#include <ItemSimpleWeapon.h>
#include <ItemProjectileWeapon.h>


#include <glfw\glfw3.h>


SlayTheDragon::SlayTheDragon(PuppetGame* _game):
	PuppetScene(_game, 10),
	dragon(new PuppetCharacterDragon(world, PuppetGame::kPLAYER, -1, -20)),
	playerCharacter1(new PuppetCharacterArcher(false, 0, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -1)),
	playerCharacter2(new PuppetCharacterArcher(false, 1, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -2)),
	playerCharacter3(new PuppetCharacterArcher(false, 2, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -3)),
	playerCharacter4(new PuppetCharacterArcher(false, 3, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -4))
{
	cl = new SlayTheDragonContactListener(this);
	/*
	TextureSampler * splashMessageTextureSampler = SlayTheDragonResourceManager::splashMessage;
	splashMessage = new Sprite(nullptr, new Transform());
	splashMessage->transform->scale(glm::vec3(3, 3, 0));
	splashMessage->mesh->pushTexture2D(splashMessageTextureSampler->texture);
	splashMessage->setShader(shader, true);
	splashMessage->transform->scale(-1, 1, 1);
	*/
	players.push_back(playerCharacter1);
	players.push_back(playerCharacter2);
	players.push_back(playerCharacter3);
	players.push_back(playerCharacter4);

	playerCharacter1->setShader(shader, true);
	addChild(playerCharacter1, 1);
	playerCharacter1->addToLayeredScene(this, 1);
	playerCharacter1->rootComponent->maxVelocity = b2Vec2(10, 10);
	static_cast<PuppetGame *>(game)->puppetController1->puppetCharacter = playerCharacter1;

	playerCharacter2->setShader(shader, true);
	addChild(playerCharacter2, 1);
	playerCharacter2->addToLayeredScene(this, 1);
	playerCharacter2->rootComponent->maxVelocity = b2Vec2(10, 10);
	static_cast<PuppetGame *>(game)->puppetController2->puppetCharacter = playerCharacter2;

	playerCharacter3->setShader(shader, true);
	addChild(playerCharacter3, 1);
	playerCharacter3->addToLayeredScene(this, 1);
	playerCharacter3->rootComponent->maxVelocity = b2Vec2(10, 10);
	static_cast<PuppetGame *>(game)->puppetController3->puppetCharacter = playerCharacter3;

	playerCharacter4->setShader(shader, true);
	addChild(playerCharacter4, 1);
	playerCharacter4->addToLayeredScene(this, 1);
	playerCharacter4->rootComponent->maxVelocity = b2Vec2(10, 10);
	static_cast<PuppetGame *>(game)->puppetController4->puppetCharacter = playerCharacter4;

	dragon->setShader(shader, true);
	addChild(dragon, 0);
	dragon->addToLayeredScene(this, 1);
	dragon->rootComponent->maxVelocity = b2Vec2(10, 10);
	dragon->translateComponents(glm::vec3(0, 10, 0));
	
	TextureSampler * emptyWeaponTex = SlayTheDragonResourceManager::itemNone;
	TextureSampler * fireBallTex = SlayTheDragonResourceManager::itemFireball;
	dragon->itemToPickup = new ItemProjectileWeapon(fireBallTex, emptyWeaponTex, world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kGROUND, dragon->groupIndex, 0, 0, -emptyWeaponTex->height);
	addChild(dragon->itemToPickup, 1);
	dragon->itemToPickup->addToLayeredScene(this, 1);
	dragon->itemToPickup->setShader(shader, true);

	gameCam->addTarget(playerCharacter1->torso);
	gameCam->addTarget(playerCharacter2->torso);
	gameCam->addTarget(playerCharacter3->torso);
	gameCam->addTarget(playerCharacter4->torso);
	gameCam->addTarget(dragon->torso);

	playerCharacter3->behaviourManager.addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), playerCharacter3, 10));
	playerCharacter3->behaviourManager.addBehaviour(new BehaviourAttack(playerCharacter3, 3, PuppetGame::kPLAYER));
	playerCharacter3->ai = true;

	playerCharacter4->behaviourManager.addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), playerCharacter4, 10));
	playerCharacter4->behaviourManager.addBehaviour(new BehaviourAttack(playerCharacter4, 3, PuppetGame::kPLAYER));
	playerCharacter4->ai = true;
	
	for(PuppetCharacter * p : players){
		TextureSampler * weaponTex = SlayTheDragonResourceManager::itemBow;
		TextureSampler * projectileTex = SlayTheDragonResourceManager::itemArrow;
		ItemProjectileWeapon * weapon = new ItemProjectileWeapon(projectileTex, weaponTex, world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kBOUNDARY | PuppetGame::kGROUND, p->groupIndex, 0, 0, -weaponTex->height);
		weapon->addToLayeredScene(this, 1);
		weapon->setShader(shader, true);
		p->itemToPickup = weapon;
	}

	playerCharacter1->translateComponents(glm::vec3(20.0f, 35, 0.f));
	playerCharacter2->translateComponents(glm::vec3(40.0f, 35, 0.f));
	playerCharacter3->translateComponents(glm::vec3(60.0f, 35, 0.f));
	playerCharacter4->translateComponents(glm::vec3(80.0f, 35, 0.f));

	playRandomBackgroundMusic();
}

SlayTheDragon::~SlayTheDragon(){
}

void SlayTheDragon::update(Step* _step){
	PuppetScene::update(_step);
	if(keyboard->keyDown(GLFW_KEY_B)){
		playerCharacter1->control = 0;
		playerCharacter2->control = 0;
		//playerCharacter->behaviourManager.behaviours.at(0)->targets.clear();
		//playerCharacter->behaviourManager.behaviours.at(0)->active = false;
	}

	if(keyboard->keyJustDown(GLFW_KEY_W)){
		playerCharacter1->jump();
	}if(keyboard->keyDown(GLFW_KEY_A)){
		playerCharacter1->targetRoll = glm::radians(-45.f);
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		playerCharacter1->targetRoll = glm::radians(45.f);
	}
	if(keyboard->keyJustDown(GLFW_KEY_T)){
		playerCharacter1->action();
	}
}

void SlayTheDragon::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	PuppetScene::render(_matrixStack, renderOptions);
}

void SlayTheDragon::load(){
	PuppetScene::load();
}

void SlayTheDragon::unload(){
	PuppetScene::unload();
}