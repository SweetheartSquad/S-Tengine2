#pragma once

#include <LD32_ResourceManager.h>

SoundManager * LD32_ResourceManager::music = new SoundManager(-1);

void LD32_ResourceManager::init(){
	music->addNewSound("bgm", "../assets/thing6 - guitar.ogg");
	resources.push_back(music);
}