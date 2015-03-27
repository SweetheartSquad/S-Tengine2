#pragma once

#include <PuppetCharacter.h>

class PuppetTexturePack;

class PuppetCharacterThief : public PuppetCharacter{
public:

	PuppetCharacterThief(bool _ai, float _ghostPosition, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~PuppetCharacterThief();
};