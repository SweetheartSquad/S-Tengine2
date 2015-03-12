#pragma once

#include "PuppetResourceManager.h"
#include "Texture.h"
#include "Box2DSuperSprite.h"
#include "RaidTheCastleResourceManager.h"

RaidTheCastleResourceManager * PuppetResourceManager::raidTheCastle = new RaidTheCastleResourceManager(); 

Texture * PuppetResourceManager::stageFloor = new Texture("../assets/hurly-burly/StageFloor.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::sky		= new Texture("../assets/hurly-burly/Sky.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::ground1	= new Texture("../assets/paper.png", 512, 512, true, true);

TextureSampler * PuppetResourceManager::head1 = new TextureSampler(new Texture("../assets/hurly-burly/Head1.png", 512, 512, true, true), 106, 111); 
TextureSampler * PuppetResourceManager::face1 = new TextureSampler(new Texture("../assets/hurly-burly/Face1.png", 512,512, true, true), 67, 72);
TextureSampler * PuppetResourceManager::hand1 = new TextureSampler(new Texture("../assets/hurly-burly/Hand1.png", 512, 512, true, true), 38, 32);

TextureSampler * PuppetResourceManager::countDown1Tex = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/1.png", 512, 512, true, true), 200, 200);
TextureSampler * PuppetResourceManager::countDown2Tex = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/2.png", 512, 512, true, true), 200, 200);
TextureSampler * PuppetResourceManager::countDown3Tex = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/3.png", 512, 512, true, true), 200, 200);
TextureSampler * PuppetResourceManager::countDown4Tex = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/4.png", 512, 512, true, true), 200, 200);
TextureSampler * PuppetResourceManager::countDown5Tex = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/5.png", 512, 512, true, true), 200, 200);

PuppetResourceManager::PuppetResourceManager(){
	resources.push_back(stageFloor);
	resources.push_back(sky);
	resources.push_back(ground1);

	resources.push_back(head1);
	resources.push_back(face1);
	resources.push_back(hand1);

	//Not sure why I need to call the constructor here 
	raidTheCastle = new RaidTheCastleResourceManager(); 
	subManagers.push_back(raidTheCastle);
}
