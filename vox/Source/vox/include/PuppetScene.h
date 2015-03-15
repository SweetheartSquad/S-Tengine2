#pragma once

#include "LayeredScene.h"

class RandomGround;
class MousePerspectiveCamera;
class PerspectiveCamera;
class FollowCamera;
class MeshEntity;
class PuppetController;
class PuppetCharacter;
class GameJamCharacter;
class Catapult;
class PuppetContactListener;
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
class PuppetGame;
class Item;
class NodeBox2DBody;

 class PuppetScene abstract : public LayeredScene{
public:

	float duration;
	float currentTime;
	unsigned int countDown;
	std::vector<Sprite *> countDownNumbers;

	Sprite * splashMessage;
	bool displayingSplash;
	float splashDuration;

	PuppetContactListener * cl;
	Box2DWorld * world;
	Box2DDebugDraw * drawer;

	PuppetController * puppetController;
	PuppetController * puppetController2;
	PuppetController * puppetController3;
	PuppetController * puppetController4;

	std::vector<PuppetCharacter *> players;

	MeshEntity * ground;
	RandomGround * randomGround;
	MeshEntity * background;
	BaseComponentShader* shader;

	std::vector<Box2DMeshEntity *> boundaries;

	SoundManager * soundManager;
	SoundManager * countdownSoundManager;
	SoundManager * backgroundSoundManager;

	AccelerometerParser * arduino;

	FollowCamera * gameCam;
	MousePerspectiveCamera * mouseCamera;

	bool mouseCam;

	std::vector<Item *> items;

	explicit PuppetScene(PuppetGame * _game, float time);
	virtual ~PuppetScene();

	virtual void load() override;
	virtual void unload() override;
	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;

	virtual void complete();

	void destroyItem(Item * item);
	void doCountDown();
	void playRandomBackgroundMusic();
};