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

}

PuppetCharacterKnight::~PuppetCharacterKnight(){
}