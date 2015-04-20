#pragma once

#include <LD32_ResourceManager.h>

SoundManager * LD32_ResourceManager::music = new SoundManager(-1);
SoundManager * LD32_ResourceManager::sfx = new SoundManager(-1);
SoundManager * LD32_ResourceManager::bumperSfx = new SoundManager(-1);
SoundManager * LD32_ResourceManager::miscSounds = new SoundManager(-1);
Texture * LD32_ResourceManager::donutTop = new Texture("../assets/donutTop.png", 1024, 1024, true, false);
Texture * LD32_ResourceManager::donutBot = new Texture("../assets/donutBot.png", 1024, 1024, true, false);

void LD32_ResourceManager::init(){
	music->addNewSound("bgm", "../assets/Birdy-baseloop.ogg");
	music->addNewSound("bgm2", "../assets/Birdy-birds.ogg");
	music->addNewSound("bgm3", "../assets/Birdy-crash.ogg");
	music->addNewSound("bgm4", "../assets/Birdy-synth.ogg");
	resources.push_back(music);

	sfx->addNewSound("1", "../assets/SF1.ogg");
	sfx->addNewSound("5", "../assets/SF5.ogg");
	sfx->addNewSound("6", "../assets/SF6.ogg");
	sfx->addNewSound("7", "../assets/SF7.ogg");
	sfx->addNewSound("8", "../assets/SF8.ogg");
	sfx->addNewSound("9", "../assets/SF9.ogg");
	sfx->addNewSound("10", "../assets/SF10.ogg");
	resources.push_back(sfx);
	
	miscSounds->addNewSound("start", "../assets/SF9.ogg");
	miscSounds->addNewSound("death", "../assets/SF10.ogg");
	resources.push_back(miscSounds);
	
	bumperSfx->addNewSound("bumper1", "../assets/SF2.ogg");
	bumperSfx->addNewSound("bumper2", "../assets/SF3.ogg");
	bumperSfx->addNewSound("bumper3", "../assets/SF4.ogg");
	resources.push_back(bumperSfx);

	resources.push_back(donutTop);
	resources.push_back(donutBot);
}