#pragma once

#include "Scene2D.h"
#include "GameJamScene.h"

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

class GameJamSceneOutdoor : public GameJamScene{
public:
	explicit GameJamSceneOutdoor(Game * _game);
	void update(Step * _step) override;
};