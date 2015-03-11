#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "RaidTheCastleResourceManager.h"

class PuppetResourceManager : public ResourceManager{
public:	
	static RaidTheCastleResourceManager * raidTheCastle;

	static Texture * stageFloor;
	static Texture * sky;
	
	PuppetResourceManager();
};