#pragma once

#include <PuppetResourceManager.h>
#include <Texture.h>
#include <Box2DSuperSprite.h>
#include <RaidTheCastleResourceManager.h>
#include <RapunzelResourceManager.h>
#include <SlayTheDragonResourceManager.h>
#include <SoundManager.h>

Texture * PuppetResourceManager::blank = new Texture("../assets/hurly-burly/blank.png", 1, 1, true, true);
Texture * PuppetResourceManager::stageFloor = new Texture("../assets/hurly-burly/StageFloor.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::sky		= new Texture("../assets/hurly-burly/Sky.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::ground1	= new Texture("../assets/hurly-burly/paper.png", 512, 512, true, true);
Texture * PuppetResourceManager::tree1  = new Texture("../assets/hurly-burly/Foliage/Tree1.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::tree2	= new Texture("../assets/hurly-burly/Foliage/Tree2.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::bush1	= new Texture("../assets/hurly-burly/Foliage/Bush1.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::bush2	= new Texture("../assets/hurly-burly/Foliage/Bush2.png", 1024, 1024, true, true);

TextureSampler * PuppetResourceManager::head1 = new TextureSampler(new Texture("../assets/hurly-burly/Head1.png", 512, 512, true, true), 212, 222); 
TextureSampler * PuppetResourceManager::face1 = new TextureSampler(new Texture("../assets/hurly-burly/Face1.png", 512,512, true, true), 134, 144);
TextureSampler * PuppetResourceManager::hand1 = new TextureSampler(new Texture("../assets/hurly-burly/Hand1.png", 512, 512, true, true), 76, 64);

TextureSampler * PuppetResourceManager::countDown0 = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/0.png", 1024, 1024, true, true), 1024, 1024);
TextureSampler * PuppetResourceManager::countDown1 = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/1.png", 1024, 1024, true, true), 1024, 1024);
TextureSampler * PuppetResourceManager::countDown2 = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/2.png", 1024, 1024, true, true), 1024, 1024);
TextureSampler * PuppetResourceManager::countDown3 = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/3.png", 1024, 1024, true, true), 1024, 1024);
TextureSampler * PuppetResourceManager::countDown4 = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/4.png", 1024, 1024, true, true), 1024, 1024);
TextureSampler * PuppetResourceManager::countDown5 = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/5.png", 1024, 1024, true, true), 1024, 1024);

SoundManager * PuppetResourceManager::jumpSounds = new SoundManager();
SoundManager * PuppetResourceManager::hitSounds = new SoundManager();
SoundManager * PuppetResourceManager::attackSounds = new SoundManager();

void PuppetResourceManager::init(){
	RaidTheCastleResourceManager::init();
	RapunzelResourceManager::init();
	SlayTheDragonResourceManager::init();

	resources.push_back(blank);
	resources.push_back(stageFloor);
	resources.push_back(sky);
	resources.push_back(ground1);
	resources.push_back(tree1);
	resources.push_back(tree2);
	resources.push_back(bush1);
	resources.push_back(bush2);

	resources.push_back(head1);
	resources.push_back(face1);
	resources.push_back(hand1);
	
	resources.push_back(countDown0);
	resources.push_back(countDown1);
	resources.push_back(countDown2);
	resources.push_back(countDown3);
	resources.push_back(countDown4);
	resources.push_back(countDown5);
	//Not sure why I need to call the constructor here 
	//raidTheCastle = new RaidTheCastleResourceManager(); 
	//subManagers.push_back(raidTheCastle);

	resources.push_back(attackSounds);
	resources.push_back(hitSounds);
	resources.push_back(jumpSounds);
}
