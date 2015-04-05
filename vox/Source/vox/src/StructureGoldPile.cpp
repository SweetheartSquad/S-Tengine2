#pragma once

#include <StructureGoldPile.h>
#include <PuppetGame.h>
#include <RapunzelResourceManager.h>
#include <MeshInterface.h>
#include <SpriteSheetAnimation.h>

StructureGoldPile::StructureGoldPile(Box2DWorld* _world):
	Structure(_world, PuppetGame::kSTRUCTURE, PuppetGame::kPLAYER, -25),
	NodeChild(nullptr),
	NodeTransformable(new Transform()),
	totalGold(6),
	goldTaken(0)
{
	componentScale = 0.0025f;
	
	TextureSampler * baseTex = RapunzelResourceManager::goldBrick;
	//841x340
	Texture * baseSpriteSheetTex = RapunzelResourceManager::goldSpriteSheet;

	rootComponent = new Box2DSprite(_world, baseTex, b2_staticBody, false, nullptr, new Transform(), componentScale);
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
	rootComponent->mesh->popTexture2D();
	SpriteSheetAnimation * spriteSheet = new SpriteSheetAnimation(baseSpriteSheetTex, 1);

	// sprite sheet animation
	int f[] = {0,1,2,3,4,5};
	std::vector<int> ff(std::begin(f), std::end(f));

	spriteSheet->pushFramesInRange(0, 5, 512, 209, baseSpriteSheetTex->width);
	
	rootComponent->addAnimation("brickStates", spriteSheet, true);

	//rootComponent->setTranslationPhysical(0.f, rootComponent->getCorrectedHeight(), 0.f);
	rootComponent->playAnimation = true;
	rootComponent->currentAnimation->currentFrame = 0;
	rootComponent->update(&vox::step);
	rootComponent->playAnimation = false;
}

StructureGoldPile::~StructureGoldPile(){
}

void StructureGoldPile::loseGold(){
	goldTaken++;
	float percentageRemaining = goldTaken/totalGold;
	for(unsigned long int i = 6; i > 0; --i){
		if(percentageRemaining < i/6){
			rootComponent->playAnimation = true;
			rootComponent->currentAnimation->currentFrame = i - 1;
			rootComponent->update(&vox::step);
			rootComponent->playAnimation = false;
			break;
		}
	}
}