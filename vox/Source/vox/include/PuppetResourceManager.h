#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "RaidTheCastleResourceManager.h"

class PuppetResourceManager : public ResourceManager{
public:	
	static RaidTheCastleResourceManager * raidTheCastle;

	static Texture * stageFloor;
	static Texture * sky;
	static Texture * ground1;
	static TextureSampler * head1;
	static TextureSampler * face1;
	static TextureSampler * hand1;

	
	PuppetResourceManager();
};