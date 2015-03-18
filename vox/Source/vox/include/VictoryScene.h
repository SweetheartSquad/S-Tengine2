#pragma once

#include <PuppetScene.h>

class PuppetGame;

class VictoryScene : public PuppetScene{
public:

	PuppetCharacter * playerCharacter1;
	PuppetCharacter * playerCharacter2;
	PuppetCharacter * playerCharacter3;
	PuppetCharacter * playerCharacter4;

	VictoryScene(PuppetGame * _game, PuppetCharacter * _p1, PuppetCharacter * _p2, PuppetCharacter * _p3, PuppetCharacter * _p4);
	~VictoryScene();

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	virtual void load() override;
	virtual void unload() override;
};