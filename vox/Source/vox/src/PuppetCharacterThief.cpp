#pragma once

#include <PuppetCharacterThief.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>

#include <RapunzelResourceManager.h>

#include <Item.h>
#include <ItemGold.h>

PuppetCharacterThief::PuppetCharacterThief(bool _ai, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
		RapunzelResourceManager::thiefTorso,
		RapunzelResourceManager::thiefArm,
		RapunzelResourceManager::thiefHelmet
	), _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{

}

PuppetCharacterThief::~PuppetCharacterThief(){
}

void PuppetCharacterThief::pickupItem(Item * _itemToPickup){
	ItemGold * gold = dynamic_cast<ItemGold *>(_itemToPickup);

	if(gold != nullptr){
		gold->destroy = true;
		score += 1;
		itemToPickup = nullptr;
	}else{
		PuppetCharacter::pickupItem(_itemToPickup);
	}
}