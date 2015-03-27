#pragma once

#include <RapunzelResourceManager.h>
#include <TextureSampler.h>

TextureSampler * RapunzelResourceManager::splashMessage	= new TextureSampler(new Texture("../assets/hurly-burly/SplashMessages/Rapunzel.png", 1024, 1024, true, true), 1024, 1024);

Texture * RapunzelResourceManager::goldSpriteSheet    = new Texture("../assets/hurly-burly/Gold/GoldSpritesheet.png", 1024, 1024, true, true);

TextureSampler * RapunzelResourceManager::leverHandle   = new TextureSampler(new Texture("../assets/hurly-burly/Lever/LeverHandle.png", 512, 512, true, true), 32, 101);
TextureSampler * RapunzelResourceManager::leverBase  = new TextureSampler(new Texture("../assets/hurly-burly/Lever/LeverBase.png", 512, 512, true, true), 104, 23);

TextureSampler * RapunzelResourceManager::hair  = new TextureSampler("../assets/hurly-burly/Hair/", "Hair.png.def");

TextureSampler * RapunzelResourceManager::thiefTorso	= new TextureSampler("../assets/hurly-burly/ThiefAssets/", "ThiefTorso.png.def");
TextureSampler * RapunzelResourceManager::thiefArm		= new TextureSampler("../assets/hurly-burly/ThiefAssets/", "ThiefArm.png.def");
TextureSampler * RapunzelResourceManager::thiefHelmet	= new TextureSampler("../assets/hurly-burly/ThiefAssets/", "ThiefHelmet.png.def");
										  
TextureSampler * RapunzelResourceManager::rapunzelRedTorso	= new TextureSampler(new Texture("../assets/hurly-burly/RapunzelAssets/CCBody.png", 1024, 1024, true, true), 275, 806);
TextureSampler * RapunzelResourceManager::rapunzelRedArm	= new TextureSampler(new Texture("../assets/hurly-burly/RapunzelAssets/CCArm.png", 1024, 1024, true, true), 67, 200);
TextureSampler * RapunzelResourceManager::rapunzelRedHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/RapunzelAssets/CCHelmet.png", 1024, 1024, true, true), 229, 295);

TextureSampler * RapunzelResourceManager::rapunzelGreenTorso	= new TextureSampler(new Texture("../assets/hurly-burly/RapunzelAssets/CCBody.png", 1024, 1024, true, true), 275, 806);
TextureSampler * RapunzelResourceManager::rapunzelGreenArm	= new TextureSampler(new Texture("../assets/hurly-burly/RapunzelAssets/CCArm.png", 1024, 1024, true, true), 67, 200);
TextureSampler * RapunzelResourceManager::rapunzelGreenHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/RapunzelAssets/CCHelmet.png", 1024, 1024, true, true), 229, 295);

TextureSampler * RapunzelResourceManager::rapunzelBlueTorso	= new TextureSampler(new Texture("../assets/hurly-burly/RapunzelAssets/CCBody.png", 1024, 1024, true, true), 275, 806);
TextureSampler * RapunzelResourceManager::rapunzelBlueArm	= new TextureSampler(new Texture("../assets/hurly-burly/RapunzelAssets/CCArm.png", 1024, 1024, true, true), 67, 200);
TextureSampler * RapunzelResourceManager::rapunzelBlueHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/RapunzelAssets/CCHelmet.png", 1024, 1024, true, true), 229, 295);

TextureSampler * RapunzelResourceManager::rapunzelYellowTorso	= new TextureSampler(new Texture("../assets/hurly-burly/RapunzelAssets/CCBody.png", 1024, 1024, true, true), 275, 806);
TextureSampler * RapunzelResourceManager::rapunzelYellowArm	= new TextureSampler(new Texture("../assets/hurly-burly/RapunzelAssets/CCArm.png", 1024, 1024, true, true), 67, 200);
TextureSampler * RapunzelResourceManager::rapunzelYellowHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/RapunzelAssets/CCHelmet.png", 1024, 1024, true, true), 229, 295);

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
	resources.push_back(goldSpriteSheet);

	/*resources.push_back(goldenArm);
	resources.push_back(hand1);
	resources.push_back(face1);
	resources.push_back(goldenHelmet);*/
	
	resources.push_back(thiefTorso);
	resources.push_back(thiefArm);
	resources.push_back(thiefHelmet);
	
	resources.push_back(rapunzelRedTorso);
	resources.push_back(rapunzelRedArm);
	resources.push_back(rapunzelRedHelmet);
						
	resources.push_back(rapunzelGreenTorso);
	resources.push_back(rapunzelGreenArm);
	resources.push_back(rapunzelGreenHelmet);
						
	resources.push_back(rapunzelBlueTorso);
	resources.push_back(rapunzelBlueArm);
	resources.push_back(rapunzelBlueHelmet);
						
	resources.push_back(rapunzelYellowTorso);
	resources.push_back(rapunzelYellowArm);
	resources.push_back(rapunzelYellowHelmet);
	
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