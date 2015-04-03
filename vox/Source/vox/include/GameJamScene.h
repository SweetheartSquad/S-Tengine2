#pragma once

#include "LayeredScene.h"

class DialogHandler;
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
class GameJamCharacter;

 class GameJamScene abstract : public LayeredScene{
public:
	Box2DWorld * world;
	Box2DDebugDraw * drawer;
	GameJamCharacter * playerCharacter;
	Box2DMeshEntity * ground;
	BaseComponentShader * shader;
	SoundManager * soundManager;
	SpriteSheetAnimation * spriteSheet;
	CylinderScreen * backgroundScreen;
	CylinderScreen * midgroundScreen;
	CylinderScreen * foregroundScreen;
	DialogHandler * dialogHandler;

	std::vector<Box2DSprite *> items;

	explicit GameJamScene(Game * _game);
	virtual ~GameJamScene();

	void load() override;
	void unload() override;
	void update(Step * _step) override;
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;

	enum GAMEJAM_CATEGORY{
		BOUNDARY = 0x0001,
		PROP = 0x0002,
		NPC = 0x0004,
		PLAYER = 0x00010
	};
};