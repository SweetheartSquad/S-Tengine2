#pragma once

#include "RaidTheCastle.h"
#include <RaidTheCastleContactListener.h>
#include "Castle.h"
#include "PuppetGame.h"
#include "PuppetCharacter.h"
#include "FollowCamera.h"
#include "Behaviour.h"
#include "BehaviourFollow.h"
#include <BehaviourPatrol.h>
#include "Boulder.h"
#include "Catapult.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "MeshEntity.h"
#include "MeshInterface.h"
#include "MeshFactory.h"
#include "shader/BaseComponentShader.h"
#include "keyboard.h"
#include <Texture.h>
#include <PuppetCharacterCastleChampion.h>
#include <PuppetCharacterKnight.h>
#include <PuppetTexturePack.h>
#include <PuppetController.h>

#include <glfw\glfw3.h>


RaidTheCastle::RaidTheCastle(PuppetGame* _game):
	PuppetScene(_game, 10),
	castle(new Castle(world, PuppetGame::kSTRUCTURE, PuppetGame::kITEM, 30)),
	catapult(new Catapult(world, PuppetGame::kSTRUCTURE, PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kBOUNDARY | PuppetGame::kPLAYER, -10)),
	champion(new PuppetCharacterCastleChampion(world, PuppetGame::kPLAYER, -1, -20)),
	playerCharacter(new PuppetCharacterKnight(0, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR, -1)),
	playerCharacter2(new PuppetCharacterKnight(1, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR, -2)),
	playerCharacter3(new PuppetCharacterKnight(2, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR, -3)),
	playerCharacter4(new PuppetCharacterKnight(3, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR, -4))
{
	cl = new RaidTheCastleContactListener(this);

	playerCharacter->setShader(shader, true);
	addChild(playerCharacter, 1);
	playerCharacter->addToLayeredScene(this, 1);
	playerCharacter->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter->translateComponents(glm::vec3(0.0f, 15.f, 0.f));
	puppetController->puppetCharacter = playerCharacter;

	playerCharacter2->setShader(shader, true);
	addChild(playerCharacter2, 1);
	playerCharacter2->addToLayeredScene(this, 1);
	playerCharacter2->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter2->translateComponents(glm::vec3(10.0f, 15.f, 0.f));
	puppetController2->puppetCharacter = playerCharacter2;

	playerCharacter3->setShader(shader, true);
	addChild(playerCharacter3, 1);
	playerCharacter3->addToLayeredScene(this, 1);
	playerCharacter3->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter3->translateComponents(glm::vec3(20.0f, 15.f, 0.f));
	puppetController3->puppetCharacter = playerCharacter3;

	playerCharacter4->setShader(shader, true);
	addChild(playerCharacter4, 1);
	playerCharacter4->addToLayeredScene(this, 1);
	playerCharacter4->head->maxVelocity = b2Vec2(10, 10);
	playerCharacter4->translateComponents(glm::vec3(30.0f, 15.f, 0.f));
	puppetController4->puppetCharacter = playerCharacter4;

	gameCam->addTarget(playerCharacter->torso);
	gameCam->addTarget(playerCharacter2->torso);
	gameCam->addTarget(playerCharacter3->torso);
	gameCam->addTarget(playerCharacter4->torso);

	castle->setShader(shader, true);
	castle->addToLayeredScene(this, 0);
	addChild(castle, 0);

	castle->translateComponents(glm::vec3(150, 0, 0));

	catapult->setShader(shader, true);
	catapult->addToLayeredScene(this, 1);
	addChild(catapult, 1);

	catapult->translateComponents(glm::vec3(-10,0,0));

	loadCatapult();

	champion->setShader(shader, true);
	addChild(champion, 0);
	champion->addToLayeredScene(this, 1);
	champion->head->maxVelocity = b2Vec2(10, 10);
	champion->translateComponents(glm::vec3(100,15,0));

	//playerCharacter4->behaviourManager.addBehaviour(new BehaviourFollow(playerCharacter4, 10, PuppetGame::kPLAYER));
	//playerCharacter4->behaviourManager.addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), playerCharacter4, 10));
	//playerCharacter4->ai = true;

	gameCam->addTarget(castle->rootComponent);
	gameCam->addTarget(catapult->rootComponent);
}

RaidTheCastle::~RaidTheCastle(){
}

void RaidTheCastle::update(Step* _step){
	PuppetScene::update(_step);
	if(catapult->ready && !catapult->boulderLoaded){
		loadCatapult();
	}
	if(keyboard->keyDown(GLFW_KEY_B)){
		//playerCharacter->behaviourManager.behaviours.at(0)->targets.clear();
		//playerCharacter->behaviourManager.behaviours.at(0)->active = false;
	}

	/*if(keyboard->keyDown(GLFW_KEY_F)){
		catapult->fireCatapult();
	}*/
	if(catapult->fireBoulder){
		catapult->fireBoulder = false;
		catapult->boulderLoaded = false;
		if(catapult->boulderJoint != nullptr){
			world->b2world->DestroyJoint(catapult->boulderJoint);
			catapult->boulderJoint = nullptr;
			

			// set the item's group index to zero so that it can collide normally
			/*for(Box2DSprite ** bs : catapult->boulder->components){
				b2Filter b1 = (*bs)->body->GetFixtureList()->GetFilterData();
				b1.groupIndex = 0;
				(*bs)->body->GetFixtureList()->SetFilterData(b1);
				(*bs)->body->GetFixtureList()->Refilter();
			}*/
			
			((FollowCamera *)gameCam)->addTarget(catapult->boulder->boulder);
			catapult->boulder = nullptr;
		}
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
	
	b2Vec2 boulderPos = b2Vec2(armPos.x + catapult->arm->getCorrectedWidth() * 2.f + jointPosB.x, armPos.y + jointPosB.y);
	//b2Vec2 boulderPos = b2Vec2(catapult->base->transform->translationVector.x - catapult->arm->getCorrectedWidth() * 2.0 * 0.8, catapult->base->getCorrectedHeight() * 2.0 * 0.9);
	//boulder->translateComponents(glm::vec3(boulderPos.x, boulderPos.y, 0));

	boulder->translateComponents(glm::vec3(-21, 6, 0)); // this is hard-coded, should not be
	//boulder->boulder->body->SetTransform(b2Vec2(-36, 6), 0);
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
}