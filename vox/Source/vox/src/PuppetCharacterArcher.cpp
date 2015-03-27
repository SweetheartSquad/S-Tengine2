#pragma once

#include <PuppetCharacterArcher.h>
#include <PuppetTexturePack.h>

#include <SlayTheDragonResourceManager.h>

PuppetCharacterArcher::PuppetCharacterArcher(bool _ai,  float _ghostPosition, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(SlayTheDragonResourceManager::archerTorso, SlayTheDragonResourceManager::archerArm, SlayTheDragonResourceManager::archerHat), _ghostPosition, _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
}

PuppetCharacterArcher::~PuppetCharacterArcher(){
}