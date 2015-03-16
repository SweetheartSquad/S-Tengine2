#pragma once

#include <PuppetCharacterGuard.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>

#include <Item.h>

PuppetCharacterGuard::PuppetCharacterGuard(bool _ai, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
	RaidTheCastleResourceManager::knightRedTorso,
	RaidTheCastleResourceManager::knightRedArm,
	RaidTheCastleResourceManager::knightRedHelmet
	), _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{

}

PuppetCharacterGuard::~PuppetCharacterGuard(){
}