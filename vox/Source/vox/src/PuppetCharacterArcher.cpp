#pragma once

#include <PuppetCharacterArcher.h>
#include <PuppetTexturePack.h>

#include <SlayTheDragonResourceManager.h>

PuppetCharacterArcher::PuppetCharacterArcher(bool _ai, unsigned long int _id, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(SlayTheDragonResourceManager::archerTorso, SlayTheDragonResourceManager::archerArm, SlayTheDragonResourceManager::archerHat), _ai, _world, _categoryBits, _maskBits, _groupIndex, _id),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
}

PuppetCharacterArcher::~PuppetCharacterArcher(){
}