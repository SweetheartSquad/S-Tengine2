#pragma once

#include "PuppetScene.h"

class PuppetTestScene : public PuppetScene{
	explicit PuppetTestScene(Game * _game);
	~PuppetTestScene();

	void update(Step * _step) override;
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void load() override;
	void unload() override;
};