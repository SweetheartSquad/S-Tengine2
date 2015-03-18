#pragma once

#include <PuppetCharacterArcher.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>

#include <SlayTheDragonResourceManager.h>

#include <Item.h>

PuppetCharacterArcher::PuppetCharacterArcher(bool _ai, unsigned long int _id, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
		(_id == 0) ? SlayTheDragonResourceManager::archerRedTorso : (_id == 1 ? SlayTheDragonResourceManager::archerGreenTorso : (_id == 2 ? SlayTheDragonResourceManager::archerBlueTorso : SlayTheDragonResourceManager::archerYellowTorso)),
		(_id == 0) ? SlayTheDragonResourceManager::archerRedArm : (_id == 1 ? SlayTheDragonResourceManager::archerGreenArm : (_id == 2 ? SlayTheDragonResourceManager::archerBlueArm : SlayTheDragonResourceManager::archerYellowArm)),
		(_id == 0) ? SlayTheDragonResourceManager::archerRedHelmet : (_id == 1 ? SlayTheDragonResourceManager::archerGreenHelmet : (_id == 2 ? SlayTheDragonResourceManager::archerBlueHelmet : SlayTheDragonResourceManager::archerYellowHelmet))
	), _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{

}

PuppetCharacterArcher::~PuppetCharacterArcher(){
}