#pragma once

#include "Scene2D.h"

class MousePerspectiveCamera;
class PerspectiveCamera;
class MeshEntity;
class PuppetController;
class PuppetCharacter;
class GameJamCharacter;
class GameJamContactListener;
class DialogHandler;
class AccelerometerParser;
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
	Box2DWorld * world;
	Box2DDebugDraw * drawer;
	PuppetCharacter * playerCharacter;
	Box2DMeshEntity * ground;
	MeshEntity * background;
	BaseComponentShader* shader;
	SoundManager * soundManager;
	AccelerometerParser * arduino;
	PuppetController * puppetController;

	PerspectiveCamera * perspectiveCamera;
	MousePerspectiveCamera * mouseCamera;

	Box2DSprite * tempCatapault;

	bool mouseCam;

	std::vector<Box2DSprite *> items;

	explicit PuppetScene(Game * _game);
	virtual ~PuppetScene();

	virtual void load() override;
	virtual void unload() override;
	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	
	enum HURLYBURLY_CATEGORY{
		BOUNDARY = 0x0001,
		PROP = 0x0002,
		NPC = 0x0004,
		PLAYER = 0x00010
	};
};