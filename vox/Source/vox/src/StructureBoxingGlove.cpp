#pragma once

#include <StructureBoxingGlove.h>
#include <PuppetGame.h>
#include <RapunzelResourceManager.h>
#include <Box2DWorld.h>

StructureBoxingGlove::StructureBoxingGlove(Box2DWorld * _world) :
	Structure(_world, PuppetGame::kSTRUCTURE, PuppetGame::kPLAYER, -25),
	NodeChild(nullptr),
	NodeTransformable(new Transform())
{
	
	componentScale = 0.016f;
	
	glove = new Box2DSprite(world, RapunzelResourceManager::glove, b2_dynamicBody, false, nullptr, new Transform(), componentScale/4.f);
	spring = new Box2DSprite(world, RapunzelResourceManager::spring, b2_staticBody, false, nullptr, new Transform(), componentScale);
	
	components.push_back(&glove);
	components.push_back(&spring);
	rootComponent = spring;
	
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
	
	/*b2PolygonShape tShape;
	tShape.SetAsBox(base->width*std::abs(transform->scaleVector.x)*base->scale*2.f, std::abs(base->height*transform->scaleVector.y)*base->scale*2.f);
	
	b2Fixture * sensor = base->body->CreateFixture(&tShape, 1);
	sensor->SetSensor(true);
	sensor->SetUserData(this);

	sensor->SetFilterData(sf);*/
	
	spring->body->GetFixtureList()->SetDensity(10.f);

	// axel
	b2PrismaticJointDef jth;
	jth.bodyA = spring->body;
	jth.bodyB = glove->body;
	jth.localAnchorA.Set(0.9f * spring->getCorrectedWidth(), 0.f * spring->getCorrectedHeight());
	jth.localAnchorB.Set(-0.9f * glove->getCorrectedWidth(), 0.f * glove->getCorrectedHeight());
	jth.collideConnected = false;
	jth.enableLimit = true;
	jth.enableMotor = true;
	jth.lowerTranslation = 0;
	jth.upperTranslation = 10;
	jth.localAxisA = b2Vec2(1, 0);
	jth.maxMotorForce = 1000.f;
	jth.motorSpeed = -100.f;
	jth.referenceAngle = 0.f;
	world->b2world->CreateJoint(&jth);
}

void StructureBoxingGlove::punch(){
	glove->applyLinearImpulseLeft(1000);
}