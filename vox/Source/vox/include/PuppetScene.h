#pragma once

#include "Scene.h"

class PuppetScene : public Scene{
public:

	explicit PuppetScene(Game * _game);
	~PuppetScene();

	void update(Step * _step) override;
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void load() override;
	void unload() override;

};