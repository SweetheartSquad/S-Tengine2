#pragma once

#include "LayeredScene.h"

class RandomGround;
class MousePerspectiveCamera;
class PerspectiveCamera;
class MeshEntity;
class PuppetController;
class PuppetCharacter;
class GameJamCharacter;
class Catapult;
class RaidTheCastleContactListener;
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

 class PuppetScene abstract : public LayeredScene{
public:

	float time;
	RaidTheCastleContactListener * cl;
	Box2DWorld * world;
	Box2DDebugDraw * drawer;

	PuppetCharacter * playerCharacter;
	PuppetCharacter * playerCharacter2;
	PuppetCharacter * playerCharacter3;
	PuppetCharacter * playerCharacter4;

	PuppetController * puppetController;
	PuppetController * puppetController2;
	PuppetController * puppetController3;
	PuppetController * puppetController4;

	Box2DMeshEntity * ground;
	RandomGround * randomGround;
	MeshEntity * background;
	BaseComponentShader* shader;
	SoundManager * soundManager;
	AccelerometerParser * arduino;

	PerspectiveCamera * perspectiveCamera;
	MousePerspectiveCamera * mouseCamera;

	Box2DSprite * tempCatapault;

	bool mouseCam;

	std::vector<Box2DSprite *> items;

	explicit PuppetScene(Game * _game, float time);
	virtual ~PuppetScene();

	virtual void load() override;
	virtual void unload() override;
	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	
	enum HURLYBURLY_CATEGORY{
		kBOUNDARY = 0x0001,
		kSTRUCTURE = 0x0002,
		kITEM = 0x0004,
		kGROUND = 0x0008,
		kPLAYER = 0x00010,
		kBEHAVIOUR = 0x00020,
		kCAT7 = 0x00040,
		kCAT8 = 0x00080,
		kCAT9 = 0x00100,
		kCAT10 = 0x00200,
		kCAT11 = 0x00400,
		kCAT12 = 0x00800
	};
};