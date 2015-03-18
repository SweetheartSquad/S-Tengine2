#pragma once

#include <PuppetCharacter.h>

class PuppetTexturePack;

class PuppetCharacterDragon : public PuppetCharacter{
public:

	PuppetCharacterDragon(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~PuppetCharacterDragon();

	void action() override;

	void update(Step * _step) override;
};