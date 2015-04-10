#pragma once

#include <PuppetCharacterArcher.h>
#include <PuppetTexturePack.h>

#include <SlayTheDragonResourceManager.h>
#include <SoundManager.h>

PuppetCharacterArcher::PuppetCharacterArcher(bool _ai, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(SlayTheDragonResourceManager::archerTorso, SlayTheDragonResourceManager::archerArm, SlayTheDragonResourceManager::archerHat), _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	onFire(false)
{
}

PuppetCharacterArcher::~PuppetCharacterArcher(){
}

void PuppetCharacterArcher::update(Step * _step){
	PuppetCharacter::update(_step);
	if(onFire){
		onFire = false;
		if(!dead){
			SlayTheDragonResourceManager::fireSounds->playRandomSound();
		}
	}
}