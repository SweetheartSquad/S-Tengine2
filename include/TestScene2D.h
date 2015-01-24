#pragma once

#include "Scene2D.h"

class Arduino;
class Box2DMeshEntity;
class Box2DDebugDraw;
class SpriteSheetAnimation;
class Box2DSprite;
class SoundManager;
class Texture;
class Sprite;
class Box2DWorld;

class TestScene2D : public Scene
{
public:
	
	Box2DWorld * world;

	Box2DSprite * sprite;
	Box2DMeshEntity * ground;
	Texture * tex;
	BaseComponentShader* shader;
	SoundManager * soundManager;
	SpriteSheetAnimation * spriteSheet;
	Box2DDebugDraw * drawer;
	Arduino * arduino;
	Sprite * frame; 

	Sprite * layer1;
	Sprite * layer2;

	explicit TestScene2D(Game * _game);
	virtual ~TestScene2D();

	void load() override;
	void unload() override;
	void update(Step * _step) override;
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
};