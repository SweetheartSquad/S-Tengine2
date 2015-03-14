#pragma once

#include <Hair.h>

#include <Box2DSprite.h>
#include <Box2DWorld.h>
#include <RapunzelResourceManager .h>

Hair::Hair(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	Structure(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	componentScale = 0.015f;
	
	TextureSampler * hairTex = RapunzelResourceManager::hair;

	rootComponent = new Box2DSprite(_world, b2_staticBody, false, nullptr, new Transform(), hairTex->width, hairTex->height, hairTex->texture, componentScale);
	components.push_back(&rootComponent);
	
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

	rootComponent->setTranslationPhysical(0.f, rootComponent->getCorrectedHeight(), 0.f);
}

Hair::~Hair()
{
}