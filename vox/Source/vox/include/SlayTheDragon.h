#pragma once

#include "PuppetScene.h"

class MeshEntity;
class PuppetCharacterDragon;
class PuppetCharacterArcher;
class Fortification;

class SlayTheDragon : public PuppetScene{
public:

	MeshEntity * sky;

	Sprite * fortBackground;
	Box2DSprite * fortForeground;
	Fortification * fort;

	PuppetCharacterDragon * dragon;

	PuppetCharacterArcher * playerCharacter1;
	PuppetCharacterArcher * playerCharacter2;
	PuppetCharacterArcher * playerCharacter3;
	PuppetCharacterArcher * playerCharacter4;

	explicit SlayTheDragon(PuppetGame * _game);
	~SlayTheDragon();

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	virtual void load() override;
	virtual void unload() override;
};