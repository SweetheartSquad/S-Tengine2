#pragma once

#include <LD32_ResourceManager.h>

SoundManager * LD32_ResourceManager::music = new SoundManager(-1);
Texture * LD32_ResourceManager::donutTop = new Texture("../assets/donutTop.png", 1024, 1024, true, true);
Texture * LD32_ResourceManager::donutBot = new Texture("../assets/donutBot.png", 1024, 1024, true, true);

void LD32_ResourceManager::init(){
	music->addNewSound("bgm", "../assets/thing6 - guitar.ogg");
	resources.push_back(music);
	
	resources.push_back(donutTop);
	resources.push_back(donutBot);
}