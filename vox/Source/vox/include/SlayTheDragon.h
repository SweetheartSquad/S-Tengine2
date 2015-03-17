#pragma once

#include "PuppetScene.h"

class MeshEntity;

class SlayTheDragon : public PuppetScene{
public:

	MeshEntity * sky;

	PuppetCharacter * playerCharacter1;
	PuppetCharacter * playerCharacter2;
	PuppetCharacter * playerCharacter3;
	PuppetCharacter * playerCharacter4;

	explicit SlayTheDragon(PuppetGame * _game);
	~SlayTheDragon();

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	virtual void load() override;
	virtual void unload() override;
};