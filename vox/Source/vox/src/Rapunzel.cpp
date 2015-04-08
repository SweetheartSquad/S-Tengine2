#pragma once

#include <Rapunzel.h>
#include <RapunzelContactListener.h>
#include <RapunzelResourceManager.h>
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
#include <PuppetCharacterGuard.h>
#include <PuppetCharacterRapunzel.h>
#include <PuppetCharacterThief.h>
#include <PuppetTexturePack.h>
#include <PuppetController.h>
#include <Item.h>
#include <ItemFlail.h>
#include <ItemSimpleWeapon.h>
#include <ItemProjectileWeapon.h>
#include <Hair.h>
#include <Lever.h>
#include <ItemGold.h>
#include <NumberUtils.h>
#include <StructureBoxingGlove.h>

#include <glfw\glfw3.h>
#include <SoundManager.h>
#include <StructureGoldPile.h>

Rapunzel::Rapunzel(PuppetGame* _game):
	PuppetScene(_game, 50.f, 100.f),
	tower(new Box2DSprite(world, RapunzelResourceManager::towerTower, b2_staticBody, false, nullptr, new Transform(), 0.03f)),
	castleCatwalk(new Box2DSprite(world, RapunzelResourceManager::towerCatwalk, b2_staticBody, false, nullptr, new Transform(), 0.03f)),
	guard(new PuppetCharacterGuard(true, RAPUNZEL_GHOST_HEIGHT, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -20)),
	playerCharacter1(new PuppetCharacterThief(false, RAPUNZEL_GHOST_HEIGHT, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -1)),
	playerCharacter2(new PuppetCharacterThief(false, RAPUNZEL_GHOST_HEIGHT, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -2)),
	playerCharacter3(new PuppetCharacterThief(false, RAPUNZEL_GHOST_HEIGHT, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -3)),
	playerCharacter4(new PuppetCharacterRapunzel(false, RAPUNZEL_GHOST_HEIGHT, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -4))
{
	populateBackground();
	cl = new RapunzelContactListener(this);

	Sprite * towerBg = new Sprite();
	addChild(towerBg, 0);
	towerBg->setShader(shader, true);
	towerBg->pushTextureSampler(RapunzelResourceManager::towerBackground);
	towerBg->transform->translate(2.f, 0.f, 0.0f);
	towerBg->transform->scale(2.6f, 2.6f, 1.0f);

	TextureSampler * splashMessageTextureSampler = RapunzelResourceManager::splashMessage;
	splashMessage = new Sprite(nullptr, new Transform());
	splashMessage->transform->scale(glm::vec3(3, 3, 0));
	splashMessage->mesh->pushTexture2D(splashMessageTextureSampler->texture);
	splashMessage->setShader(shader, true);
	splashMessage->transform->scale(-1, 1, 1);

	players.push_back(playerCharacter1);
	players.push_back(playerCharacter2);
	players.push_back(playerCharacter3);
	players.push_back(playerCharacter4);

	b2Filter tf;
    tf.categoryBits = PuppetGame::kGROUND;
    tf.groupIndex = 0;
    tf.maskBits = 0;

	b2Filter sf;
    sf.categoryBits = PuppetGame::kGROUND;
    sf.groupIndex = 0;
    sf.maskBits = PuppetGame::kITEM | PuppetGame::kPLAYER;

	tower->createFixture(tf, b2Vec2(0, 0), tower);
    tower->setShader(shader, true);
    addChild(tower, 1);

	castleCatwalk->createFixture(sf, b2Vec2(0, 0), castleCatwalk);
    castleCatwalk->setShader(shader, true);
    addChild(castleCatwalk, 2);

	playerCharacter1->setShader(shader, true);
	addChild(playerCharacter1, 1);
	playerCharacter1->addToLayeredScene(this, 1);
	static_cast<PuppetGame *>(game)->puppetControllers.at(0)->setPuppetCharacter(playerCharacter1);

	playerCharacter2->setShader(shader, true);
	addChild(playerCharacter2, 1);
	playerCharacter2->addToLayeredScene(this, 1);
	static_cast<PuppetGame *>(game)->puppetControllers.at(1)->setPuppetCharacter(playerCharacter2);

	playerCharacter3->setShader(shader, true);
	addChild(playerCharacter3, 1);
	playerCharacter3->addToLayeredScene(this, 1);
	static_cast<PuppetGame *>(game)->puppetControllers.at(2)->setPuppetCharacter(playerCharacter3);

	playerCharacter4->setShader(shader, true);
	addChild(playerCharacter4, 1);
	playerCharacter4->addToLayeredScene(this, 1);
	static_cast<PuppetGame *>(game)->puppetControllers.at(3)->setPuppetCharacter(playerCharacter4);
	// start Rapunzel with a ridiculous score so if the time runs out they win regardless of how well the thieves play
	playerCharacter4->score = 1000000;
	playerCharacter4->lastUpdateScore = 1000000; 

	guard->setShader(shader, true);
	addChild(guard, 0);
	guard->addToLayeredScene(this, 1);
	
	/*guard->itemToPickup = new ItemFlail(world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kGROUND, guard->groupIndex, 0, 0, -RapunzelResourceManager::itemFlailGrip->height/2.f);
	addChild(guard->itemToPickup, 1);
	guard->itemToPickup->addToLayeredScene(this, 1);
	guard->itemToPickup->setShader(shader, true);*/

	gameCam->addTarget(playerCharacter1->torso);
	gameCam->addTarget(playerCharacter2->torso);
	gameCam->addTarget(playerCharacter3->torso);
	gameCam->addTarget(playerCharacter4->torso);

	//playerCharacter3->behaviourManager->addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), playerCharacter3, 10));
	//playerCharacter3->behaviourManager->addBehaviour(new BehaviourAttack(playerCharacter3, 3, PuppetGame::kPLAYER));
	//playerCharacter3->ai = true;

	//playerCharacter4->behaviourManager->addBehaviour(new BehaviourPatrol(glm::vec3(50, 0, 0), glm::vec3(100, 0, 0), playerCharacter4, 10));
	//playerCharacter4->behaviourManager->addBehaviour(new BehaviourAttack(playerCharacter4, 3, PuppetGame::kPLAYER));
	//playerCharacter4->ai = true;

	guard->behaviourManager->addBehaviour(new BehaviourPatrol(glm::vec3(0, 0, 0), glm::vec3(40.f, 0, 0), guard, 10));
	guard->behaviourManager->addBehaviour(new BehaviourAttack(guard, 3, PuppetGame::kPLAYER));
	guard->ai = true;

	for(PuppetCharacter * p : players){
		if(p != playerCharacter4){
			TextureSampler * weaponTex = RapunzelResourceManager::getRandomWeapon();
			ItemSimpleWeapon * weapon = new ItemSimpleWeapon(weaponTex, false, world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kBOUNDARY | PuppetGame::kGROUND, p->groupIndex, 1, 0, -weaponTex->height);
			weapon->addToLayeredScene(this, 1);
			weapon->setShader(shader, true);
			p->itemToPickup = weapon;
			addChild(weapon, 1);
		}
	}
	
	tower->setTranslationPhysical(glm::vec3(glm::vec3(60.f, tower->getCorrectedHeight(), 0.f)));
    castleCatwalk->setTranslationPhysical(glm::vec3(tower->getPos().x - castleCatwalk->getCorrectedWidth() - tower->getCorrectedWidth() * 0.75f, tower->getPos().y - tower->getCorrectedHeight() * 0.08, 0));
	
	playerCharacter1->translateComponents(glm::vec3(10.f, 5.f, 0.f));
	playerCharacter2->translateComponents(glm::vec3(20.f, 5.f, 0.f));
	playerCharacter3->translateComponents(glm::vec3(30.f, 5.f, 0.f));
	playerCharacter4->translateComponents(glm::vec3(castleCatwalk->getPos().x, tower->getPos().y + tower->getCorrectedHeight(), 0.f));
	guard->translateComponents(glm::vec3(50.f, 5.f, 0.f));

	Hair * hair = new Hair(world, PuppetGame::kGROUND, PuppetGame::kPLAYER, 0);
	addChild(hair, 1);
	hair->setShader(shader, true);
	hair->translateComponents(glm::vec3(tower->getPos().x, tower->getPos().y - tower->getCorrectedHeight() * 0.75, 0.f));
	
	lever1 = new Lever(world, PuppetGame::kSTRUCTURE, PuppetGame::kPLAYER, 0);
	addChild(lever1, 1);
	lever1->addToLayeredScene(this, 1);
	lever1->setShader(shader, true);
	lever1->translateComponents(glm::vec3(5.f, 2.f, 0));
	lever1->type = 1;
	
	lever2 = new Lever(world, PuppetGame::kSTRUCTURE, PuppetGame::kPLAYER, 0);
	addChild(lever2, 1);
	lever2->addToLayeredScene(this, 1);
	lever2->setShader(shader, true);
	lever2->translateComponents(glm::vec3(10.f, 2.f, 0));
	lever2->type = 2;

	lever3 = new Lever(world, PuppetGame::kSTRUCTURE, PuppetGame::kPLAYER, 0);
	addChild(lever3, 1);
	lever3->addToLayeredScene(this, 1);
	lever3->setShader(shader, true);
	lever3->translateComponents(glm::vec3(15.f, 2.f, 0));
	lever3->type = 3;

	glove = new StructureBoxingGlove(world);
	glove->translateComponents(glm::vec3(50.f, 25.f, 0.f));
	addChild(glove, 1);
	glove->setShader(shader, true);
	glove->addToLayeredScene(this, 1);

	goldPile = new StructureGoldPile(world);
	addChild(goldPile, 1);
	goldPile->setShader(shader, true);
	goldPile->addToLayeredScene(this, 1);
	//Uncomment to place on castle
	//goldPile->translateComponents(glm::vec3(15.f, 23.f, 0.f));
	goldPile->translateComponents(glm::vec3(30.f, 2.f, 0.f));
	goldPile->rootComponent->transform->scale(10.0f, 10.0f, 1.0f);
	gameCam->addTarget(goldPile);
	playRandomBackgroundMusic();
}

Rapunzel::~Rapunzel(){
}

void Rapunzel::update(Step* _step){
	PuppetScene::update(_step);

	if(keyboard->keyJustDown(GLFW_KEY_N)){
		goldPile->actuallyInteract();
	}

	if(!splashSoundPlayed){
		PuppetResourceManager::splashSounds->play("Rapunzel");
		splashSoundPlayed = true;
	}

	// trigger victory (thieves should win, since gold is gone)
	if(goldPile->goldTaken >= goldPile->totalGold){
		playerCharacter4->score = 0;
        triggerVictoryState();
	}
}

void Rapunzel::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	PuppetScene::render(_matrixStack, renderOptions);
}

void Rapunzel::load(){
	PuppetScene::load();
}

void Rapunzel::unload(){
	PuppetScene::unload();
}