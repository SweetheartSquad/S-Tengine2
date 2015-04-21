#pragma once

#include <LD32_ResourceManager.h>

Texture * LD32_ResourceManager::startSplash = new Texture("../assets/startSplash.png", 1024, 1024, true, false);
Texture * LD32_ResourceManager::endSplash = new Texture("../assets/endSplash.png", 1024, 1024, true, false);
Texture * LD32_ResourceManager::endSplashBad = new Texture("../assets/endSplashBad.png", 1024, 1024, true, false);
SoundManager * LD32_ResourceManager::music = new SoundManager(-1);
SoundManager * LD32_ResourceManager::sfx = new SoundManager(-1);
SoundManager * LD32_ResourceManager::bumperSfx = new SoundManager(-1);
SoundManager * LD32_ResourceManager::enemySfx = new SoundManager(-1);
SoundManager * LD32_ResourceManager::miscSounds = new SoundManager(-1);
Texture * LD32_ResourceManager::donutTop = new Texture("../assets/donutTop.png", 1024, 1024, true, false);
Texture * LD32_ResourceManager::donutBot = new Texture("../assets/donutBot.png", 1024, 1024, true, false);

void LD32_ResourceManager::init(){
	resources.push_back(startSplash);
	resources.push_back(endSplash);
	resources.push_back(endSplashBad);

	music->addNewSound("bgm", "../assets/Birdy-baseloop.ogg");
	music->addNewSound("bgm2", "../assets/Birdy-birds.ogg");
	music->addNewSound("bgm3", "../assets/Birdy-crash.ogg");
	music->addNewSound("bgm4", "../assets/Birdy-synth.ogg");
	resources.push_back(music);
	
	sfx->addNewSound("5", "../assets/SF5.ogg");
	sfx->addNewSound("6", "../assets/SF6.ogg");
	sfx->addNewSound("7", "../assets/SF7.ogg");
	resources.push_back(sfx);
	
	miscSounds->addNewSound("start", "../assets/SF1.ogg");
	miscSounds->addNewSound("spawn", "../assets/SF1.ogg");
	miscSounds->addNewSound("death", "../assets/death.ogg");
	miscSounds->addNewSound("win", "../assets/death.ogg");
	resources.push_back(miscSounds);
	
	bumperSfx->addNewSound("bumper1", "../assets/SF2.ogg");
	bumperSfx->addNewSound("bumper2", "../assets/SF3.ogg");
	bumperSfx->addNewSound("bumper3", "../assets/SF4.ogg");
	resources.push_back(bumperSfx);
	
	enemySfx->addNewSound("8", "../assets/SF8.ogg");
	enemySfx->addNewSound("9", "../assets/SF9.ogg");
	enemySfx->addNewSound("10", "../assets/SF10.ogg");
	resources.push_back(enemySfx);

	resources.push_back(donutTop);
	resources.push_back(donutBot);
}