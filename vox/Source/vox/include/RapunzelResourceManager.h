#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "CharacterComponent.h"
#include "Box2DSuperSprite.h"

class TextureSampler;
class Texture;

class RapunzelResourceManager abstract : public ResourceManager{
public:
	static TextureSampler * splashMessage;

	static Texture * goldSpriteSheet;

	static TextureSampler * leverHandle;
	static TextureSampler * leverBase;

	static TextureSampler * hair;

	/*
	static TextureSampler * knightRedTorso;
	static TextureSampler * knightRedArm;
	static TextureSampler * knightRedHelmet;

	static TextureSampler * knightGreenTorso;
	static TextureSampler * knightGreenArm;
	static TextureSampler * knightGreenHand;
	static TextureSampler * knightGreenHelmet;

	static TextureSampler * knightBlueTorso;
	static TextureSampler * knightBlueArm;
	static TextureSampler * knightBlueHelmet;

	static TextureSampler * knightYellowTorso;
	static TextureSampler * knightYellowArm;
	static TextureSampler * knightYellowHelmet;
	*/

	static TextureSampler * rapunzelRedTorso;
	static TextureSampler * rapunzelRedArm;
	static TextureSampler * rapunzelRedHelmet;

	static TextureSampler * rapunzelGreenTorso;
	static TextureSampler * rapunzelGreenArm;
	static TextureSampler * rapunzelGreenHelmet;

	static TextureSampler * rapunzelBlueTorso;
	static TextureSampler * rapunzelBlueArm;
	static TextureSampler * rapunzelBlueHelmet;

	static TextureSampler * rapunzelYellowTorso;
	static TextureSampler * rapunzelYellowArm;
	static TextureSampler * rapunzelYellowHelmet;
	
	static TextureSampler * itemAxe;
	static TextureSampler * itemClub;
	static TextureSampler * itemMace;
	static TextureSampler * itemSword;
	static TextureSampler * itemScimitar;

	static TextureSampler * getRandomWeapon();
	
	static void init();
};