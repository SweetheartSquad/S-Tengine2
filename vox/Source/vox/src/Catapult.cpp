#pragma once

#include "Catapult.h"
#include <Box2DSprite.h>
#include <Texture.h>
#include <GameJamCharacter.h>
#include "Box2DWorld.h"

Catapult::Catapult(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits):
	Structure(_world, _categoryBits, _maskBits),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	targetRoll(0)
{
	componentScale = 0.008f;

	TextureSampler baseTex = TextureSampler(new Texture("../assets/structure components/catapult/CatapultBase.png", 512, 512, true, true), 418, 264);
	TextureSampler armTex = TextureSampler(new Texture("../assets/structure components/catapult/CatapultFlinger.png", 512, 512, true, true), 429, 76);

	base = new Box2DSprite(_world, b2_staticBody, false, nullptr, new Transform(), baseTex.width, baseTex.height, baseTex.texture, componentScale);
	arm = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), armTex.width, armTex.height, armTex.texture, componentScale);

	components.push_back(&arm);
	components.push_back(&base);
	
	/*
	arm->createFixture(groupIndex);
	arm->setTranslationPhysical(0.f, 0.f, 0.2f);
	base->createFixture(groupIndex);
	base->setTranslationPhysical(0.f, 0.f, 10.f);
	*/
	for(Box2DSprite ** c : components){
		(*c)->createFixture(groupIndex);
	}
	base->setTranslationPhysical(0.f, base->getCorrectedHeight(), 0.f);
	
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
	jth.referenceAngle = 0;
	jth.lowerAngle = -glm::radians(45.f);
	jth.upperAngle = glm::radians(45.f);
	world->b2world->CreateJoint(&jth);

	/*b2PolygonShape tShape;
	tShape.SetAsBox(head->width*std::abs(transform->scaleVector.x)*head->scale*4.f, std::abs(head->height*transform->scaleVector.y)*head->scale*5.f);
	
	b2Fixture * s = head->body->CreateFixture(&tShape, 1);
	s->SetSensor(true);
	
	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}
	s->SetFilterData(sf);*/
}

Catapult::~Catapult(){
}

void Catapult::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Structure::render(_matrixStack, _renderStack);
}

void Catapult::update(Step* _step){
	Structure::update(_step);
	//neck
	/*b2RevoluteJoint * neck = ((b2RevoluteJoint *)head->body->GetJointList()->joint);
	float angle = neck->GetJointAngle();

	neck->SetMotorSpeed(-angle*360);
	neck->SetMaxMotorTorque(head->body->GetMass()*750*(std::abs(angle)*5));*/

	//body
	float bodAngle = (*components.at(0))->body->GetAngle() - targetRoll;
	(*components.at(0))->body->SetAngularVelocity(-bodAngle*10);
	/*if((*components.at(0))->body->GetPosition().y < 5){
		(*components.at(0))->applyLinearImpulseUp(250);
	}*/
}

void Catapult::unload(){
	Structure::unload();
}

void Catapult::load(){
	Structure::load();
}