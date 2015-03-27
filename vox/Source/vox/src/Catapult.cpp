#pragma once

#include "Catapult.h"
#include <Box2DSprite.h>
#include <Texture.h>
#include "Box2DWorld.h"
#include "Boulder.h"
#include "PuppetScene.h"
#include <PuppetCharacter.h>
#include <iostream>

#include <RaidTheCastleResourceManager.h>

#define COOLDOWN 10


Catapult::Catapult(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	Structure(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	ready(true),
	firing(false),
	fireBoulder(false),
	boulderLoaded(false),
	boulderJoint(nullptr),
	playerWhoFired(nullptr),
	cooldownCnt(0.f),
	boulder(nullptr),
	arm(nullptr),
	base(nullptr)
{
	componentScale = 0.016f;

	TextureSampler * baseTex = RaidTheCastleResourceManager::catapultBody;
	TextureSampler * armTex = RaidTheCastleResourceManager::catapultArm;

	base = new Box2DSprite(_world, baseTex, b2_staticBody, false, nullptr, new Transform(), componentScale);
	arm = new Box2DSprite(_world, armTex, b2_dynamicBody, false, nullptr, new Transform(), componentScale);
	
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

Catapult::~Catapult(){
}

void Catapult::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Structure::render(_matrixStack, _renderStack);
}

void Catapult::update(Step * _step){
	Structure::update(_step);

	b2RevoluteJoint * jk = (b2RevoluteJoint *)base->body->GetJointList()->joint;
	float angle = jk->GetJointAngle();
	
	if(!ready){
		if(firing){
			arm->body->SetAngularVelocity(-20);
			if(angle <= glm::radians(-45.f)){
				fireBoulder = true;
			}
			if(angle <= glm::radians(-75.f)){
				arm->body->SetAngularVelocity(1);
				firing = false;
			}
		}else if(angle >= -0.0001f){
			arm->body->SetAngularVelocity(1);
			ready = true;
		}
	}else{
		arm->body->SetAngularVelocity(1);
	}
}

void Catapult::unload(){
	Structure::unload();
}

void Catapult::load(){
	Structure::load();
}

void Catapult::fireCatapult(PuppetCharacter * _playerWhoFired){
	b2RevoluteJoint * j = (b2RevoluteJoint *)base->body->GetJointList()->joint;
	//arm->body->SetAngularVelocity(-20);
	firing = true;
	ready = false;
	playerWhoFired = _playerWhoFired;
}