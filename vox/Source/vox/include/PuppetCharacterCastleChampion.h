#pragma once

#include <PuppetCharacter.h>

class PuppetCharacterCastleChampion : public PuppetCharacter{
public:
	PuppetCharacterCastleChampion(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);

	void action() override;
};