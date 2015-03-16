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

	
	static TextureSampler * thiefRedTorso;
	static TextureSampler * thiefRedArm;
	static TextureSampler * thiefRedHelmet;
							
	static TextureSampler * thiefGreenTorso;
	static TextureSampler * thiefGreenArm;
	static TextureSampler * thiefGreenHand;
	static TextureSampler * thiefGreenHelmet;
							
	static TextureSampler * thiefBlueTorso;
	static TextureSampler * thiefBlueArm;
	static TextureSampler * thiefBlueHelmet;
							
	static TextureSampler * thiefYellowTorso;
	static TextureSampler * thiefYellowArm;
	static TextureSampler * thiefYellowHelmet;
	

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