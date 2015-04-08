#pragma once

#include <SlayTheDragon.h>
#include <SlayTheDragonContactListener.h>
#include <SlayTheDragonResourceManager.h>
#include <PuppetGame.h>
#include <Fortification.h>
#include <PuppetCharacter.h>
#include <FollowCamera.h>
#include <BehaviourManager.h>
#include <Behaviour.h>
#include <BehaviourFollow.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>
#include <BehaviourAttackThrow.h>
#include <Box2DSprite.h>
#include <Box2DWorld.h>
#include <MeshEntity.h>
#include <MeshInterface.h>
#include <MeshFactory.h>
#include <shader/BaseComponentShader.h>
#include <keyboard.h>
#include <Texture.h>
#include <TextureSampler.h>
#include <PuppetCharacterDragon.h>
#include <PuppetCharacterArcher.h>
#include <PuppetTexturePack.h>
#include <PuppetController.h>
#include <ItemSimpleWeapon.h>
#include <ItemProjectileWeapon.h>
#include <ItemFireballLauncher.h>

#include <glfw\glfw3.h>
#include <SoundManager.h>
#include <Box2DMeshEntity.h>


SlayTheDragon::SlayTheDragon(PuppetGame* _game):
	PuppetScene(_game, 30, 170.f, 120.f),
	fort(new Fortification(world, PuppetGame::kSTRUCTURE, PuppetGame::kITEM | PuppetGame::kPLAYER)),
    fortForeground(new Box2DSprite(world, SlayTheDragonResourceManager::fortForeground, b2_staticBody, false, nullptr, new Transform(), 0.03f)),
	playerCharacter1(new PuppetCharacterArcher(false, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY | PuppetGame::kDEAD_ZONE, -1)),
	playerCharacter2(new PuppetCharacterArcher(false, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY | PuppetGame::kDEAD_ZONE, -2)),
	playerCharacter3(new PuppetCharacterArcher(false, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY | PuppetGame::kDEAD_ZONE, -3)),
	playerCharacter4(new PuppetCharacterDragon(false, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -4))
{
	ghostPosition = 30.0f;
	cl = new SlayTheDragonContactListener(this);
	
	populateBackground();
	
	Sprite * fortBg = new Sprite();
	fort->addToLayeredScene(this, 1);
	addChild(fortBg, 0);
	fortBg->setShader(shader, true);
	fortBg->pushTextureSampler(SlayTheDragonResourceManager::fortBackground);
	fortBg->transform->translate(65.0f, 20.0f, 0.0f);
	fortBg->transform->scale(2.6f, 2.6f, 1.0f);

	splashMessage = new Sprite(nullptr, new Transform());
	splashMessage->transform->scale(glm::vec3(3, 3, 0));
	splashMessage->mesh->pushTexture2D(SlayTheDragonResourceManager::splashMessage->texture);
	splashMessage->setShader(shader, true);
	splashMessage->transform->scale(-1, 1, 1);

	players.push_back(playerCharacter1);
	players.push_back(playerCharacter2);
	players.push_back(playerCharacter3);
	players.push_back(playerCharacter4);


    b2Filter sf;
    sf.categoryBits = PuppetGame::kGROUND;
    sf.groupIndex = 0;
    sf.maskBits = PuppetGame::kITEM | PuppetGame::kPLAYER;

    fortForeground->createFixture(sf, b2Vec2(0, 0), fortForeground);
    fortForeground->setShader(shader, true);
    addChild(fortForeground, 2);

    fortForeground->setTranslationPhysical(glm::vec3(0, fortForeground->getCorrectedHeight(), 0));


    fort->translateComponents(glm::vec3(0.f, fortForeground->getCorrectedHeight() + 25.f, 0.f));

	playerCharacter1->setShader(shader, true);
	addChild(playerCharacter1, 1);
	playerCharacter1->addToLayeredScene(this, 1);
	static_cast<PuppetGame *>(game)->puppetControllers.at(0)->setPuppetCharacter(playerCharacter1);
	playerCharacter1->createIndicator(playerCharacter1->id);

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
	//playerCharacter3->behaviourManager->addBehaviour(new BehaviourAttackThrow(false, playerCharacter3, 3, PuppetGame::kPLAYER));
	//playerCharacter3->ai = true;

	//->ai = true;
	
	dragon = static_cast<PuppetCharacterDragon * >(playerCharacter4);

	TextureSampler * bowTex = SlayTheDragonResourceManager::itemBow;
	TextureSampler * arrowTex = SlayTheDragonResourceManager::itemArrow;
	TextureSampler * fireballTex = SlayTheDragonResourceManager::itemFireball;

	unsigned int pCnt = 0;
	for(PuppetCharacter * p : players){
		ItemProjectileWeapon * weapon;
		if(p == dragon){
			weapon = new ItemFireballLauncher(dragon, world);
		}else{
			weapon = new ItemProjectileWeapon(arrowTex, bowTex, world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kBOUNDARY | PuppetGame::kGROUND | PuppetGame::kSTRUCTURE, p->groupIndex, 1, 0, -bowTex->height);
		}
		weapon->addToLayeredScene(this, 1);
		weapon->setShader(shader, true);
		p->itemToPickup = weapon;
		addChild(weapon, 1);
		p->translateComponents(glm::vec3(40.f + (pCnt * 20), fortForeground->getCorrectedHeight() * 2, 0));
	}
	
	//dragon->translateComponents(glm::vec3(0, 100, 0));

	//playerCharacter1->translateComponents(glm::vec3(40.0f, fort->rootComponent->getCorrectedHeight() * 2, 0.f));
	//playerCharacter2->translateComponents(glm::vec3(40.f, fort->rootComponent->getCorrectedHeight() * 2, 0));
	playerCharacter1->translateComponents(glm::vec3(20.f, fort->rootComponent->getCorrectedHeight() * 1.2, 0.f));
	playerCharacter2->translateComponents(glm::vec3(30.f, fort->rootComponent->getCorrectedHeight() * 1.2, 0.f));
	playerCharacter3->translateComponents(glm::vec3(40.f, fort->rootComponent->getCorrectedHeight() * 1.2, 0.f));
	//dragon->translateComponents(glm::vec3(0.f, fort->rootComponent->getPos().y + fort->rootComponent->getCorrectedHeight() + fort->roof->getPos().y + fort->rootComponent->getCorrectedHeight() + 10.f, 0.f));

	fort->setShader(shader, true);
	fort->addToLayeredScene(this, 1);
	addChild(fort, 1);
    fort->translateComponents(glm::vec3(80.0f, 0.f, 0.f));
    fortForeground->setTranslationPhysical(glm::vec3(80.0f, 0.f, 0.f), true);

	Box2DMeshEntity * deathBounds = new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody);
	deathBounds->transform->scale(100.f, 18.f, 1.f);
	deathBounds->setTranslationPhysical(60.f, -18.f, 0.f);
	world->addToWorld(deathBounds);
	b2Filter sfd;
	sfd.categoryBits = PuppetGame::kDEAD_ZONE;
	//sfd.maskBits = -1;
	deathBounds->body->GetFixtureList()->SetFilterData(sfd);
	addChild(deathBounds, 2);
	deathBounds->setShader(shader, true);

	playRandomBackgroundMusic();

	populateClouds();
}

SlayTheDragon::~SlayTheDragon(){
	delete ground; // have to do this here because the ground is never added and therefore not deleted as one of the scene children
}

void SlayTheDragon::update(Step* _step){
	PuppetScene::update(_step);

	if(playerCharacter1->torso->transform->getTranslationVector().y < 25.0f && gameCam->hasTarget(playerCharacter1->torso)){
		gameCam->removeTarget(playerCharacter1->torso);
	}else if(!gameCam->hasTarget(playerCharacter1->torso) && playerCharacter1->torso->transform->getTranslationVector().y > 25.0f){
		gameCam->addTarget(playerCharacter1->torso);
	}

	if(playerCharacter2->torso->transform->getTranslationVector().y < 25.0f && gameCam->hasTarget(playerCharacter2->torso)){
		gameCam->removeTarget(playerCharacter2->torso);
	}else if(!gameCam->hasTarget(playerCharacter2->torso) && playerCharacter2->torso->transform->getTranslationVector().y > 25.0f){
		gameCam->addTarget(playerCharacter2->torso);
	}

	if(playerCharacter3->torso->transform->getTranslationVector().y < 25.0f && gameCam->hasTarget(playerCharacter3->torso)){
		gameCam->removeTarget(playerCharacter3->torso);
	}else if(!gameCam->hasTarget(playerCharacter3->torso) && playerCharacter3->torso->transform->getTranslationVector().y > 25.0f){
		gameCam->addTarget(playerCharacter3->torso);
	}

	if(!splashSoundPlayed){
		PuppetResourceManager::splashSounds->play("SlayTheDragon");
		splashSoundPlayed = true;
	}

    // handle archer's victory state
    if (dragon->dead){
        triggerVictoryState();
    }
    else{
        // handle dragon victory
        bool archersDead = true;
        for (unsigned long int i = 0; i < players.size() - 1; ++i){
            if (!players.at(i)->dead){
                archersDead = false;
                break;
            }
        }
        if (archersDead){
            dragon->score += 100000;
            triggerVictoryState();
        }
    }
}

void SlayTheDragon::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	PuppetScene::render(_matrixStack, renderOptions);
}

void SlayTheDragon::load(){
	PuppetScene::load();
}

void SlayTheDragon::unload(){
	PuppetScene::unload();
}

void SlayTheDragon::populateBackground(){
}