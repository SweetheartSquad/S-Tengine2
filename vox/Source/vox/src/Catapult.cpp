#pragma once

#include "Catapult.h"
#include <Box2DSprite.h>
#include <Texture.h>
#include "Box2DWorld.h"
#include "Boulder.h"
#include "PuppetScene.h"
#include <iostream>

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
	cooldownCnt(0.f)
{
	componentScale = 0.008f;

	TextureSampler baseTex = TextureSampler(new Texture("../assets/structure components/catapult/CatapultBase.png", 512, 512, true, true), 418, 264);
	TextureSampler armTex = TextureSampler(new Texture("../assets/structure components/catapult/CatapultFlinger.png", 512, 512, true, true), 429, 76);

	base = new Box2DSprite(_world, b2_staticBody, false, nullptr, new Transform(), baseTex.width, baseTex.height, baseTex.texture, componentScale);
	arm = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), armTex.width, armTex.height, armTex.texture, componentScale);
	
	components.push_back(&arm);
	components.push_back(&base);
	
	for(Box2DSprite ** c : components){
		(*c)->createFixture(groupIndex);
	}

	base->setTranslationPhysical(0.f, base->getCorrectedHeight(), 0.f);
	
	b2PolygonShape tShape;
	tShape.SetAsBox(base->width*std::abs(transform->scaleVector.x)*base->scale*2.f, std::abs(base->height*transform->scaleVector.y)*base->scale*2.f);
	
	b2Fixture * sensor = base->body->CreateFixture(&tShape, 1);
	sensor->SetSensor(true);
	sensor->SetUserData(this);
	
	arm->body->GetFixtureList()->SetDensity(10.f);

	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}
	sensor->SetFilterData(sf);

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
	jth.lowerAngle = -glm::radians(80.f);
	//jth.upperAngle = 0.f;
	world->b2world->CreateJoint(&jth);
}

Catapult::~Catapult(){
}

void Catapult::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Structure::render(_matrixStack, _renderStack);
}

void Catapult::update(Step* _step){
	Structure::update(_step);

	b2RevoluteJoint * jk = (b2RevoluteJoint *)base->body->GetJointList()->joint;
	float angle = jk->GetJointAngle();
	//std::cout << glm::degrees(angle) << std::endl;
	//std::cout << ready << std::endl;
	//std::cout << firing << std::endl;
	
	if(!ready){
		if(firing){
			if(angle <= glm::radians(-70.f)){
				fireBoulder = true;
			}
			if(angle <= glm::radians(-80.f)){
				arm->body->SetAngularVelocity(0);
				firing = false;
			}
		}else if(angle >= -0.00001f){
			ready = true;
		}
	}
}

void Catapult::unload(){
	Structure::unload();
}

void Catapult::load(){
	Structure::load();
}

void Catapult::fireCatapult(){
	b2RevoluteJoint * j = (b2RevoluteJoint *)base->body->GetJointList()->joint;
	arm->body->SetAngularVelocity(-50);
	firing = true;
	ready = false;
}