#pragma once

#include <PuppetScene.h>

class PuppetGame;

class VictoryScene : public PuppetScene{
public:

	VictoryScene(PuppetGame * _game, std::vector<PuppetCharacter *> players);
	~VictoryScene();

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	virtual void load() override;
	virtual void unload() override;

	virtual void doCountDown() override;
};