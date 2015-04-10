#pragma once

#include <FightYourFriends.h>
#include <RaidTheCastleContactListener.h>
#include <FightYourFriendsResourceManager.h>
#include <Castle.h>
#include <PuppetGame.h>
#include <PuppetCharacter.h>
#include <FollowCamera.h>
#include <BehaviourManager.h>
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
#include <ParticleSystem.h>
#include <Item.h>
#include <ItemFlail.h>
#include <ItemSimpleWeapon.h>
#include <ItemProjectileWeapon.h>
#include <SoundManager.h>
#include <glfw\glfw3.h>
#include <NumberUtils.h>
#include <Particle.h>

FightYourFriends::FightYourFriends(PuppetGame* _game):
	PuppetScene(_game, 30),
	playerCharacter1(new PuppetCharacterKnight(false, 0, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -1)),
	playerCharacter2(new PuppetCharacterKnight(false, 1, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -2)),
	playerCharacter3(new PuppetCharacterKnight(false, 2, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -3)),
	playerCharacter4(new PuppetCharacterKnight(false, 3, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -4)),
	camTargetsRemoved(false)
{
	ghostPosition = 12.0f;
	populateBackground();
	cl = new PuppetContactListener(this);

	TextureSampler * splashMessageTextureSampler = FightYourFriendsResourceManager::splashMessage;
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
	static_cast<PuppetGame *>(game)->puppetControllers.at(0)->setPuppetCharacter(playerCharacter1);
	playerCharacter1->createIndicator(playerCharacter1->id);;

	playerCharacter2->setShader(shader, true);
	addChild(playerCharacter2, 1);
	playerCharacter2->addToLayeredScene(this, 1);
	static_cast<PuppetGame *>(game)->puppetControllers.at(1)->setPuppetCharacter(playerCharacter2);
	playerCharacter2->createIndicator(playerCharacter2->id);

	playerCharacter3->setShader(shader, true);
	addChild(playerCharacter3, 1);
	playerCharacter3->addToLayeredScene(this, 1);
	static_cast<PuppetGame *>(game)->puppetControllers.at(2)->setPuppetCharacter(playerCharacter3);
	playerCharacter3->createIndicator(playerCharacter3->id);

	playerCharacter4->setShader(shader, true);
	addChild(playerCharacter4, 1);
	playerCharacter4->addToLayeredScene(this, 1);
	static_cast<PuppetGame *>(game)->puppetControllers.at(3)->setPuppetCharacter(playerCharacter4);
	playerCharacter4->createIndicator(playerCharacter4->id);

	gameCam->addTarget(playerCharacter1->torso);
	gameCam->addTarget(playerCharacter2->torso);
	gameCam->addTarget(playerCharacter3->torso);
	gameCam->addTarget(playerCharacter4->torso);

	//playerCharacter3->behaviourManager->addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), playerCharacter3, 10));
	//playerCharacter3->behaviourManager->addBehaviour(new BehaviourAttack(playerCharacter3, 3, PuppetGame::kPLAYER));
	//playerCharacter3->ai = true;

	//playerCharacter4->behaviourManager->addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), playerCharacter4, 10));
	//playerCharacter4->behaviourManager->addBehaviour(new BehaviourAttack(playerCharacter4, 3, PuppetGame::kPLAYER));
	//playerCharacter4->ai = true;

	for(PuppetCharacter * p : players){
		TextureSampler * weaponTex = PuppetResourceManager::getRandomMeleeWeapon();
		TextureSampler * projTex = PuppetResourceManager::getRandomMeleeWeapon();
		
		//ItemProjectileWeapon * weapon = new ItemProjectileWeapon(projTex, weaponTex, world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kBOUNDARY | PuppetGame::kGROUND, p->groupIndex, 0, 0, -weaponTex->height);
        ItemSimpleWeapon * weapon = new ItemSimpleWeapon(weaponTex, false, world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kBOUNDARY | PuppetGame::kGROUND, p->groupIndex, 1, 0, -weaponTex->height);

        weapon->addToLayeredScene(this, 1);
		weapon->setShader(shader, true);
		p->itemToPickup = weapon;
        addChild(weapon, 1);
	}
	
	playerCharacter1->translateComponents(glm::vec3(20.0f, 35, 0.f));
	playerCharacter2->translateComponents(glm::vec3(40.0f, 35, 0.f));
	playerCharacter3->translateComponents(glm::vec3(60.0f, 35, 0.f));
	playerCharacter4->translateComponents(glm::vec3(80.0f, 35, 0.f));

	playRandomBackgroundMusic();

	populateClouds();
}

FightYourFriends::~FightYourFriends(){
}

void FightYourFriends::update(Step* _step){
	PuppetScene::update(_step);

	if(!splashSoundPlayed){
		PuppetResourceManager::splashSounds->play("FightYourFriends");
		splashSoundPlayed = true;
	}
}

void FightYourFriends::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	PuppetScene::render(_matrixStack, renderOptions);
}

void FightYourFriends::load(){
	PuppetScene::load();
}

void FightYourFriends::unload(){
	PuppetScene::unload();
}