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
class CylinderScreen;
class TestCharacter;

class GameJamScene : public Scene{
public:
	Box2DDebugDraw * drawer;
	Box2DWorld * world;
	TestCharacter * playerCharacter;
	Box2DMeshEntity * ground;
	Texture * tex;
	BaseComponentShader* shader;
	SoundManager * soundManager;
	SpriteSheetAnimation * spriteSheet;
	CylinderScreen * backgroundScreen;
	CylinderScreen * midgroundScreen;
	CylinderScreen * foregroundScreen;

	std::vector<Box2DSprite *> items;

	explicit GameJamScene(Game * _game);
	virtual ~GameJamScene();

	void load() override;
	void unload() override;
	void update(Step * _step) override;
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
};