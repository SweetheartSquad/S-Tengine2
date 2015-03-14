#pragma once

#include <ItemFlail.h>
#include <Box2DWorld.h>

ItemFlail::ItemFlail(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex, int _damage, float _handleX, float _handleY):
	Item(false, _world, _categoryBits, _maskBits, _groupIndex, _damage, _handleX, _handleY),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	TextureSampler * weapon = RaidTheCastleResourceManager::itemFlailGrip;
	TextureSampler * weapon2 = RaidTheCastleResourceManager::itemFlailJoint;
	TextureSampler * weapon3 = RaidTheCastleResourceManager::itemFlailHead;
	
	componentScale = 0.008f;

	rootComponent = handle = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), weapon->width, weapon->height, weapon->texture, componentScale/4);
	joint = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), weapon2->width, weapon2->height, weapon2->texture, componentScale/4);
	head = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), weapon3->width, weapon3->height, weapon3->texture, componentScale/4);
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

	b2RevoluteJointDef rhrej;
	rhrej.bodyA = rootComponent->body;
	rhrej.bodyB = joint->body;
	rhrej.localAnchorA.Set(0.f, 0.9f * joint->getCorrectedHeight());
	rhrej.localAnchorB.Set(0.f, -0.9f);
	rhrej.collideConnected = false;
	rhrej.enableLimit = false;
	rhrej.referenceAngle = glm::radians(0.f);
	_world->b2world->CreateJoint(&rhrej);

	rhrej.bodyA = joint->body;
	rhrej.bodyB = head->body;
	rhrej.localAnchorA.Set(0.f, 0.9f * joint->getCorrectedHeight());
	rhrej.localAnchorB.Set(0.f, -0.9f);
	rhrej.collideConnected = false;
	rhrej.enableLimit = false;
	rhrej.referenceAngle = glm::radians(0.f);
	_world->b2world->CreateJoint(&rhrej);
}

ItemFlail::~ItemFlail(){
}
