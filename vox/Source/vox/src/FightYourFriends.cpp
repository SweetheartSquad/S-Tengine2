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
#include <Resource.h>

FightYourFriends::FightYourFriends(PuppetGame* _game):
	PuppetScene(_game, 30, 50.f, 50.f),
	camTargetsRemoved(false),
	itemDropFreq(0.0025f),
	playerCharacter1(new PuppetCharacterKnight(false, 0, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -1)),
	playerCharacter2(new PuppetCharacterKnight(false, 1, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -2)),
	playerCharacter3(new PuppetCharacterKnight(false, 2, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -3)),
	playerCharacter4(new PuppetCharacterKnight(false, 3, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -4))
{
	ghostPosition = 12.0f;
	cl = new PuppetContactListener(this);
	cl->damageScoreMult = 1.f;

	TextureSampler * splashMessageTextureSampler = FightYourFriendsResourceManager::splashMessage;
	splashMessage = new Sprite(nullptr, new Transform());
	splashMessage->mesh->pushTexture2D(splashMessageTextureSampler->texture);
	splashMessage->setShader(shader, true);
	splashMessage->transform->translate(1920.f*0.5, 1080.f*0.5f, 0);

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

	for(PuppetCharacter * p : players){
		TextureSampler * weaponTex = PuppetResourceManager::getRandomMeleeWeapon();
		ItemSimpleWeapon * weapon = new ItemSimpleWeapon(weaponTex, false, world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kBOUNDARY | PuppetGame::kGROUND, p->groupIndex, 1, 0, -weaponTex->height);

        weapon->addToLayeredScene(this, 1);
		weapon->setShader(shader, true);
		p->itemToPickup = weapon;
        addChild(weapon, 1);
	}
	
	playerCharacter1->translateComponents(glm::vec3(sceneWidth/4 * 0 + sceneWidth/8, sceneHeight*0.75f, 0.f));
	playerCharacter2->translateComponents(glm::vec3(sceneWidth/4 * 1 + sceneWidth/8, sceneHeight*0.75f, 0.f));
	playerCharacter3->translateComponents(glm::vec3(sceneWidth/4 * 2 + sceneWidth/8, sceneHeight*0.75f, 0.f));
	playerCharacter4->translateComponents(glm::vec3(sceneWidth/4 * 3 + sceneWidth/8, sceneHeight*0.75f, 0.f));

	playRandomBackgroundMusic();
	
	populateBackground();
	populateClouds();

	gameCam->addTarget(playerCharacter1->torso);
	gameCam->addTarget(playerCharacter2->torso);
	gameCam->addTarget(playerCharacter3->torso);
	gameCam->addTarget(playerCharacter4->torso);

	gameCam->useBounds = true;
	gameCam->minBounds.y = -2.f;
	gameCam->minBounds.height = sceneHeight;
}

FightYourFriends::~FightYourFriends(){
}

void FightYourFriends::update(Step* _step){
	PuppetScene::update(_step);

	if(!splashSoundPlayed){
		PuppetResourceManager::splashSounds->play("FightYourFriends");
		splashSoundPlayed = true;
	}

	// randomly drop weapons
	if(vox::NumberUtils::randomFloat(0.f, 1.f) <= itemDropFreq){
		TextureSampler * weaponTex = PuppetResourceManager::getRandomMeleeWeapon();
		ItemSimpleWeapon * weapon = new ItemSimpleWeapon(weaponTex, false, world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kBOUNDARY | PuppetGame::kGROUND, 0, 1, 0, -weaponTex->height);

        weapon->addToLayeredScene(this, 1);
		weapon->setShader(shader, true);
		weapon->translateComponents(vox::NumberUtils::randomFloat(sceneWidth*0.1f, sceneWidth*0.9f), sceneHeight*0.8f, 0);
		addChild(weapon, 1);
	}

	// handle last one standing
    PuppetCharacter * p;
	unsigned int survivors = 0;
    for (unsigned long int i = 0; i < players.size(); ++i){
        if (!players.at(i)->dead){
            ++survivors;
            p = players.at(i);
        }
    }
    if(survivors == 1){
        p->score += 100000;
		p->lastUpdateScore = p->score;
        triggerVictoryState();
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

void FightYourFriends::populateBackground(){
	stageFloor = new MeshEntity(Resource::loadMeshFromObj("../assets/hurly-burly/stageFloor.vox"));
	stageFront = new MeshEntity(Resource::loadMeshFromObj("../assets/hurly-burly/stageFront.vox"));

	addChild(stageFloor, 0);
	addChild(stageFront, 0);
	
	stageFloor->setShader(shader, true);
	stageFloor->transform->scale(1000, 100, 100);
	stageFloor->transform->translate(50.f / 2.f, 0, -15.f / 2.f);
	stageFloor->mesh->uvEdgeMode = GL_REPEAT;
	stageFloor->mesh->pushTexture2D(PuppetResourceManager::stageFloor);
	for (Vertex & v : stageFloor->mesh->vertices){
		v.u *= 10;
		v.v *= 100;
	}
	stageFloor->mesh->dirty = true;


	stageFront->setShader(shader, true);
	stageFront->transform->scale(1000, 100, 100);
	stageFront->transform->translate(50.f / 2.f, 0, -15.f / 2.f);
	stageFront->mesh->uvEdgeMode = GL_REPEAT;
	stageFront->mesh->pushTexture2D(PuppetResourceManager::stageFront);
	for (Vertex & v : stageFront->mesh->vertices){
		v.u *= 10;
		v.v *= 100;
	}
	stageFront->mesh->dirty = true;


	if(std::rand() % 2 == 0){
		Sprite * arenaBg = new Sprite();
		float scale = sceneWidth*0.8f;
		arenaBg->transform->translate(25, 35, -5);
		arenaBg->transform->scale(scale, scale, 1);
		arenaBg->mesh->pushTexture2D(FightYourFriendsResourceManager::arena1);
		arenaBg->setShader(shader, true);
		addChild(arenaBg, 0);
	}else{
		Sprite * arenaBg = new Sprite();
		float scale = sceneWidth*0.8f;
		arenaBg->transform->translate(/*1024/2 * scale * 0.001f*/25, 35, -5);
		arenaBg->transform->scale(scale, scale, 1);
		arenaBg->mesh->pushTexture2D(FightYourFriendsResourceManager::arena2Bg);
		arenaBg->setShader(shader, true);
		addChild(arenaBg, 0);

		Box2DSprite * arenaFg = new Box2DSprite(world, FightYourFriendsResourceManager::arena2Fg, b2_staticBody, false, nullptr, new Transform(), 0.03f);
		arenaFg->setTranslationPhysical(arenaFg->getCorrectedWidth()/2.f + 12.f, sceneHeight*0.25f, 0);
		
	b2Filter sf;
    sf.categoryBits = PuppetGame::kGROUND;
    sf.groupIndex = 0;
    sf.maskBits = PuppetGame::kITEM | PuppetGame::kPLAYER;
		arenaFg->createFixture(sf);
		arenaFg->setShader(shader, true);
		addChild(arenaFg, 1);

		for(unsigned long int i = 0; i < arenaFg->mesh->vertices.size(); ++i){
			arenaFg->mesh->vertices.at(i).y += 2.5f;
		}
		arenaFg->mesh->dirty = true;
	}
}