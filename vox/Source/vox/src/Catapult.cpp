#pragma once

#include "Catapult.h"
#include <Box2DSprite.h>
#include <Texture.h>
#include "Box2DWorld.h"
#include "Boulder.h"
#include "PuppetScene.h"
#include <iostream>
using namespace std;
#define COOLDOWN 10


Catapult::Catapult(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits):
	Structure(_world, _categoryBits, _maskBits),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	ready(true),
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
	jth.enableMotor = true;
	jth.maxMotorTorque = 0;
	jth.motorSpeed = 0;
	jth.referenceAngle = glm::radians(0.f);
	jth.lowerAngle = glm::radians(-90.f);
	jth.upperAngle = glm::radians(0.f);
	world->b2world->CreateJoint(&jth);
}

Catapult::~Catapult(){
}

void Catapult::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Structure::render(_matrixStack, _renderStack);
}

void Catapult::update(Step* _step){
	b2RevoluteJoint * jk = (b2RevoluteJoint *)(*components.at(1))->body->GetJointList()->joint;
	float angle = glm::degrees(jk->GetJointAngle());
	cout << "angle: ";
	cout << angle << endl;
	cout << "motorSpeed: ";
	cout << jk->GetMotorSpeed() << endl;
	cout << "maxMotorTorque";
	cout << jk->GetMaxMotorTorque() << endl;
	Structure::update(_step);
	if(!ready){
		b2RevoluteJoint * j = (b2RevoluteJoint *)(*components.at(1))->body->GetJointList()->joint;
		
		if(j->GetMaxMotorTorque() > 20.f && angle > -180.f){
			j->SetMaxMotorTorque(20.f);
			j->SetMotorSpeed(18.f);
		}
		if(angle <= -180.f){
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
	b2RevoluteJoint * j = (b2RevoluteJoint *)(*components.at(1))->body->GetJointList()->joint;
	float angle = abs((j->GetJointAngle()*180));
	j->SetMotorSpeed(-180+angle);
	j->SetMaxMotorTorque((*components.at(0))->body->GetMass()*750*(angle*5));
	
	ready = false;
}