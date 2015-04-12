#pragma once

#include <PuppetScene.h>

class PuppetCharacterKnight;

class StartupScene : public PuppetScene{
public:

	PuppetCharacterKnight * playerCharacter1;
	PuppetCharacterKnight * playerCharacter2;
	PuppetCharacterKnight * playerCharacter3;
	PuppetCharacterKnight * playerCharacter4;

	StartupScene(PuppetGame * _game);
	void complete(std::string _switchTo = "") override;
	void update(Step * _step) override;
	void triggerVictoryState() override;

	void populateBackground() override;
};

