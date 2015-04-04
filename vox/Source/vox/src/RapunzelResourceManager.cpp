#pragma once

#include <RapunzelResourceManager.h>
#include <TextureSampler.h>

TextureSampler * RapunzelResourceManager::splashMessage	= new TextureSampler(new Texture("../assets/hurly-burly/SplashMessages/Rapunzel.png", 1024, 1024, true, true), 1024, 1024);

Texture * RapunzelResourceManager::goldSpriteSheet    = new Texture("../assets/hurly-burly/Gold/GoldSpritesheet.png", 1024, 1024, true, true);

TextureSampler * RapunzelResourceManager::towerBackground	= new TextureSampler("../assets/hurly-burly/RapCastleAssets/", "RapCastleBackground.png.def");
TextureSampler * RapunzelResourceManager::towerCatwalk		= new TextureSampler("../assets/hurly-burly/RapCastleAssets/", "RapCastleCatwalk.png.def");
TextureSampler * RapunzelResourceManager::towerTower		= new TextureSampler("../assets/hurly-burly/RapCastleAssets/", "RapCastleTower.png.def");

TextureSampler * RapunzelResourceManager::goldBrick   = new TextureSampler("../assets/hurly-burly/Gold/", "goldBrick.png.def");
TextureSampler * RapunzelResourceManager::glove   = new TextureSampler("../assets/hurly-burly/RapCastleAssets/", "RapCastleGlove.png.def");
TextureSampler * RapunzelResourceManager::spring   = new TextureSampler("../assets/hurly-burly/RapCastleAssets/", "RapCastleSpring.png.def");

TextureSampler * RapunzelResourceManager::leverHandle   = new TextureSampler(new Texture("../assets/hurly-burly/Lever/LeverHandle.png", 512, 512, true, true), 32, 101);
TextureSampler * RapunzelResourceManager::leverBase  = new TextureSampler(new Texture("../assets/hurly-burly/Lever/LeverBase.png", 512, 512, true, true), 104, 23);

TextureSampler * RapunzelResourceManager::hair  = new TextureSampler("../assets/hurly-burly/Hair/", "Hair.png.def");

TextureSampler * RapunzelResourceManager::hairLink  = new TextureSampler("../assets/hurly-burly/RapunzelAssets/", "RapunzelHairLink.png.def");
TextureSampler * RapunzelResourceManager::hairEnd  = new TextureSampler("../assets/hurly-burly/RapunzelAssets/", "RapunzelHairEnd.png.def");

TextureSampler * RapunzelResourceManager::thiefTorso	= new TextureSampler("../assets/hurly-burly/ThiefAssets/", "ThiefTorso.png.def");
TextureSampler * RapunzelResourceManager::thiefArm		= new TextureSampler("../assets/hurly-burly/ThiefAssets/", "ThiefArm.png.def");
TextureSampler * RapunzelResourceManager::thiefHelmet	= new TextureSampler("../assets/hurly-burly/ThiefAssets/", "ThiefHelmet.png.def");
										  
TextureSampler * RapunzelResourceManager::rapunzelTorso	    = new TextureSampler("../assets/hurly-burly/RapunzelAssets/", "RapunzelDress.png.def");
TextureSampler * RapunzelResourceManager::rapunzelArm	    = new TextureSampler("../assets/hurly-burly/RapunzelAssets/", "RapunzelArm.png.def");
TextureSampler * RapunzelResourceManager::rapunzelHelmet    = new TextureSampler("../assets/hurly-burly/RapunzelAssets/", "RapunzelHat.png.def");

TextureSampler * RapunzelResourceManager::itemSpear	= new TextureSampler("../assets/hurly-burly/WeaponAssets/", "Arrow.png.def");
TextureSampler * RapunzelResourceManager::itemAxe	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Axe.png", 1024, 1024, true, true), 230, 395);
TextureSampler * RapunzelResourceManager::itemClub	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Club.png", 1024, 1024, true, true), 83, 515);
TextureSampler * RapunzelResourceManager::itemMace	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Mace.png", 1024, 1024, true, true), 178, 516);
TextureSampler * RapunzelResourceManager::itemSword	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Sword.png", 1024, 1024, true, true), 81, 382);
TextureSampler * RapunzelResourceManager::itemScimitar	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Scimitar.png", 1024, 1024, true, true), 79, 401);

void RapunzelResourceManager::init(){

	resources.push_back(splashMessage);

	resources.push_back(leverHandle);
	resources.push_back(leverBase);

	resources.push_back(hair);
	resources.push_back(hairLink);
	resources.push_back(hairEnd);

	resources.push_back(goldSpriteSheet);
	resources.push_back(goldBrick);
	resources.push_back(glove);
	resources.push_back(spring);

	resources.push_back(towerBackground);
	resources.push_back(towerCatwalk);
	resources.push_back(towerTower);

	resources.push_back(thiefTorso);
	resources.push_back(thiefArm);
	resources.push_back(thiefHelmet);
	
	resources.push_back(rapunzelTorso);
	resources.push_back(rapunzelArm);
	resources.push_back(rapunzelHelmet);
	
	resources.push_back(itemSpear);
	resources.push_back(itemAxe);
	resources.push_back(itemClub);
	resources.push_back(itemMace);
	resources.push_back(itemSword);
	resources.push_back(itemScimitar);
}

TextureSampler * RapunzelResourceManager::getRandomWeapon(){
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