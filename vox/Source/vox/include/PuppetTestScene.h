#pragma once

#include "PuppetScene.h"

class PuppetTestScene : public PuppetScene{
public:


	explicit PuppetTestScene(Game * _game);
	~PuppetTestScene();

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	virtual void load() override;
	virtual void unload() override;
};