#pragma once

#include <PuppetCharacterKnight.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>

#include <Item.h>

PuppetCharacterKnight::PuppetCharacterKnight(bool _ai, unsigned long int _id, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
		(_id == 0) ? RaidTheCastleResourceManager::knightRedTorso : (_id == 1 ? RaidTheCastleResourceManager::knightGreenTorso : (_id == 2 ? RaidTheCastleResourceManager::knightBlueTorso : RaidTheCastleResourceManager::knightYellowTorso)),
		(_id == 0) ? RaidTheCastleResourceManager::knightRedArm : (_id == 1 ? RaidTheCastleResourceManager::knightGreenArm : (_id == 2 ? RaidTheCastleResourceManager::knightBlueArm : RaidTheCastleResourceManager::knightYellowArm)),
		(_id == 0) ? RaidTheCastleResourceManager::knightRedHelmet : (_id == 1 ? RaidTheCastleResourceManager::knightGreenHelmet : (_id == 2 ? RaidTheCastleResourceManager::knightBlueHelmet : RaidTheCastleResourceManager::knightYellowHelmet))
	), _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
	
	TextureSampler * weapon = RaidTheCastleResourceManager::getRandomWeapon();
	itemToPickup = new Item(false, _world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kBOUNDARY | PuppetGame::kGROUND, _groupIndex, 0, 0, -weapon->height/2.5f);

	//itemToPickup->translateComponents(torso->transform->translationVector);

	componentScale = 0.008f;

	
	itemToPickup->rootComponent = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), weapon->width, weapon->height, weapon->texture, componentScale/4);
	itemToPickup->components.push_back(&itemToPickup->rootComponent);
	
	b2Filter sf;
	sf.categoryBits = itemToPickup->categoryBits;
	if(itemToPickup->maskBits != (int16)-1){
		sf.maskBits = itemToPickup->maskBits;
	}else{
		sf.maskBits = 0;
	}
	sf.groupIndex = groupIndex;

	for(Box2DSprite ** c : itemToPickup->components){
		(*c)->createFixture(sf);
		(*c)->body->GetFixtureList()->SetDensity(0.01f);
		(*c)->body->ResetMassData();
	}

	itemToPickup->setUserData(itemToPickup);

	//itemToPickup->snapComponents(handRight);
}

PuppetCharacterKnight::~PuppetCharacterKnight(){
}