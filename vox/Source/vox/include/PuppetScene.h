#pragma once

#include "Scene2D.h"

class GameJamCharacter;
class GameJamContactListener;
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

 class PuppetScene abstract : public Scene{
public:

GameJamContactListener * cl;
	bool debugDraw;
	Box2DDebugDraw * drawer;
	Box2DWorld * world;
	GameJamCharacter * playerCharacter;
	Box2DMeshEntity * ground;
	BaseComponentShader* shader;
	SoundManager * soundManager;

	std::vector<Box2DSprite *> items;

	explicit PuppetScene(Game * _game);
	virtual ~PuppetScene();

	void load() override;
	void unload() override;
	void update(Step * _step) override;
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;

	enum HURLYBURLY_CATEGORY{
		BOUNDARY = 0x0001,
		PROP = 0x0002,
		NPC = 0x0004,
		PLAYER = 0x00010
	};
};