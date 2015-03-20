#pragma once

#include <PuppetCharacter.h>

class PuppetTexturePack;

class PuppetCharacterDragon : public PuppetCharacter{
public:

	PuppetCharacterDragon(bool _ai, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~PuppetCharacterDragon();

	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step * _step) override;
	void action() override;
	void pickupItem(Item * _item) override;
};