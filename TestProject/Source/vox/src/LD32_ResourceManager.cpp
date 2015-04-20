#pragma once

#include <LD32_ResourceManager.h>

SoundManager * LD32_ResourceManager::music = new SoundManager(-1);
SoundManager * LD32_ResourceManager::sfx = new SoundManager(-1);
Texture * LD32_ResourceManager::donutTop = new Texture("../assets/donutTop.png", 1024, 1024, true, false);
Texture * LD32_ResourceManager::donutBot = new Texture("../assets/donutBot.png", 1024, 1024, true, false);

void LD32_ResourceManager::init(){
	music->addNewSound("bgm", "../assets/Birdy.ogg");
	resources.push_back(music);

	sfx->addNewSound("1", "../assets/SF1.ogg");
	sfx->addNewSound("2", "../assets/SF2.ogg");
	sfx->addNewSound("3", "../assets/SF3.ogg");
	sfx->addNewSound("4", "../assets/SF4.ogg");
	sfx->addNewSound("5", "../assets/SF5.ogg");
	sfx->addNewSound("6", "../assets/SF6.ogg");
	sfx->addNewSound("7", "../assets/SF7.ogg");
	sfx->addNewSound("8", "../assets/SF8.ogg");
	sfx->addNewSound("9", "../assets/SF9.ogg");
	sfx->addNewSound("10", "../assets/SF10.ogg");
	resources.push_back(sfx);
	
	resources.push_back(donutTop);
	resources.push_back(donutBot);
}