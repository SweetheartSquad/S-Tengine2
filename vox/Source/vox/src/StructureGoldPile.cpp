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
	totalGold(60),
	goldTaken(0)
{
	componentScale = 0.0025f;
	
	TextureSampler * baseTex = RapunzelResourceManager::goldBrick;
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
	SpriteSheetAnimation * spriteSheet = new SpriteSheetAnimation(baseSpriteSheetTex, 0);

	// sprite sheet animation
	int f[] = {0,1,2,3,4,5};
	std::vector<int> ff(std::begin(f), std::end(f));

	spriteSheet->pushFramesInRange(0, 5, 512, 209, baseSpriteSheetTex->width);
	
	rootComponent->addAnimation("brickStates", spriteSheet, true);
	rootComponent->playAnimation = true;
	rootComponent->currentAnimation->currentFrame = 0;
	rootComponent->update(&vox::step);
	rootComponent->playAnimation = false;
}

StructureGoldPile::~StructureGoldPile(){
	
}

void StructureGoldPile::loseGold(){
	goldTaken++;
	int goldPerState = totalGold / 6;
	int frame = goldTaken/goldPerState;
	if(frame <= 5) {
		rootComponent->playAnimation = true;
		rootComponent->currentAnimation->currentFrame = frame;
		rootComponent->update(&vox::step);
		rootComponent->playAnimation = false; 
	}
}