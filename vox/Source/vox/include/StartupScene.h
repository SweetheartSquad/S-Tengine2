#pragma once

#include <PuppetScene.h>

class StartupScene : public PuppetScene{
public:
	StartupScene(PuppetGame * _game);
	void complete(std::string _switchTo = "") override;
	void update(Step * _step) override;
	void triggerVictoryState();

	void populateBackground() override;
};

