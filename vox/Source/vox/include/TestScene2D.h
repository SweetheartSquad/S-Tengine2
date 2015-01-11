#pragma once

#include "Scene2D.h"

class Box2DDebugDraw;
class SpriteSheetAnimation;
class Box2DSprite;
class SoundManager;
class Texture;
class Sprite;
class Box2DWorld;

class TestScene2D : public Scene2D
{
public:

	Box2DSprite * sprite;
	Box2DSprite * ground;
	Box2DWorld * world;
	Texture * tex;
	BaseComponentShader* shader;
	SoundManager * soundManager;
	SpriteSheetAnimation * spriteSheet;
	Box2DDebugDraw * drawer;

	explicit TestScene2D(Game * _game);
	virtual ~TestScene2D();

	void load() override;
	void unload() override;
	void update(Step * _step) override;
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
};