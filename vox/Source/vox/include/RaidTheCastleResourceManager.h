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

	static TextureSampler * itemFlailGrip;
	static TextureSampler * itemFlailJoint;
	static TextureSampler * itemFlailHead;
	
	static SoundManager * catapultWhistleSounds;
	static SoundManager * catapultHitSounds;
	static SoundManager * catapultThrowSounds;
	
	static void init();
	//RaidTheCastleResourceManager();
};