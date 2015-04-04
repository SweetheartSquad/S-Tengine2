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
	
	glove = new Box2DSprite(world, RapunzelResourceManager::glove, b2_dynamicBody, false, nullptr, new Transform(), componentScale);
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
	b2RevoluteJointDef jth;
	jth.bodyA = spring->body;
	jth.bodyB = glove->body;
	jth.localAnchorA.Set(0.9f * spring->getCorrectedWidth(), 0.f * spring->getCorrectedHeight());
	jth.localAnchorB.Set(-0.9f * glove->getCorrectedWidth(), 0.f * glove->getCorrectedHeight());
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

void StructureBoxingGlove::punch(){
	std::cout << "punch!" << std::endl;
}