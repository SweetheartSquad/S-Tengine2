#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "CharacterComponent.h"
#include "Box2DSuperSprite.h"

class TextureSampler;
class Texture;

class RaidTheCastleResourceManager abstract : public ResourceManager{
public:
	static Texture * castleSpriteSheet;

	static TextureSampler * catapultArm;
	static TextureSampler * catapultBody;
	static TextureSampler * boulder;
	static TextureSampler * castleBase;
	/*
	static TextureSampler * goldenBreastPlateStick;
	static TextureSampler * goldenArm;
	static TextureSampler * goldenHelmet;
	*/
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

	static TextureSampler * castleChampionTorso;
	static TextureSampler * castleChampionArm;
	static TextureSampler * castleChampionHelmet;
	
	static void init();
	//RaidTheCastleResourceManager();
};