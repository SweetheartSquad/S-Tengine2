#pragma once

#include <RapunzelResourceManager.h>
#include <TextureSampler.h>

TextureSampler * RapunzelResourceManager::splashMessage	= new TextureSampler(new Texture("../assets/hurly-burly/SplashMessages/Rapunzel.png", 1024, 1024, true, true), 1024, 1024);

Texture * RapunzelResourceManager::goldSpriteSheet    = new Texture("../assets/structure components/gold/gold_SpriteSheet.png", 2048, 2048, true, true);

TextureSampler * RapunzelResourceManager::leverHandle   = new TextureSampler(new Texture("../assets/hurly-burly/Lever/LeverHandle.png", 512, 512, true, true), 32, 101);
TextureSampler * RapunzelResourceManager::leverBase  = new TextureSampler(new Texture("../assets/hurly-burly/Lever/LeverBase.png", 512, 512, true, true), 104, 23);

TextureSampler * RapunzelResourceManager::hair  = new TextureSampler(new Texture("../assets/hurly-burly/Hair/Hair.png", 1024, 1024, true, true), 233, 475);


TextureSampler * RapunzelResourceManager::thiefRedTorso		= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/redKnightBreastplate.png", 1024, 1024, true, true), 268, 776);
TextureSampler * RapunzelResourceManager::thiefRedArm		= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/redKnightArm.png", 1024, 1024, true, true), 92, 327);
TextureSampler * RapunzelResourceManager::thiefRedHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/redKnightHelmet.png", 1024, 1024, true, true), 233, 254);
										  
TextureSampler * RapunzelResourceManager::thiefGreenTorso	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/greenKnightBreastplate.png", 1024, 1024, true, true), 268, 776);
TextureSampler * RapunzelResourceManager::thiefGreenArm		= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/greenKnightArm.png", 1024, 1024, true, true), 92, 327);
TextureSampler * RapunzelResourceManager::thiefGreenHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/greenKnightHelmet.png", 1024, 1024, true, true), 233, 254);
										  
TextureSampler * RapunzelResourceManager::thiefBlueTorso	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/blueKnightBreastplate.png", 1024, 1024, true, true), 268, 776);
TextureSampler * RapunzelResourceManager::thiefBlueArm		= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/blueKnightArm.png", 1024, 1024, true, true), 92, 327);
TextureSampler * RapunzelResourceManager::thiefBlueHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/blueKnightHelmet.png", 1024, 1024, true, true), 233, 254);
										  
TextureSampler * RapunzelResourceManager::thiefYellowTorso	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/yellowKnightBreastplate.png", 1024, 1024, true, true), 268, 776);
TextureSampler * RapunzelResourceManager::thiefYellowArm	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/yellowKnightArm.png", 1024, 1024, true, true), 92, 327);
TextureSampler * RapunzelResourceManager::thiefYellowHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/yellowKnightHelmet.png", 1024, 1024, true, true), 233, 254);

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

	/*resources.push_back(goldenArm);
	resources.push_back(hand1);
	resources.push_back(face1);
	resources.push_back(goldenHelmet);*/
	
	resources.push_back(thiefRedTorso);
	resources.push_back(thiefRedArm);
	resources.push_back(thiefRedHelmet);
						
	resources.push_back(thiefGreenTorso);
	resources.push_back(thiefGreenArm);
	resources.push_back(thiefGreenHelmet);
						
	resources.push_back(thiefBlueTorso);
	resources.push_back(thiefBlueArm);
	resources.push_back(thiefBlueHelmet);
						
	resources.push_back(thiefYellowTorso);
	resources.push_back(thiefYellowArm);
	resources.push_back(thiefYellowHelmet);
	
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