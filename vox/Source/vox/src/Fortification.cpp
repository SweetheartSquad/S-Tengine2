#pragma once

#include "Fortification.h"
#include <Box2DSprite.h>
#include "MeshInterface.h"
#include <Texture.h>
#include "Box2DWorld.h"
#include "Boulder.h"
#include "PuppetScene.h"
#include "SpriteSheet.h"
#include "SpriteSheetAnimation.h"
#include <iostream>

#include <SlayTheDragonResourceManager.h>

Fortification::Fortification(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	StructureBreakable(25.f, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{

	/*
	//fortBackground = new Sprite(nullptr, new Transform());
	fortBackground->mesh->pushTexture2D(SlayTheDragonResourceManager::fortBackground->texture);
	fortBackground->setShader(shader, true);
	fortBackground->transform->translate(glm::vec3(0, 30, 0));
	*/

	componentScale = 0.03f;

	TextureSampler * roofTex = SlayTheDragonResourceManager::fortStructure;

	rootComponent = new Box2DSprite(_world, roofTex, b2_staticBody, false, nullptr, new Transform(), componentScale);

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
	
	/*// TODO
	roof->mesh->textures.pop_back();
	SpriteSheetAnimation * spriteSheet = new SpriteSheetAnimation(baseSpriteSheetTex, 0);

	// sprite sheet animation
	int f[] = {0,1,2,3};
	std::vector<int> ff(std::begin(f), std::end(f));

	spriteSheet->pushFramesInRange(0, 3, baseTex->width, baseTex->height, baseSpriteSheetTex->width);
	
	roof->addAnimation("castleStates", spriteSheet, true);
	*/

}

void Fortification::takeDamage(float _damage){
	StructureBreakable::takeDamage(_damage);
	glm::vec3 sv = rootComponent->transform->getScaleVector();
	switch (state){
	default:
	case StructureBreakable::kNORMAL:
		sv.x = 5;
		sv.y = 1;
		break;
	case StructureBreakable::kDAMAGED:
        sv.x = 1;
        sv.y = 1;
		break;
	case StructureBreakable::kDEAD:
        sv.x = 1;
        sv.y = 5;
		break;
	}
	rootComponent->transform->scale(sv, false);
}