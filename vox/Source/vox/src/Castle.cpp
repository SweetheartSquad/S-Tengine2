#pragma once

#include "Castle.h"
#include <Box2DSprite.h>
#include "MeshInterface.h"
#include <Texture.h>
#include "Box2DWorld.h"
#include "Boulder.h"
#include "PuppetScene.h"
#include "SpriteSheet.h"
#include "SpriteSheetAnimation.h"
#include <iostream>

#define COOLDOWN 10
#define MAX_HEALTH 100

Castle::Castle(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	Structure(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	health(MAX_HEALTH),
	damage(0)
{
	componentScale = 0.008f;
	
	TextureSampler baseTex = TextureSampler(new Texture("../assets/structure components/castle/CastleNorm_State1.png", 1024, 1024, true, true), 973, 619);
	Texture * baseSpriteSheetTex = new Texture("../assets/structure components/castle/Castle_SpriteSheet.png", 2048, 2048, true, true);

	base = new Box2DSprite(_world, b2_staticBody, false, nullptr, new Transform(), baseTex.width, baseTex.height, baseTex.texture, componentScale);
	components.push_back(&base);
	
	
	for(Box2DSprite ** c : components){
		(*c)->createFixture(groupIndex);
	}
	
	base->mesh->textures.pop_back();
	SpriteSheetAnimation * spriteSheet = new SpriteSheetAnimation(baseSpriteSheetTex, 1);

	//spriteSheet->secondsPerFrame = 10000;

	// sprite sheet animation
	int f[] = {0,1,2,3};
	std::vector<int> ff(std::begin(f), std::end(f));

	spriteSheet->pushFramesInRange(0, 3, 973, 619, 973 * 2);
	

	base->addAnimation("castleStates", spriteSheet, true);


	base->setTranslationPhysical(0.f, base->getCorrectedHeight(), 0.f);
	
	b2PolygonShape tShape;
	tShape.SetAsBox(base->width*std::abs(transform->scaleVector.x)*base->scale*2.f, std::abs(base->height*transform->scaleVector.y)*base->scale*2.f);
	
	b2Fixture * sensor = base->body->CreateFixture(&tShape, 1);
	sensor->SetSensor(true);
	sensor->SetUserData(this);

	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}
	sensor->SetFilterData(sf);

	
}

Castle::~Castle(){
}

void Castle::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Structure::render(_matrixStack, _renderStack);
}

void Castle::update(Step * _step){
	Structure::update(_step);

	if(damage > 0){
		health -= damage;
		damage = 0;

		if(state != kDAMAGED && health <= MAX_HEALTH * 0.5){
			state == kDAMAGED;
			
		}
		if(state != kDEAD && health <= 0){
			state == kDEAD;
			translateComponents(glm::vec3(0, 0, -10));
		}
	}
}

void Castle::unload(){
	Structure::unload();
}

void Castle::load(){
	Structure::load();
}