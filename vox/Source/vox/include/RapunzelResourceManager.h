#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "CharacterComponent.h"

class TextureSampler;
class Texture;

class RapunzelResourceManager abstract : public ResourceManager{
public:
	static TextureSampler * splashMessage;
	
	static Texture * goldSpriteSheet;
	static TextureSampler * goldBrick;
	
	static TextureSampler * glove;
	static TextureSampler * spring;

	static TextureSampler * leverHandle;
	static TextureSampler * leverBase;

	static TextureSampler * hair;

	
	static TextureSampler * thiefTorso;
	static TextureSampler * thiefArm;
	static TextureSampler * thiefHelmet;

	static TextureSampler * rapunzelTorso;
	static TextureSampler * rapunzelArm;
	static TextureSampler * rapunzelHelmet;
	
	static TextureSampler * itemSpear;
	static TextureSampler * itemAxe;
	static TextureSampler * itemClub;
	static TextureSampler * itemMace;
	static TextureSampler * itemSword;
	static TextureSampler * itemScimitar;

	static TextureSampler * getRandomWeapon();
	
	static void init();
};