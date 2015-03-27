#pragma once

#include <PuppetCharacterKnight.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>

#include <RaidTheCastleResourceManager.h>

#include <Item.h>

PuppetCharacterKnight::PuppetCharacterKnight(bool _ai, unsigned long int _id, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
		RaidTheCastleResourceManager::knightTorso,
		RaidTheCastleResourceManager::knightArm,
		RaidTheCastleResourceManager::knightHelmet
	), _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{

}

PuppetCharacterKnight::~PuppetCharacterKnight(){
}