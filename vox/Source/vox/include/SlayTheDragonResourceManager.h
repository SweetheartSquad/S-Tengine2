#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "CharacterComponent.h"

class SoundManager;
class TextureSampler;
class Texture;

class SlayTheDragonResourceManager abstract : public ResourceManager{
public:
	static TextureSampler * splashMessage;

	static TextureSampler * fortBackground;
	static TextureSampler * fortForeground;
	static TextureSampler * fortStructure;
	static Texture * fortificationSpriteSheet;
	
	static TextureSampler * archerTorso;
	static TextureSampler * archerArm;
	static TextureSampler * archerHat;
	
	static TextureSampler * dragonHead;
	static TextureSampler * dragonMouth;
	static TextureSampler * dragonTorso;
	static TextureSampler * dragonUpperWing;
	static TextureSampler * dragonLowerWing;
	
	static TextureSampler * itemBow;
	static TextureSampler * itemArrow;
	static TextureSampler * itemFireball;
	static TextureSampler * itemFireParticle;
	
	static SoundManager * dragonSounds;

	static void init();
};