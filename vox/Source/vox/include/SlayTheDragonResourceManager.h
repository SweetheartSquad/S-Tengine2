#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "CharacterComponent.h"
#include "Box2DSuperSprite.h"

class TextureSampler;
class Texture;

class SlayTheDragonResourceManager abstract : public ResourceManager{
public:
	static TextureSampler * splashMessage;

	static Texture * fortificationSpriteSheet;
	
	static TextureSampler * archerTorso;
	static TextureSampler * archerArm;
	static TextureSampler * archerHelmet;
	
	static TextureSampler * dragonHead;
	static TextureSampler * dragonMouth;
	static TextureSampler * dragonTorso;
	static TextureSampler * dragonUpperWing;
	static TextureSampler * dragonLowerWing;
	
	static TextureSampler * itemBow;
	static TextureSampler * itemArrow;
	static TextureSampler * itemFireball;
	static TextureSampler * itemNone;
	
	static void init();
};