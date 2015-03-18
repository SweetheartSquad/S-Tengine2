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
	
	static TextureSampler * dragonTorso;
	static TextureSampler * dragonArm;
	static TextureSampler * dragonHand;
	static TextureSampler * dragonLeg;
	static TextureSampler * dragonHead;
	
	static TextureSampler * itemBow;
	static TextureSampler * itemArrow;
	static TextureSampler * itemFireball;
	
	static void init();
};