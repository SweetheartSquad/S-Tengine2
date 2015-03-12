#pragma once

#include "PuppetResourceManager.h"
#include "Texture.h"
#include "Box2DSuperSprite.h"
#include "RaidTheCastleResourceManager.h"

RaidTheCastleResourceManager * PuppetResourceManager::raidTheCastle = new RaidTheCastleResourceManager(); 

Texture * PuppetResourceManager::stageFloor = new Texture("../assets/hurly-burly/StageFloor.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::sky		= new Texture("../assets/hurly-burly/Sky.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::ground1	= new Texture("../assets/paper.png", 512, 512, true, true);

TextureSampler * PuppetResourceManager::goldenBreastPlateStick = new TextureSampler(new Texture("../assets/hurly-burly/KnightAssets/BreastplateStick1.png", 512, 512, true, true),	145, 405); 

TextureSampler * PuppetResourceManager::head1	     = new TextureSampler(new Texture("../assets/hurly-burly/KnightAssets/Head1.png", 512, 512, true, true), 106, 111); 
TextureSampler * PuppetResourceManager::goldenArm    = new TextureSampler(new Texture("../assets/hurly-burly/KnightAssets/Arm1.png", 512, 512, true, true), 40, 105); 
TextureSampler * PuppetResourceManager::hand1	     = new TextureSampler(new Texture("../assets/hurly-burly/KnightAssets/Hand1.png", 512, 512, true, true), 38, 32);
TextureSampler * PuppetResourceManager::face1		 = new TextureSampler(new Texture("../assets/hurly-burly/KnightAssets/Face1.png", 512,512, true, true), 67, 72);
TextureSampler * PuppetResourceManager::goldenHelmet = new TextureSampler(new Texture("../assets/hurly-burly/KnightAssets/Helmet1.png", 512,512, true, true), 114, 165);

PuppetResourceManager::PuppetResourceManager(){
	resources.push_back(stageFloor);
	resources.push_back(sky);

	resources.push_back(goldenBreastPlateStick);
	resources.push_back(head1);
	resources.push_back(goldenArm);
	resources.push_back(hand1);
	resources.push_back(face1);
	resources.push_back(goldenHelmet);
	resources.push_back(ground1);

	//Not sure why I need to call the constructor here 
	raidTheCastle = new RaidTheCastleResourceManager(); 
	subManagers.push_back(raidTheCastle);
}
