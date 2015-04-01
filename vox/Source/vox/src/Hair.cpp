#pragma once

#include <Hair.h>

#include <Box2DSprite.h>
#include <Box2DWorld.h>
#include <RapunzelResourceManager.h>

Hair::Hair(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	Structure(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	componentScale = 0.015f;
	
	TextureSampler * hairLinkTex = RapunzelResourceManager::hairLink;
	TextureSampler * hairEndTex = RapunzelResourceManager::hairEnd;

	rootComponent = new Box2DSprite(_world, hairLinkTex, b2_staticBody, false, nullptr, new Transform(), componentScale);

	components.push_back(&rootComponent);

	unsigned int numLinks = 5;

	for(unsigned int i = 0; i < numLinks; i++){
		TextureSampler * tex = (i < numLinks - 1 ? hairLinkTex : hairEndTex);
		Box2DSprite ** hairLink = new Box2DSprite*[1];
		hairLink[0] = new Box2DSprite(_world, tex, b2_dynamicBody, false, nullptr, new Transform(), componentScale);

		components.push_back(hairLink);
	}

	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}
	sf.groupIndex = groupIndex;
	
	for(Box2DSprite ** c : components){
		(*c)->createFixture(sf, b2Vec2(0.0f, 0.0f), nullptr, true);
		(*c)->body->GetFixtureList()->SetFriction(20.0f);
	}
	setUserData(this);

	for(unsigned int i = 0; i < numLinks; ++i){
		// axel
		b2RevoluteJointDef jt;
		jt.bodyA = (*components.at(i))->body;
		jt.bodyB = (*components.at(i + 1))->body;
		jt.localAnchorA.Set(0.f, -0.9f * rootComponent->getCorrectedHeight());
		jt.localAnchorB.Set(0.f, 0.9f * rootComponent->getCorrectedHeight());
		jt.collideConnected = false;
		jt.enableLimit = true;
		/*jth.enableMotor = true;
		jth.maxMotorTorque = 0.f;
		jth.motorSpeed = 0.f;*/
		jt.referenceAngle = 0.f;
		jt.lowerAngle = glm::radians(-20.f);
		jt.upperAngle = glm::radians(20.f);
		world->b2world->CreateJoint(&jt);
	}

	rootComponent->setTranslationPhysical(0.f, rootComponent->getCorrectedHeight() * (numLinks + 1), 0.f);
}

Hair::~Hair()
{
}