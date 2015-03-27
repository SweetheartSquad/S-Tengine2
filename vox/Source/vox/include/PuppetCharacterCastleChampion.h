#pragma once

#include <PuppetCharacter.h>

class PuppetTexturePack;

class PuppetCharacterCastleChampion : public PuppetCharacter{
public:

	PuppetCharacterCastleChampion(Box2DWorld * _world, float _ghostPosition, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~PuppetCharacterCastleChampion();

	void action() override;

	void update(Step * _step) override;
};