#pragma once

#include <Lever.h>

#include <Box2DSprite.h>
#include <Box2DWorld.h>
#include <RapunzelResourceManager .h>

Lever::Lever(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	Structure(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	componentScale = 0.008f;

	TextureSampler * baseTextureSampler = RapunzelResourceManager::leverBase;
	TextureSampler * handleTextureSampler = RapunzelResourceManager::leverHandle;

	rootComponent = base = new Box2DSprite(_world, b2_staticBody, false, nullptr, new Transform(), baseTextureSampler->width, baseTextureSampler->height, baseTextureSampler->texture, componentScale);
	handle = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), handleTextureSampler->width, handleTextureSampler->height, handleTextureSampler->texture, componentScale);
	
	components.push_back(&base);
	components.push_back(&handle);
	
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
	
	handle->body->GetFixtureList()->SetDensity(10.f);

	// axel
	b2RevoluteJointDef jth;
	jth.bodyA = base->body;
	jth.bodyB = handle->body;
	jth.localAnchorA.Set(0.f, 0.9f * base->getCorrectedHeight());
	jth.localAnchorB.Set(0.f, 0.1f * handle->getCorrectedHeight());
	jth.collideConnected = false;
	jth.enableLimit = true;
	jth.referenceAngle = 0.f;
	jth.lowerAngle = glm::radians(-80.f);
	//jth.upperAngle = 0.f;
	world->b2world->CreateJoint(&jth);
}

Lever::~Lever()
{
}