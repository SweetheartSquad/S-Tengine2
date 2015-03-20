#pragma once

#include <PuppetCharacterThief.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>

#include <RapunzelResourceManager.h>

#include <Item.h>

PuppetCharacterThief::PuppetCharacterThief(bool _ai, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
		RapunzelResourceManager::thiefRedTorso,
		RapunzelResourceManager::thiefRedArm,
		RapunzelResourceManager::thiefRedHelmet
	), _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{

}

PuppetCharacterThief::~PuppetCharacterThief(){
}