#pragma once

#include <ItemFlail.h>
#include <Box2DWorld.h>

#include <RaidTheCastleResourceManager.h>
#include <TextureSampler.h>

ItemFlail::ItemFlail(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex, int _damage, float _handleX, float _handleY):
	Item(false, _world, _categoryBits, _maskBits, _groupIndex, _damage, _handleX, _handleY),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	TextureSampler * weapon = RaidTheCastleResourceManager::itemFlailGrip;
	TextureSampler * weapon2 = RaidTheCastleResourceManager::itemFlailJoint;
	TextureSampler * weapon3 = RaidTheCastleResourceManager::itemFlailHead;

	rootComponent = handle = new Box2DSprite(_world, weapon, b2_dynamicBody, false, nullptr, new Transform(), componentScale);
	joint = new Box2DSprite(_world, weapon2, b2_dynamicBody, false, nullptr, new Transform(), componentScale);
	head = new Box2DSprite(_world, weapon3, b2_dynamicBody, false, nullptr, new Transform(), componentScale);
	components.push_back(&rootComponent);
	components.push_back(&joint);
	components.push_back(&head);
	
	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}else{
		sf.maskBits = 0;
	}
	sf.groupIndex = groupIndex;
	
	for(Box2DSprite ** c : components){
		(*c)->createFixture(sf);
		(*c)->body->GetFixtureList()->SetDensity(0.01f);
		(*c)->body->ResetMassData();
	}

	setUserData(this);
	//itemToPickup->rootComponent

	b2RevoluteJointDef jointDef;
	jointDef.bodyA = rootComponent->body;
	jointDef.bodyB = joint->body;
	jointDef.localAnchorA.Set(0.f, 0.75f * joint->getCorrectedHeight());
	jointDef.localAnchorB.Set(0.f, -0.75f);
	jointDef.collideConnected = false;
	jointDef.enableLimit = false;
	jointDef.referenceAngle = glm::radians(0.f);
	//jointDef.upperAngle = glm::radians(20.f);
	//jointDef.lowerAngle = glm::radians(-20.f);
	_world->b2world->CreateJoint(&jointDef);

	jointDef.bodyA = joint->body;
	jointDef.bodyB = head->body;
	jointDef.localAnchorA.Set(0.f, 0.5f * joint->getCorrectedHeight());
	jointDef.localAnchorB.Set(0.f, -0.5f);
	jointDef.collideConnected = false;
	jointDef.enableLimit = false;
	jointDef.referenceAngle = glm::radians(0.f);
	//jointDef.upperAngle = glm::radians(20.f);
	//jointDef.lowerAngle = glm::radians(-20.f);
	_world->b2world->CreateJoint(&jointDef);
}

ItemFlail::~ItemFlail(){
}
