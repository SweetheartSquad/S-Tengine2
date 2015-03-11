#pragma once

#include "PuppetResourceManager.h"
#include "Texture.h"
#include "RaidTheCastleResourceManager.h"

RaidTheCastleResourceManager * PuppetResourceManager::raidTheCastle = new RaidTheCastleResourceManager(); 

Texture * PuppetResourceManager::stageFloor = new Texture("../assets/hurly-burly/StageFloor.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::sky		= new Texture("../assets/hurly-burly/Sky.png", 1024, 1024, true, true);

PuppetResourceManager::PuppetResourceManager(){
	resources.push_back(stageFloor);
	resources.push_back(sky);

	subManagers.push_back(raidTheCastle);
}