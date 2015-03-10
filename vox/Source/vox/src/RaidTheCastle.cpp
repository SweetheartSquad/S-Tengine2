#pragma once

#include "RaidTheCastle.h"
#include "Castle.h"
#include "PuppetCharacter.h"
#include "Behaviour.h"
#include "Behaviours.h"
#include "Boulder.h"
#include "Catapult.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "shader/BaseComponentShader.h"
#include "keyboard.h"

#include <glfw\glfw3.h>

RaidTheCastle::RaidTheCastle(Game* _game):
	PuppetScene(_game, 0.5)
{
	castle = new Castle(world, kSTRUCTURE, kITEM, 30);
	castle->setShader(shader, true);
	castle->addToLayeredScene(this, 0);
	addChild(castle, 0);

<<<<<<< HEAD
	castle->translateComponents(glm::vec3(100, 0, 0));
=======
	castle->translateComponents(glm::vec3(50, 0, 0));
>>>>>>> 8ec075e434656f238248925f34476ae44a291cab

	catapult = new Catapult(world, kSTRUCTURE, kSTRUCTURE | kITEM | kBOUNDARY | kPLAYER, -10);
	catapult->setShader(shader, true);
	catapult->addToLayeredScene(this, 1);
	addChild(catapult, 1);

	catapult->translateComponents(glm::vec3(-10,0,0));

	loadCatapult();
}

RaidTheCastle::~RaidTheCastle(){
}

void RaidTheCastle::update(Step* _step){
	PuppetScene::update(_step);
	if(catapult->ready && !catapult->boulderLoaded){
		loadCatapult();
	}
	if(keyboard->keyDown(GLFW_KEY_B)){
		playerCharacter->behaviourManager.behaviours.at(0)->targets.clear();
		playerCharacter->behaviourManager.behaviours.at(0)->active = false;
	}

	if(keyboard->keyDown(GLFW_KEY_F)){
		catapult->fireCatapult();
	}
	if(catapult->fireBoulder){
		catapult->fireBoulder = false;
		catapult->boulderLoaded = false;
		if(catapult->boulderJoint != nullptr){
			world->b2world->DestroyJoint(catapult->boulderJoint);
			catapult->boulderJoint = nullptr;
			

			// set the item's group index to zero so that it can collide normally
			for(Box2DSprite ** bs : catapult->boulder->components){
				b2Filter b1 = (*bs)->body->GetFixtureList()->GetFilterData();
				b1.groupIndex = 0;
				(*bs)->body->GetFixtureList()->SetFilterData(b1);
				(*bs)->body->GetFixtureList()->Refilter();
			}

			catapult->boulder = nullptr;
		}
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
	Boulder * boulder = new Boulder(world, PuppetScene::kITEM, PuppetScene::kITEM | PuppetScene::kPLAYER | PuppetScene::kSTRUCTURE | PuppetScene::kGROUND, catapult->groupIndex);
	boulder->setShader(shader, true);
	addChild(boulder, 1);
	boulder->addToLayeredScene(this, 1);
	
	b2Vec2 jointPosB = ((b2RevoluteJoint *)catapult->arm->body->GetJointList()->joint)->GetLocalAnchorB();
	//boulder->boulder->body->SetTransform(catapult->arm->body->GetPosition(), std::rand());
	b2Vec2 armPos = catapult->arm->body->GetPosition();
	
	b2Vec2 boulderPos = b2Vec2(armPos.x + catapult->arm->getCorrectedWidth() * 2.0 + jointPosB.x, armPos.y + jointPosB.y);
	//b2Vec2 boulderPos = b2Vec2(catapult->base->transform->translationVector.x - catapult->arm->getCorrectedWidth() * 2.0 * 0.8, catapult->base->getCorrectedHeight() * 2.0 * 0.9);
	//boulder->translateComponents(glm::vec3(boulderPos.x, boulderPos.y, 0));

	boulder->translateComponents(glm::vec3(-21, 6, 0)); // this is hard-coded, should not be
	//boulder->boulder->body->SetTransform(b2Vec2(-36, 6), 0);
	catapult->cooldownCnt = 0.f;
	catapult->ready = true;
	catapult->boulderLoaded = true;
	catapult->boulder = boulder;

	// axel
	b2WeldJointDef abpj;
	abpj.bodyA = catapult->arm->body;
	abpj.bodyB = boulder->boulder->body;
	abpj.localAnchorA.Set(-0.7f * catapult->arm->getCorrectedWidth(), 0.0f * catapult->arm->getCorrectedHeight());
	abpj.localAnchorB.Set(0.0f * boulder->boulder->getCorrectedWidth(), 0.f * boulder->boulder->getCorrectedHeight());
	abpj.collideConnected = false;
	abpj.referenceAngle = 0.f;
	catapult->boulderJoint = (b2WeldJoint *)world->b2world->CreateJoint(&abpj);
}