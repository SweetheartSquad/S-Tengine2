#pragma once

#include "PuppetScene.h"

class PuppetCharacterKnight;

class FightYourFriends : public PuppetScene{
public:
	bool camTargetsRemoved;
	float itemDropFreq;

	PuppetCharacterKnight * playerCharacter1;
	PuppetCharacterKnight * playerCharacter2;
	PuppetCharacterKnight * playerCharacter3;
	PuppetCharacterKnight * playerCharacter4;

	explicit FightYourFriends(PuppetGame * _game);
	~FightYourFriends();

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	virtual void load() override;
	virtual void unload() override;

	void populateBackground() override;
};