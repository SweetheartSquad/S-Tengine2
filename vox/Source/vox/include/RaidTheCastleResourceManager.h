#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "CharacterComponent.h"

class SoundManager;
class TextureSampler;
class Texture;

class RaidTheCastleResourceManager abstract : public ResourceManager{
public:
	static TextureSampler * splashMessage;

	static Texture * castleSpriteSheet;

	static TextureSampler * catapultArm;
	static TextureSampler * catapultBody;
	static TextureSampler * boulder;
	static TextureSampler * castleBase;

	static TextureSampler * knightTorso;
	static TextureSampler * knightArm;
	static TextureSampler * knightHelmet;

	static TextureSampler * castleChampionTorso;
	static TextureSampler * castleChampionArm;
	static TextureSampler * castleChampionHelmet;

	static TextureSampler * itemAxe;
	static TextureSampler * itemClub;
	static TextureSampler * itemMace;
	static TextureSampler * itemSword;
	static TextureSampler * itemScimitar;
	static TextureSampler * itemFlailGrip;
	static TextureSampler * itemFlailJoint;
	static TextureSampler * itemFlailHead;

	static TextureSampler * getRandomWeapon();
	
	static SoundManager * catapultSounds;
	
	static void init();
	//RaidTheCastleResourceManager();
};