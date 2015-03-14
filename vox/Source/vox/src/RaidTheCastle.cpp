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

	playerCharacter2->setShader(shader, true);
	addChild(playerCharacter2, 1);
	playerCharacter2->addToLayeredScene(this, 1);
	playerCharacter2->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter2->translateComponents(glm::vec3(40.0f, 35, 0.f));
	puppetController2->puppetCharacter = playerCharacter2;

	playerCharacter3->setShader(shader, true);
	addChild(playerCharacter3, 1);
	playerCharacter3->addToLayeredScene(this, 1);
	playerCharacter3->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter3->translateComponents(glm::vec3(60.0f, 35, 0.f));
	puppetController3->puppetCharacter = playerCharacter3;

	playerCharacter4->setShader(shader, true);
	addChild(playerCharacter4, 1);
	playerCharacter4->addToLayeredScene(this, 1);
	playerCharacter4->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter4->translateComponents(glm::vec3(80.0f, 35, 0.f));
	puppetController4->puppetCharacter = playerCharacter4;

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
		/*if(playerCharacter->torso->body->GetPosition().y < 8){
			float t = playerCharacter->torso->body->GetAngle();
			playerCharacter->torso->applyLinearImpulseUp(50*(1-sin(t)));
			if(playerCharacter->torso->body->GetAngle() > 0){
				playerCharacter->torso->applyLinearImpulseLeft(150*(1-cos(t)));
			}else{
				playerCharacter->torso->applyLinearImpulseRight(150*(1-cos(t)));
			}
		}*/
	}if(keyboard->keyDown(GLFW_KEY_A)){
		playerCharacter->targetRoll = glm::radians(-45.f);
		/*playerCharacter->head->applyLinearImpulseLeft(25);
		if(playerCharacter->transform->scaleVector.x < 0){
			playerCharacter->transform->scaleX(-1);
		}*/
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		playerCharacter->targetRoll = glm::radians(45.f);
		/*playerCharacter->head->applyLinearImpulseRight(25);
		if(playerCharacter->transform->scaleVector.x > 0){
			playerCharacter->transform->scaleX(-1);
		}*/
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

	b2Vec2 jointPosB = ((b2RevoluteJoint *)catapult->arm->body->GetJointList()->joint)->GetLocalAnchorB();
	//boulder->boulder->body->SetTransform(catapult->arm->body->GetPosition(), std::rand());
	b2Vec2 armPos = catapult->arm->body->GetPosition();
	
	b2Vec2 cPos = catapult->base->body->GetPosition();
	b2Vec2 aSize = b2Vec2(catapult->arm->getCorrectedWidth(), catapult->arm->getCorrectedHeight());
	//b2Vec2 boulderPos = b2Vec2(armPos.x + catapult->arm->getCorrectedWidth() * 2.f + jointPosB.x, armPos.y + jointPosB.y);
	b2Vec2 boulderPos = b2Vec2(catapult->base->body->GetPosition().x - catapult->arm->getCorrectedWidth() * 0.8, catapult->base->body->GetPosition().y + catapult->base->getCorrectedHeight() * 0.9);
	//boulder->translateComponents(glm::vec3(boulderPos.x, boulderPos.y, 0));
	boulder->snapComponents(catapult->base);
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