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

#define MAX_HEALTH 100

Fortification::Fortification(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	Structure(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	health(MAX_HEALTH),
	state(kNORMAL),
	damage(0.f)
{

	/*
	//fortBackground = new Sprite(nullptr, new Transform());
	fortBackground->mesh->pushTexture2D(SlayTheDragonResourceManager::fortBackground->texture);
	fortBackground->setShader(shader, true);
	fortBackground->transform->translate(glm::vec3(0, 30, 0));
	*/

	componentScale = 0.03f;
	
	TextureSampler * baseTex = SlayTheDragonResourceManager::fortForeground;
	TextureSampler * roofTex = SlayTheDragonResourceManager::fortStructure;

	rootComponent = base = new Box2DSprite(_world, baseTex, b2_staticBody, false, nullptr, new Transform(), componentScale);
	roof = new Box2DSprite(_world, roofTex, b2_staticBody, false, nullptr, new Transform(), componentScale);

	components.push_back(&rootComponent);
	components.push_back(&roof);
	
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

	roof->setTranslationPhysical(0.f, rootComponent->getCorrectedHeight() + roof->getCorrectedHeight() + 15.f, 0.f);
	translateComponents(glm::vec3(0, rootComponent->getCorrectedHeight(), 0));
}

Fortification::~Fortification(){
}

void Fortification::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Structure::render(_matrixStack, _renderStack);
}

void Fortification::update(Step * _step){
	Structure::update(_step);

	/*//TODO
	if(damage > 0){
		health -= damage;
		damage = 0;

		if(state != kDEAD && health <= 0){
			state = kDEAD;
			rootComponent->currentAnimation->currentFrame = 3;
			translateComponents(glm::vec3(0, -10, 0));
		}else if(state != kDAMAGED && state != kDEAD && health <= MAX_HEALTH * 0.5){
			state = kDAMAGED;
			rootComponent->currentAnimation->currentFrame = 1;
		}
		
	}
	*/
}

void Fortification::unload(){
	Structure::unload();
}

void Fortification::load(){
	Structure::load();
}