#pragma once

#include "RaidTheCastleResourceManager.h"
#include <TextureSampler.h>
#include <SoundManager.h>


TextureSampler * RaidTheCastleResourceManager::catapultArm   = new TextureSampler(new Texture("../assets/hurly-burly/Catapult/CatapultFlinger.png", 512, 512, true, true), 429, 76);
TextureSampler * RaidTheCastleResourceManager::catapultBody  = new TextureSampler(new Texture("../assets/hurly-burly/Catapult/CatapultBase.png", 512, 512, true, true), 418, 264);
TextureSampler * RaidTheCastleResourceManager::boulder		 = new TextureSampler(new Texture("../assets/hurly-burly/Catapult/Boulder1.png", 512, 512, true, true), 108, 103);
TextureSampler * RaidTheCastleResourceManager::castleBase = new TextureSampler(new Texture("../assets/hurly-burly/Castle/CastleNorm_State1.png", 1024, 1024, true, true), 973, 619);
Texture * RaidTheCastleResourceManager::castleSpriteSheet = new Texture("../assets/hurly-burly/Castle/Castle_SpriteSheet.png", 2048, 2048, true, true);

TextureSampler * RaidTheCastleResourceManager::knightTorso	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/KnightBreastplate.png", 1024, 1024, true, true), 268, 776);
TextureSampler * RaidTheCastleResourceManager::knightArm	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/KnightArm.png", 1024, 1024, true, true), 92, 327);
TextureSampler * RaidTheCastleResourceManager::knightHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/KnightHelmet.png", 1024, 1024, true, true), 233, 254);

TextureSampler * RaidTheCastleResourceManager::castleChampionTorso	= new TextureSampler(new Texture("../assets/hurly-burly/CastleChampionAssets/CCBody.png", 1024, 1024, true, true), 275, 806);
TextureSampler * RaidTheCastleResourceManager::castleChampionArm	= new TextureSampler(new Texture("../assets/hurly-burly/CastleChampionAssets/CCArm.png", 1024, 1024, true, true), 67, 200);
TextureSampler * RaidTheCastleResourceManager::castleChampionHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/CastleChampionAssets/CCHelmet.png", 1024, 1024, true, true), 229, 295);

TextureSampler * RaidTheCastleResourceManager::splashMessage	= new TextureSampler(new Texture("../assets/hurly-burly/SplashMessages/RaidTheCastle.png", 1024, 1024, true, true), 1024, 1024);

TextureSampler * RaidTheCastleResourceManager::itemFlailGrip	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/FlailGrip.png", 1024, 1024, true, true), 51, 134);
TextureSampler * RaidTheCastleResourceManager::itemFlailJoint	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/FlailJoint.png", 1024, 1024, true, true), 67, 66);
TextureSampler * RaidTheCastleResourceManager::itemFlailHead	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/FlailHead.png", 1024, 1024, true, true), 178, 178);

SoundManager * RaidTheCastleResourceManager::catapultHitSounds = new SoundManager(-1);
SoundManager * RaidTheCastleResourceManager::catapultThrowSounds = new SoundManager(-1);
SoundManager * RaidTheCastleResourceManager::catapultWhistleSounds = new SoundManager(-1);

void RaidTheCastleResourceManager::init(){

	resources.push_back(splashMessage);
	resources.push_back(castleSpriteSheet);
	resources.push_back(catapultArm);
	resources.push_back(catapultBody);
	resources.push_back(boulder);
	resources.push_back(castleBase);

	resources.push_back(knightTorso);
	resources.push_back(knightArm);
	resources.push_back(knightHelmet);
	
	resources.push_back(castleChampionTorso);
	resources.push_back(castleChampionArm);
	resources.push_back(castleChampionHelmet);
	
	resources.push_back(itemFlailGrip);
	resources.push_back(itemFlailJoint);
	resources.push_back(itemFlailHead);
	
	catapultHitSounds->addNewSound("1", "../assets/hurly-burly/audio/catapultAudio/crash1.ogg");
	catapultHitSounds->addNewSound("2", "../assets/hurly-burly/audio/catapultAudio/crash2.ogg");
	catapultHitSounds->addNewSound("3", "../assets/hurly-burly/audio/catapultAudio/crash3.ogg");
	catapultHitSounds->addNewSound("4", "../assets/hurly-burly/audio/catapultAudio/crash4.ogg");
	catapultHitSounds->addNewSound("5", "../assets/hurly-burly/audio/catapultAudio/crash5.ogg");
	catapultHitSounds->addNewSound("6", "../assets/hurly-burly/audio/catapultAudio/crash6.ogg");
	catapultHitSounds->addNewSound("7", "../assets/hurly-burly/audio/puppetDialog_Castle/castleHit1.ogg");
	catapultHitSounds->addNewSound("8", "../assets/hurly-burly/audio/puppetDialog_Castle/castleHit2.ogg");
	catapultHitSounds->addNewSound("9", "../assets/hurly-burly/audio/puppetDialog_Castle/castleHit3.ogg");
	catapultHitSounds->addNewSound("10", "../assets/hurly-burly/audio/puppetDialog_Castle/castleHit4.ogg");
	resources.push_back(catapultHitSounds);
	
	catapultThrowSounds->addNewSound("1", "../assets/hurly-burly/audio/catapultAudio/throw1.ogg");
	resources.push_back(catapultThrowSounds);

	catapultWhistleSounds->addNewSound("1", "../assets/hurly-burly/audio/catapultAudio/whistle1.ogg");
	catapultWhistleSounds->addNewSound("15", "../assets/hurly-burly/audio/catapultAudio/whistle2.ogg");
	catapultWhistleSounds->addNewSound("2", "../assets/hurly-burly/audio/puppetDialog_Castle/fireBoulder1.ogg");
	catapultWhistleSounds->addNewSound("3", "../assets/hurly-burly/audio/puppetDialog_Castle/fireBoulder2.ogg");
	catapultWhistleSounds->addNewSound("4", "../assets/hurly-burly/audio/puppetDialog_Castle/fireBoulder3.ogg");
	catapultWhistleSounds->addNewSound("5", "../assets/hurly-burly/audio/puppetDialog_Castle/fireBoulder4.ogg");
	catapultWhistleSounds->addNewSound("6", "../assets/hurly-burly/audio/puppetDialog_Castle/fireBoulder5.ogg");
	catapultWhistleSounds->addNewSound("7", "../assets/hurly-burly/audio/puppetDialog_Castle/fireBoulder6.ogg");
	catapultWhistleSounds->addNewSound("8", "../assets/hurly-burly/audio/puppetDialog_Castle/fireBoulder7.ogg");
	catapultWhistleSounds->addNewSound("9", "../assets/hurly-burly/audio/puppetDialog_Castle/fireBoulder8.ogg");
	catapultWhistleSounds->addNewSound("10", "../assets/hurly-burly/audio/puppetDialog_Castle/fireBoulder9.ogg");
	catapultWhistleSounds->addNewSound("11", "../assets/hurly-burly/audio/puppetDialog_Castle/fireBoulder10.ogg");
	catapultWhistleSounds->addNewSound("12", "../assets/hurly-burly/audio/puppetDialog_Castle/fireBoulder11.ogg");
	catapultWhistleSounds->addNewSound("13", "../assets/hurly-burly/audio/puppetDialog_Castle/fireBoulder12.ogg");
	catapultWhistleSounds->addNewSound("14", "../assets/hurly-burly/audio/puppetDialog_Castle/fireBoulder13.ogg");
	resources.push_back(catapultWhistleSounds);

}