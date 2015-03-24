#pragma once

#include <PuppetResourceManager.h>
#include <Texture.h>
#include <Box2DSuperSprite.h>
#include <RaidTheCastleResourceManager.h>
#include <RapunzelResourceManager.h>
#include <SlayTheDragonResourceManager.h>
#include <SoundManager.h>
#include <TextureSampler.h>

Texture * PuppetResourceManager::blank = new Texture("../assets/hurly-burly/blank.png", 1, 1, true, true);
TextureSampler * PuppetResourceManager::itemNone = new TextureSampler(new Texture("../assets/hurly-burly/blank.png", 1, 1, true, true), 1, 1);

Texture * PuppetResourceManager::stageFloor = new Texture("../assets/hurly-burly/StageFloor.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::sky		= new Texture("../assets/hurly-burly/Sky.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::ground1	= new Texture("../assets/hurly-burly/paper.png", 512, 512, true, true);

Texture * PuppetResourceManager::tree1  = new Texture("../assets/hurly-burly/Foliage/Tree1.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::tree2	= new Texture("../assets/hurly-burly/Foliage/Tree2.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::bush1	= new Texture("../assets/hurly-burly/Foliage/Bush1.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::bush2	= new Texture("../assets/hurly-burly/Foliage/Bush2.png", 1024, 1024, true, true);

Texture * PuppetResourceManager::cloud1	= new Texture("../assets/hurly-burly/Clouds/Cloud1.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::cloud2	= new Texture("../assets/hurly-burly/Clouds/Cloud2.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::cloud3	= new Texture("../assets/hurly-burly/Clouds/Cloud3.png", 1024, 1024, true, true);
Texture * PuppetResourceManager::cloud4	= new Texture("../assets/hurly-burly/Clouds/Cloud4.png", 1024, 1024, true, true);

TextureSampler * PuppetResourceManager::dustParticle = new TextureSampler(new Texture("../assets/hurly-burly/dustParticle.png", 512, 512, true, true), 333, 291);

//TextureSampler * PuppetResourceManager::head1 = new TextureSampler(new Texture("../assets/hurly-burly/Head1.png", 512, 512, true, true), 212, 222); 
TextureSampler * PuppetResourceManager::head1 = new TextureSampler(new Texture("../assets/hurly-burly/Head1.png", 512, 512, true, true), 212, 222); 
TextureSampler * PuppetResourceManager::face1 = new TextureSampler(new Texture("../assets/hurly-burly/Face1.png", 512,512, true, true), 134, 144);
TextureSampler * PuppetResourceManager::hand1 = new TextureSampler(new Texture("../assets/hurly-burly/Hand1.png", 512, 512, true, true), 76, 64);

TextureSampler * PuppetResourceManager::countDown0 = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/0.png", 1024, 1024, true, true), 1024, 1024);
TextureSampler * PuppetResourceManager::countDown1 = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/1.png", 1024, 1024, true, true), 1024, 1024);
TextureSampler * PuppetResourceManager::countDown2 = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/2.png", 1024, 1024, true, true), 1024, 1024);
TextureSampler * PuppetResourceManager::countDown3 = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/3.png", 1024, 1024, true, true), 1024, 1024);
TextureSampler * PuppetResourceManager::countDown4 = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/4.png", 1024, 1024, true, true), 1024, 1024);
TextureSampler * PuppetResourceManager::countDown5 = new TextureSampler(new Texture("../assets/hurly-burly/Countdown/5.png", 1024, 1024, true, true), 1024, 1024);

TextureSampler * PuppetResourceManager::redWins = new TextureSampler(new Texture("../assets/hurly-burly/VictorySplashMessages/redWins.png", 1024, 1024, true, true), 1024, 1024);
TextureSampler * PuppetResourceManager::yellowWins = new TextureSampler(new Texture("../assets/hurly-burly/VictorySplashMessages/yellowWins.png", 1024, 1024, true, true), 1024, 1024);
TextureSampler * PuppetResourceManager::greenWins = new TextureSampler(new Texture("../assets/hurly-burly/VictorySplashMessages/greenWins.png", 1024, 1024, true, true), 1024, 1024);
TextureSampler * PuppetResourceManager::blueWins = new TextureSampler(new Texture("../assets/hurly-burly/VictorySplashMessages/blueWins.png", 1024, 1024, true, true), 1024, 1024);

SoundManager * PuppetResourceManager::jumpSounds   = new SoundManager(1.0);
SoundManager * PuppetResourceManager::hitSounds    = new SoundManager(1.0);
SoundManager * PuppetResourceManager::splashSounds = new SoundManager(-1);

void PuppetResourceManager::init(){
	RaidTheCastleResourceManager::init();
	RapunzelResourceManager::init();
	SlayTheDragonResourceManager::init();

	resources.push_back(blank);
	resources.push_back(itemNone);
	resources.push_back(stageFloor);
	resources.push_back(sky);
	resources.push_back(ground1);
	resources.push_back(tree1);
	resources.push_back(tree2);
	resources.push_back(bush1);
	resources.push_back(bush2);

	resources.push_back(dustParticle);

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
	
	resources.push_back(redWins);
	resources.push_back(yellowWins);
	resources.push_back(greenWins);
	resources.push_back(blueWins);

	resources.push_back(splashSounds);
	resources.push_back(hitSounds);
	resources.push_back(jumpSounds);

	jumpSounds->addNewSound("jump1", "../assets/hurly-burly/audio/jump/jumpSound1.ogg");
	jumpSounds->addNewSound("jump2", "../assets/hurly-burly/audio/jump/jumpSound2.ogg");
	jumpSounds->addNewSound("jump3", "../assets/hurly-burly/audio/jump/jumpSound3.ogg");
	jumpSounds->addNewSound("jump4", "../assets/hurly-burly/audio/jump/jumpSound4.ogg");
	jumpSounds->addNewSound("jump5", "../assets/hurly-burly/audio/jump/jumpSound5.ogg");
	jumpSounds->addNewSound("jump6", "../assets/hurly-burly/audio/jump/jumpSound6.ogg");
	jumpSounds->addNewSound("jump7", "../assets/hurly-burly/audio/jump/jumpSound7.ogg");
	jumpSounds->addNewSound("jump8", "../assets/hurly-burly/audio/jump/jumpSound8.ogg");
	jumpSounds->addNewSound("jump9", "../assets/hurly-burly/audio/jump/jumpSound9.ogg");
	jumpSounds->addNewSound("jump10", "../assets/hurly-burly/audio/jump/jumpSound10.ogg");
	
	hitSounds->addNewSound("hit1", "../assets/hurly-burly/audio/hit/hitSound1.ogg");
	hitSounds->addNewSound("hit2", "../assets/hurly-burly/audio/hit/hitSound2.ogg");
	hitSounds->addNewSound("hit3", "../assets/hurly-burly/audio/hit/hitSound3.ogg");
	hitSounds->addNewSound("hit4", "../assets/hurly-burly/audio/hit/hitSound4.ogg");
	hitSounds->addNewSound("hit5", "../assets/hurly-burly/audio/hit/hitSound5.ogg");
	hitSounds->addNewSound("hit6", "../assets/hurly-burly/audio/hit/hitSound6.ogg");
	hitSounds->addNewSound("hit7", "../assets/hurly-burly/audio/hit/hitSound7.ogg");
	hitSounds->addNewSound("hit8", "../assets/hurly-burly/audio/hit/hitSound8.ogg");
	hitSounds->addNewSound("hit9", "../assets/hurly-burly/audio/hit/hitSound9.ogg");
	hitSounds->addNewSound("hit10", "../assets/hurly-burly/audio/hit/hitSound10.ogg");
	hitSounds->addNewSound("hit11", "../assets/hurly-burly/audio/hit/hitSound11.ogg");
	hitSounds->addNewSound("hit12", "../assets/hurly-burly/audio/hit/hitSound12.ogg");
	hitSounds->addNewSound("hit13", "../assets/hurly-burly/audio/hit/hitSound13.ogg");
	hitSounds->addNewSound("hit14", "../assets/hurly-burly/audio/hit/hitSound14.ogg");
	hitSounds->addNewSound("hit15", "../assets/hurly-burly/audio/hit/hitSound15.ogg");
	hitSounds->addNewSound("hit16", "../assets/hurly-burly/audio/hit/hitSound16.ogg");
	hitSounds->addNewSound("hit17", "../assets/hurly-burly/audio/hit/hitSound17.ogg");
	hitSounds->addNewSound("hit18", "../assets/hurly-burly/audio/hit/hitSound18.ogg");
	hitSounds->addNewSound("hit19", "../assets/hurly-burly/audio/hit/hitSound19.ogg");
	hitSounds->addNewSound("hit10", "../assets/hurly-burly/audio/hit/hitSound20.ogg");

	splashSounds->addNewSound("FightYourFriends", "../assets/hurly-burly/audio/splash/FightYourFriends.ogg");
	splashSounds->addNewSound("Joust", "../assets/hurly-burly/audio/splash/Joust.ogg");
	splashSounds->addNewSound("RaidTheCastle", "../assets/hurly-burly/audio/splash/RaidTheCastle.ogg");
	splashSounds->addNewSound("Rapunzel", "../assets/hurly-burly/audio/splash/Rapunzel.ogg");
	splashSounds->addNewSound("SlayTheDragon", "../assets/hurly-burly/audio/splash/SlayTheDragon.ogg");
}
