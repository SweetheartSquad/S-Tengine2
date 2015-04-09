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
#include <SoundManager.h>

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
	
	// sprite sheet animation
	rootComponent->mesh->popTexture2D();
	
	SpriteSheetAnimation * spriteSheet = new SpriteSheetAnimation(SlayTheDragonResourceManager::fortificationSpriteSheet, 0);

	spriteSheet->pushFramesInRange(0, 3, 1024, 187, 1024);
	rootComponent->addAnimation("fortStates", spriteSheet, true);
	rootComponent->mesh->uvEdgeMode = GL_MIRRORED_REPEAT_ARB;

}

void Fortification::takeDamage(float _damage){
	StructureBreakable::takeDamage(_damage);
	glm::vec3 sv = rootComponent->transform->getScaleVector();
	unsigned long int lastFrame = rootComponent->currentAnimation->currentFrame;
	switch (state){
	default:
	case StructureBreakable::kNORMAL:
		rootComponent->currentAnimation->currentFrame = 0;
		break;
	case StructureBreakable::kDAMAGED:
		if(lastFrame != 2){
			rootComponent->currentAnimation->currentFrame = 2;
			SlayTheDragonResourceManager::miscSounds->play("damaged");
		}
		break;
	case StructureBreakable::kDEAD:
		if(lastFrame != 3){
			rootComponent->currentAnimation->currentFrame = 3;
			SlayTheDragonResourceManager::miscSounds->play("broken");
			b2Filter sf;
			sf.groupIndex = groupIndex;
			sf.categoryBits = PuppetGame::kSTRUCTURE;
			sf.maskBits = 0;
			for(Box2DSprite ** c : components){
				(*c)->body->GetFixtureList()->SetFilterData(sf);
			}
		}
		break;
	}

	rootComponent->transform->scale(sv, false);
}