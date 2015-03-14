#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "RaidTheCastleResourceManager.h"

class PuppetResourceManager abstract : public ResourceManager{
public:	
	static RaidTheCastleResourceManager * raidTheCastle;

	static Texture * stageFloor;
	static Texture * sky;
	static Texture * ground1;
	static Texture * PuppetResourceManager::tree1; 
	static Texture * PuppetResourceManager::tree2;	
	static Texture * PuppetResourceManager::bush1;
	static Texture * PuppetResourceManager::bush2;

	static TextureSampler * head1;
	static TextureSampler * face1;
	static TextureSampler * hand1;

	static TextureSampler * countDown1;
	static TextureSampler * countDown2;
	static TextureSampler * countDown3;
	static TextureSampler * countDown4;
	static TextureSampler * countDown5;

	
	static void init();
};