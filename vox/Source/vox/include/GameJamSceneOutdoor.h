#pragma once

#include "Scene2D.h"

class DialogEvent;
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
class Character;
class TestCharacter;
class Character1;
class Character2;
class Character3;
class Character4;

class GameJamSceneOutdoor : public Scene{
public:
	bool debugDraw;
	Box2DDebugDraw * drawer;
	Box2DWorld * world;
	Character * playerCharacter;
	Box2DMeshEntity * ground;
	BaseComponentShader* shader;
	SoundManager * soundManager;
	SpriteSheetAnimation * spriteSheet;
	CylinderScreen * backgroundScreen;
	CylinderScreen * midgroundScreen;
	CylinderScreen * foregroundScreen;
	DialogEvent * dialogEvent;

	std::vector<Box2DSprite *> items;

	explicit GameJamSceneOutdoor(Game * _game);
	virtual ~GameJamSceneOutdoor();

	void load() override;
	void unload() override;
	void update(Step * _step) override;
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
};