#pragma once

#include <PuppetCharacter.h>

class PuppetTexturePack;

class PuppetCharacterGuard : public PuppetCharacter{
public:

	PuppetCharacterGuard(bool _ai, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~PuppetCharacterGuard();
};