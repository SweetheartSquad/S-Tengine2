#pragma once

#include <PuppetCharacter.h>

class PuppetTexturePack;

class PuppetCharacterDragon : public PuppetCharacter{
public:

	PuppetCharacterDragon(bool _ai, unsigned long int _id, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~PuppetCharacterDragon();

	void update(Step * _step) override;
};