#pragma once

#include "Catapult.h"
#include <Box2DSprite.h>
#include <Texture.h>
#include "Box2DWorld.h"
#include "Boulder.h"
#include "PuppetScene.h"
#include <PuppetCharacter.h>
#include <iostream>

#include <RaidTheCastle.h>
#include <RaidTheCastleResourceManager.h>
#include <SoundManager.h>
#include <FollowCamera.h>

#define COOLDOWN 10


Catapult::Catapult(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	StructureInteractable(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	fireBoulder(false),
	boulderJoint(nullptr),
	cooldownCnt(0.f),
	boulder(nullptr),
	arm(nullptr),
	base(nullptr)
{
	componentScale = 0.016f;

	base = new Box2DSprite(_world, RaidTheCastleResourceManager::catapultBody, b2_staticBody, false, nullptr, new Transform(), componentScale);
	arm = new Box2DSprite(_world, RaidTheCastleResourceManager::catapultArm, b2_dynamicBody, false, nullptr, new Transform(), componentScale);
	
	components.push_back(&arm);
	components.push_back(&base);
	rootComponent = base;
	
	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}
	sf.groupIndex = groupIndex;

	for(Box2DSprite ** c : components){
		(*c)->createFixture(sf);
	}

	setUserData(this);

	base->setTranslationPhysical(0.f, base->getCorrectedHeight(), 0.f);
	
	/*b2PolygonShape tShape;
	tShape.SetAsBox(base->width*std::abs(transform->scaleVector.x)*base->scale*2.f, std::abs(base->height*transform->scaleVector.y)*base->scale*2.f);
	
	b2Fixture * sensor = base->body->CreateFixture(&tShape, 1);
	sensor->SetSensor(true);
	sensor->SetUserData(this);

	sensor->SetFilterData(sf);*/
	
	arm->body->GetFixtureList()->SetDensity(10.f);

	// axel
	b2RevoluteJointDef jth;
	jth.bodyA = base->body;
	jth.bodyB = arm->body;
	jth.localAnchorA.Set(0.f * base->getCorrectedWidth(), 0.7f * base->getCorrectedHeight());
	jth.localAnchorB.Set(0.9f * arm->getCorrectedWidth(), 0.f * arm->getCorrectedHeight());
	jth.collideConnected = false;
	jth.enableLimit = true;
	/*jth.enableMotor = true;
	jth.maxMotorTorque = 0.f;
	jth.motorSpeed = 0.f;*/
	jth.referenceAngle = 0.f;
	jth.lowerAngle = glm::radians(-80.f);
	//jth.upperAngle = 0.f;
	world->b2world->CreateJoint(&jth);
}

void Catapult::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Structure::render(_matrixStack, _renderStack);
}

void Catapult::update(Step * _step){
	StructureInteractable::update(_step);

	b2RevoluteJoint * jk = (b2RevoluteJoint *)base->body->GetJointList()->joint;
	float angle = jk->GetJointAngle();
	
	if(!ready){
		if(triggering){
			arm->body->SetAngularVelocity(-20);
			if(angle <= glm::radians(-45.f)){
				fireBoulder = true;
			}
			if(angle <= glm::radians(-75.f)){
				arm->body->SetAngularVelocity(1);
				triggering = false;
			}
		}else if(angle >= -0.0001f){
			arm->body->SetAngularVelocity(1);
			ready = true;
		}
	}else{
		arm->body->SetAngularVelocity(1);
	}

	
	RaidTheCastle * rtc = static_cast<RaidTheCastle *>(scene);
	if(fireBoulder){
		RaidTheCastleResourceManager::catapultSounds->playRandomSound();
		fireBoulder = false;
		if(boulderJoint != nullptr){
			world->b2world->DestroyJoint(boulderJoint);
			boulderJoint = nullptr;
			boulder->catapult = nullptr;
			((FollowCamera *)rtc->gameCam)->addTarget(boulder->rootComponent);
			boulder->playerWhoFired = playerWhoTriggered;
			boulder = nullptr;
			playerWhoTriggered = nullptr;
		}
	}

}

void Catapult::unload(){
	Structure::unload();
}

void Catapult::load(){
	Structure::load();
}

void Catapult::trigger(PuppetCharacter * _playerWhoTriggered){
	StructureInteractable::trigger(_playerWhoTriggered);
	b2RevoluteJoint * j = (b2RevoluteJoint *)base->body->GetJointList()->joint;
}

void Catapult::prepare(){
	RaidTheCastle * rtc = static_cast<RaidTheCastle *>(scene);

	boulder = new Boulder(world, PuppetGame::kITEM, PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kGROUND, groupIndex);
	boulder->setShader((Shader *)rtc->shader, true);
	rtc->addChild(boulder, 1);
	boulder->addToLayeredScene(rtc, 1);
	rtc->items.push_back(boulder);

	b2Vec2 armPos = b2Vec2(-arm->getCorrectedWidth() * 0.8f, base->getCorrectedHeight() * 0.9f);
	// snap to capatult base
	boulder->snapComponents(base);
	// translate to arm position from base
	boulder->translateComponents(glm::vec3(armPos.x*2, armPos.y, 0.f));
	//boulder->translateComponents(glm::vec3(-21, 6, 0)); // this is hard-coded, should not be
	cooldownCnt = 0.f;

	// axle
	b2WeldJointDef abpj;
	abpj.bodyA = arm->body;
	abpj.bodyB = boulder->boulder->body;
	abpj.localAnchorA.Set(-0.7f * arm->getCorrectedWidth(), 0.0f * arm->getCorrectedHeight());
	abpj.localAnchorB.Set(0.0f * boulder->boulder->getCorrectedWidth(), 0.f * boulder->boulder->getCorrectedHeight());
	abpj.collideConnected = false;
	abpj.referenceAngle = 0.f;
	boulderJoint = (b2WeldJoint *)world->b2world->CreateJoint(&abpj);
	boulder->catapult = this;

	StructureInteractable::prepare();
}

void Catapult::interact(){
	StructureInteractable::interact();
}