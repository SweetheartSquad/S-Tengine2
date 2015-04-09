#pragma once

#include <StructureGoldPile.h>
#include <PuppetGame.h>
#include <RapunzelResourceManager.h>
#include <MeshInterface.h>
#include <SpriteSheetAnimation.h>
#include <PuppetScene.h>
#include <ItemGold.h>
#include <PuppetCharacter.h>
#include <shader\BaseComponentShader.h>

StructureGoldPile::StructureGoldPile(Box2DWorld* _world):
	StructureInteractable(_world, PuppetGame::kSTRUCTURE, PuppetGame::kPLAYER, -25),
	NodeChild(nullptr),
	NodeTransformable(new Transform()),
	totalGold(20),
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
	spriteSheet = new SpriteSheetAnimation(baseSpriteSheetTex, 0);

	// sprite sheet animation
	int f[] = {0,1,2,3,4,5};
	std::vector<int> ff(std::begin(f), std::end(f));

	spriteSheet->pushFramesInRange(0, 5, 512, 209, baseSpriteSheetTex->width);
	
	rootComponent->addAnimation("brickStates", spriteSheet, true);
	rootComponent->playAnimation = true;
	rootComponent->currentAnimation->currentFrame = 0;
	rootComponent->update(&vox::step);
	
	rootComponent->body->GetFixtureList()->SetSensor(true);
}

StructureGoldPile::~StructureGoldPile(){
	delete spriteSheet;
}

void StructureGoldPile::actuallyInteract(){
	if(playerWhoTriggered->heldItem == nullptr){

		if(goldTaken < totalGold){
			goldTaken++;
		
			int goldPerState = totalGold / 5;
			int frame = goldTaken/goldPerState;
			if(frame <= 5) {
				rootComponent->currentAnimation->currentFrame = frame;
				rootComponent->update(&vox::step);
			}

			PuppetScene * pScene = static_cast<PuppetScene *>(scene);
			ItemGold * gold = new ItemGold(pScene->world);
			gold->setShader(pScene->shader, true);
			gold->snapComponents(playerWhoTriggered->itemHolder);
			pScene->addChild(gold, 1);
			pScene->items.push_back(gold);
			playerWhoTriggered->pickupItem(gold);
			triggering = false;
		}
	}
}
void StructureGoldPile::evaluateState(){
	if(goldTaken < totalGold){
		ready = true;
		prepared = true;
	}else{
		ready = false;
		prepared = false;
	}
}