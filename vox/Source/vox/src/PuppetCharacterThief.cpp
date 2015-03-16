#pragma once

#include <PuppetCharacterThief.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>

#include <RapunzelResourceManager.h>

#include <Item.h>

PuppetCharacterThief::PuppetCharacterThief(bool _ai, unsigned long int _id, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
		(_id == 0) ? RapunzelResourceManager::thiefRedTorso : (_id == 1 ? RapunzelResourceManager::thiefGreenTorso : (_id == 2 ? RapunzelResourceManager::thiefBlueTorso : RapunzelResourceManager::thiefYellowTorso)),
		(_id == 0) ? RapunzelResourceManager::thiefRedArm : (_id == 1 ? RapunzelResourceManager::thiefGreenArm : (_id == 2 ? RapunzelResourceManager::thiefBlueArm : RapunzelResourceManager::thiefYellowArm)),
		(_id == 0) ? RapunzelResourceManager::thiefRedHelmet : (_id == 1 ? RapunzelResourceManager::thiefGreenHelmet : (_id == 2 ? RapunzelResourceManager::thiefBlueHelmet : RapunzelResourceManager::thiefYellowHelmet))
	), _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{

}

PuppetCharacterThief::~PuppetCharacterThief(){
}