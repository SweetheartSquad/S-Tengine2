#pragma once

#include "RaidTheCastleResourceManager.h"
#include <TextureSampler.h>
#include <SoundManager.h>

Texture * RaidTheCastleResourceManager::castleSpriteSheet    = new Texture("../assets/structure components/castle/Castle_SpriteSheet.png", 2048, 2048, true, true);

TextureSampler * RaidTheCastleResourceManager::catapultArm   = new TextureSampler(new Texture("../assets/structure components/catapult/CatapultFlinger.png", 512, 512, true, true), 429, 76);;
TextureSampler * RaidTheCastleResourceManager::catapultBody  = new TextureSampler(new Texture("../assets/structure components/catapult/CatapultBase.png", 512, 512, true, true), 418, 264);
TextureSampler * RaidTheCastleResourceManager::boulder		 = new TextureSampler(new Texture("../assets/structure components/catapult/Boulder1.png", 512, 512, true, true), 108, 103);
TextureSampler * RaidTheCastleResourceManager::castleBase    = new TextureSampler(new Texture("../assets/structure components/castle/CastleNorm_State1.png", 1024, 1024, true, true), 973, 619);

TextureSampler * RaidTheCastleResourceManager::knightTorso	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/KnightBreastplate.png", 1024, 1024, true, true), 268, 776);
TextureSampler * RaidTheCastleResourceManager::knightArm	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/KnightArm.png", 1024, 1024, true, true), 92, 327);
TextureSampler * RaidTheCastleResourceManager::knightHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/KnightHelmet.png", 1024, 1024, true, true), 233, 254);

TextureSampler * RaidTheCastleResourceManager::castleChampionTorso	= new TextureSampler(new Texture("../assets/hurly-burly/CastleChampionAssets/CCBody.png", 1024, 1024, true, true), 275, 806);
TextureSampler * RaidTheCastleResourceManager::castleChampionArm	= new TextureSampler(new Texture("../assets/hurly-burly/CastleChampionAssets/CCArm.png", 1024, 1024, true, true), 67, 200);
TextureSampler * RaidTheCastleResourceManager::castleChampionHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/CastleChampionAssets/CCHelmet.png", 1024, 1024, true, true), 229, 295);

TextureSampler * RaidTheCastleResourceManager::splashMessage	= new TextureSampler(new Texture("../assets/hurly-burly/SplashMessages/RaidTheCastle.png", 1024, 1024, true, true), 1024, 1024);

TextureSampler * RaidTheCastleResourceManager::itemAxe	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Axe.png", 1024, 1024, true, true), 230, 395);
TextureSampler * RaidTheCastleResourceManager::itemClub	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Club.png", 1024, 1024, true, true), 83, 515);
TextureSampler * RaidTheCastleResourceManager::itemMace	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Mace.png", 1024, 1024, true, true), 178, 516);
TextureSampler * RaidTheCastleResourceManager::itemSword	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Sword.png", 1024, 1024, true, true), 81, 382);
TextureSampler * RaidTheCastleResourceManager::itemScimitar	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Scimitar.png", 1024, 1024, true, true), 79, 401);
TextureSampler * RaidTheCastleResourceManager::itemFlailGrip	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/FlailGrip.png", 1024, 1024, true, true), 51, 134);
TextureSampler * RaidTheCastleResourceManager::itemFlailJoint	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/FlailJoint.png", 1024, 1024, true, true), 67, 66);
TextureSampler * RaidTheCastleResourceManager::itemFlailHead	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/FlailHead.png", 1024, 1024, true, true), 178, 178);

SoundManager * RaidTheCastleResourceManager::catapultSounds = new SoundManager(-1);

void RaidTheCastleResourceManager::init(){

	resources.push_back(splashMessage);

	resources.push_back(catapultArm);
	resources.push_back(catapultBody);
	resources.push_back(boulder);

	resources.push_back(knightTorso);
	resources.push_back(knightArm);
	resources.push_back(knightHelmet);
	
	resources.push_back(castleChampionTorso);
	resources.push_back(castleChampionArm);
	resources.push_back(castleChampionHelmet);
	
	resources.push_back(itemAxe);
	resources.push_back(itemClub);
	resources.push_back(itemMace);
	resources.push_back(itemSword);
	resources.push_back(itemScimitar);
	resources.push_back(itemFlailGrip);
	resources.push_back(itemFlailJoint);
	resources.push_back(itemFlailHead);

	resources.push_back(catapultSounds);

	catapultSounds->addNewSound("1", "../assets/hurly-burly/audio/dragon/dragonSound12.ogg");
}

TextureSampler * RaidTheCastleResourceManager::getRandomWeapon(){
	unsigned long int i = std::rand() % 5;
	switch(i){
	default:
	case 0:
		return itemAxe;
	case 1:
		return itemClub;
	case 2:
		return itemMace;
	case 3:
		return itemSword;
	case 4:
		return itemScimitar;
	}
}