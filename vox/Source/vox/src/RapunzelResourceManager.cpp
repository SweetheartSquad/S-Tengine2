#pragma once

#include <RapunzelResourceManager .h>

TextureSampler * RapunzelResourceManager::splashMessage	= new TextureSampler(new Texture("../assets/hurly-burly/SplashMessages/Rapunzel.png", 1024, 1024, true, true), 300, 100);

Texture * RapunzelResourceManager::goldSpriteSheet    = new Texture("../assets/structure components/gold/gold_SpriteSheet.png", 2048, 2048, true, true);

TextureSampler * RapunzelResourceManager::leverHandle   = new TextureSampler(new Texture("../assets/structure components/lever/LeverHandle.png", 512, 512, true, true), 429, 76);;
TextureSampler * RapunzelResourceManager::leverBase  = new TextureSampler(new Texture("../assets/structure components/lever/LeverBase.png", 512, 512, true, true), 418, 264);

TextureSampler * RapunzelResourceManager::hair  = new TextureSampler(new Texture("../assets/structure components/hair/Hair.png", 512, 512, true, true), 418, 264);

/*
TextureSampler * RapunzelResourceManager::knightRedTorso	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/redKnightBreastplate.png", 1024, 1024, true, true), 268, 776);
TextureSampler * RapunzelResourceManager::knightRedArm		= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/redKnightArm.png", 1024, 1024, true, true), 92, 327);
TextureSampler * RapunzelResourceManager::knightRedHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/redKnightHelmet.png", 1024, 1024, true, true), 233, 254);

TextureSampler * RapunzelResourceManager::knightGreenTorso	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/greenKnightBreastplate.png", 1024, 1024, true, true), 268, 776);
TextureSampler * RapunzelResourceManager::knightGreenArm		= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/greenKnightArm.png", 1024, 1024, true, true), 92, 327);
TextureSampler * RapunzelResourceManager::knightGreenHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/greenKnightHelmet.png", 1024, 1024, true, true), 233, 254);

TextureSampler * RapunzelResourceManager::knightBlueTorso	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/blueKnightBreastplate.png", 1024, 1024, true, true), 268, 776);
TextureSampler * RapunzelResourceManager::knightBlueArm		= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/blueKnightArm.png", 1024, 1024, true, true), 92, 327);
TextureSampler * RapunzelResourceManager::knightBlueHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/blueKnightHelmet.png", 1024, 1024, true, true), 233, 254);

TextureSampler * RapunzelResourceManager::knightYellowTorso	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/yellowKnightBreastplate.png", 1024, 1024, true, true), 268, 776);
TextureSampler * RapunzelResourceManager::knightYellowArm		= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/yellowKnightArm.png", 1024, 1024, true, true), 92, 327);
TextureSampler * RapunzelResourceManager::knightYellowHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/NormalKnightAssets/yellowKnightHelmet.png", 1024, 1024, true, true), 233, 254);

TextureSampler * RapunzelResourceManager::castleChampionTorso	= new TextureSampler(new Texture("../assets/hurly-burly/CastleChampionAssets/CCBody.png", 1024, 1024, true, true), 275, 806);
TextureSampler * RapunzelResourceManager::castleChampionArm	= new TextureSampler(new Texture("../assets/hurly-burly/CastleChampionAssets/CCArm.png", 1024, 1024, true, true), 67, 200);
TextureSampler * RapunzelResourceManager::castleChampionHelmet	= new TextureSampler(new Texture("../assets/hurly-burly/CastleChampionAssets/CCHelmet.png", 1024, 1024, true, true), 229, 295);
*/

TextureSampler * RapunzelResourceManager::itemAxe	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Axe.png", 1024, 1024, true, true), 230, 395);
TextureSampler * RapunzelResourceManager::itemClub	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Club.png", 1024, 1024, true, true), 83, 515);
TextureSampler * RapunzelResourceManager::itemMace	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Mace.png", 1024, 1024, true, true), 178, 516);
TextureSampler * RapunzelResourceManager::itemSword	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Sword.png", 1024, 1024, true, true), 81, 382);
TextureSampler * RapunzelResourceManager::itemScimitar	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Scimitar.png", 1024, 1024, true, true), 79, 401);

/*
TextureSampler * RapunzelResourceManager::goldenBreastPlateStick = new TextureSampler(new Texture("../assets/hurly-burly/KnightAssets/BreastplateStick1.png", 512, 512, true, true),	145, 405); 
TextureSampler * RapunzelResourceManager::goldenArm     = new TextureSampler(new Texture("../assets/hurly-burly/KnightAssets/Arm1.png", 512, 512, true, true), 40, 105); 
TextureSampler * RapunzelResourceManager::goldenHelmet  = new TextureSampler(new Texture("../assets/hurly-burly/KnightAssets/Helmet1.png", 512,512, true, true), 114, 165);
*/
void RapunzelResourceManager::init(){

	resources.push_back(splashMessage);

	resources.push_back(leverHandle);
	resources.push_back(leverBase);

	/*resources.push_back(goldenArm);
	resources.push_back(hand1);
	resources.push_back(face1);
	resources.push_back(goldenHelmet);*/
	/*
	resources.push_back(knightRedTorso);
	resources.push_back(knightRedArm);
	resources.push_back(knightRedHelmet);
	
	resources.push_back(knightGreenTorso);
	resources.push_back(knightGreenArm);
	resources.push_back(knightGreenHelmet);
	
	resources.push_back(knightBlueTorso);
	resources.push_back(knightBlueArm);
	resources.push_back(knightBlueHelmet);
	
	resources.push_back(knightYellowTorso);
	resources.push_back(knightYellowArm);
	resources.push_back(knightYellowHelmet);
	
	resources.push_back(castleChampionTorso);
	resources.push_back(castleChampionArm);
	resources.push_back(castleChampionHelmet);
	*/
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