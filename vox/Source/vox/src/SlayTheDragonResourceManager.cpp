#pragma once

#include <SlayTheDragonResourceManager.h>
#include <TextureSampler.h>
#include <SoundManager.h>

TextureSampler * SlayTheDragonResourceManager::splashMessage	= new TextureSampler(new Texture("../assets/hurly-burly/SplashMessages/SlayTheDragon.png", 1024, 1024, true, true), 1024, 1024);

TextureSampler * SlayTheDragonResourceManager::fortBackground		= new TextureSampler(new Texture("../assets/hurly-burly/Fortification/FortBackground.png", 1024, 1024, true, true), 453, 307);
TextureSampler * SlayTheDragonResourceManager::fortForeground		= new TextureSampler(new Texture("../assets/hurly-burly/Fortification/FortForeground.png", 1024, 1024, true, true), 1024, 413);
TextureSampler * SlayTheDragonResourceManager::fortStructure		= new TextureSampler(new Texture("../assets/hurly-burly/Fortification/FortStructure.png", 1024, 1024, true, true), 683, 127);
Texture * SlayTheDragonResourceManager::fortificationSpriteSheet	= new Texture("../assets/hurly-burly/Fortification/FortSpriteSheet.png", 1024, 1024, true, true);

TextureSampler * SlayTheDragonResourceManager::archerTorso		= new TextureSampler(new Texture("../assets/hurly-burly/ArcherAssets/ArcherTorso.png", 1024, 1024, true, true), 275, 765);
TextureSampler * SlayTheDragonResourceManager::archerArm		= new TextureSampler(new Texture("../assets/hurly-burly/ArcherAssets/ArcherArm.png", 1024, 1024, true, true), 64, 228);
TextureSampler * SlayTheDragonResourceManager::archerHat		= new TextureSampler(new Texture("../assets/hurly-burly/ArcherAssets/ArcherHat.png", 1024, 1024, true, true), 191, 275);

TextureSampler * SlayTheDragonResourceManager::dragonHead		= new TextureSampler("../assets/hurly-burly/DragonAssets/", "DragonHead.png.def");
TextureSampler * SlayTheDragonResourceManager::dragonMouth		= new TextureSampler("../assets/hurly-burly/DragonAssets/", "DragonMouth.png.def");
TextureSampler * SlayTheDragonResourceManager::dragonTorso		= new TextureSampler("../assets/hurly-burly/DragonAssets/", "DragonTorso.png.def");
TextureSampler * SlayTheDragonResourceManager::dragonUpperWing	= new TextureSampler("../assets/hurly-burly/DragonAssets/", "DragonUpperWing.png.def");
TextureSampler * SlayTheDragonResourceManager::dragonLowerWing	= new TextureSampler("../assets/hurly-burly/DragonAssets/", "DragonLowerWing.png.def");

TextureSampler * SlayTheDragonResourceManager::itemBow	= new TextureSampler("../assets/hurly-burly/WeaponAssets/", "Bow.png.def");
TextureSampler * SlayTheDragonResourceManager::itemArrow	= new TextureSampler("../assets/hurly-burly/WeaponAssets/", "Arrow.png.def");
TextureSampler * SlayTheDragonResourceManager::itemFireball	= new TextureSampler("../assets/hurly-burly/DragonAssets/", "Fireball.png.def");
TextureSampler * SlayTheDragonResourceManager::itemFireParticle	= new TextureSampler("../assets/hurly-burly/DragonAssets/", "FireParticle.png.def");

SoundManager * SlayTheDragonResourceManager::dragonSounds = new SoundManager(-1);
SoundManager * SlayTheDragonResourceManager::fireSounds = new SoundManager(-1);
SoundManager * SlayTheDragonResourceManager::miscSounds = new SoundManager(-1);

void SlayTheDragonResourceManager::init(){
	resources.push_back(splashMessage);
	
	resources.push_back(fortBackground);
	resources.push_back(fortForeground);
	resources.push_back(fortStructure);
	resources.push_back(fortificationSpriteSheet);

	resources.push_back(archerTorso);
	resources.push_back(archerArm);
	resources.push_back(archerHat);
	
	resources.push_back(dragonHead);
	resources.push_back(dragonMouth);
	resources.push_back(dragonTorso);
	resources.push_back(dragonUpperWing);
	resources.push_back(dragonLowerWing);

	resources.push_back(itemBow);
	resources.push_back(itemArrow);
	resources.push_back(itemFireball);
	resources.push_back(itemFireParticle);

	
	dragonSounds->addNewSound("1", "../assets/hurly-burly/audio/dragon/dragonSound1.ogg");
	dragonSounds->addNewSound("2", "../assets/hurly-burly/audio/dragon/dragonSound2.ogg");
	dragonSounds->addNewSound("3", "../assets/hurly-burly/audio/dragon/dragonSound3.ogg");
	dragonSounds->addNewSound("4", "../assets/hurly-burly/audio/dragon/dragonSound4.ogg");
	dragonSounds->addNewSound("5", "../assets/hurly-burly/audio/dragon/dragonSound5.ogg");
	dragonSounds->addNewSound("6", "../assets/hurly-burly/audio/dragon/dragonSound6.ogg");
	dragonSounds->addNewSound("7", "../assets/hurly-burly/audio/dragon/dragonSound7.ogg");
	dragonSounds->addNewSound("8", "../assets/hurly-burly/audio/dragon/dragonSound8.ogg");
	dragonSounds->addNewSound("9", "../assets/hurly-burly/audio/dragon/dragonSound9.ogg");
	dragonSounds->addNewSound("10", "../assets/hurly-burly/audio/dragon/dragonSound10.ogg");
	dragonSounds->addNewSound("11", "../assets/hurly-burly/audio/dragon/dragonSound11.ogg");
	dragonSounds->addNewSound("12", "../assets/hurly-burly/audio/dragon/dragonSound12.ogg");
	dragonSounds->addNewSound("13", "../assets/hurly-burly/audio/dragon/dragonSound13.ogg");
	dragonSounds->addNewSound("14", "../assets/hurly-burly/audio/dragon/dragonSound14.ogg");
	dragonSounds->addNewSound("15", "../assets/hurly-burly/audio/puppetDialog_Dragon/fire1.ogg");
	dragonSounds->addNewSound("16", "../assets/hurly-burly/audio/puppetDialog_Dragon/lookOut1.ogg");
	dragonSounds->addNewSound("17", "../assets/hurly-burly/audio/puppetDialog_Dragon/lookOut2.ogg");
	dragonSounds->addNewSound("18", "../assets/hurly-burly/audio/puppetDialog_Dragon/ohNo1.ogg");
	dragonSounds->addNewSound("19", "../assets/hurly-burly/audio/puppetDialog_Dragon/ohNo2.ogg");
	dragonSounds->addNewSound("20", "../assets/hurly-burly/audio/puppetDialog_Dragon/ohNo3.ogg");
	resources.push_back(dragonSounds);

	fireSounds->addNewSound("1", "../assets/hurly-burly/audio/puppetDialog_Dragon/onFire1.ogg");
	fireSounds->addNewSound("2", "../assets/hurly-burly/audio/puppetDialog_Dragon/onFire2.ogg");
	fireSounds->addNewSound("3", "../assets/hurly-burly/audio/puppetDialog_Dragon/onFire3.ogg");
	fireSounds->addNewSound("4", "../assets/hurly-burly/audio/puppetDialog_Dragon/onFire4.ogg");
	fireSounds->addNewSound("5", "../assets/hurly-burly/audio/puppetDialog_Dragon/onFire5.ogg");
	fireSounds->addNewSound("6", "../assets/hurly-burly/audio/puppetDialog_Dragon/onFire6.ogg");
	resources.push_back(fireSounds);

	miscSounds->addNewSound("damaged", "../assets/hurly-burly/audio/puppetDialog_Dragon/castleAboutToBreak.ogg");
	miscSounds->addNewSound("broken", "../assets/hurly-burly/audio/puppetDialog_Dragon/castleBroken.ogg");
	resources.push_back(miscSounds);
}
