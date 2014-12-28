#pragma once

#include "SoundManager.h"

SoundManager::SoundManager(){
}

SoundManager::~SoundManager(){
}

void SoundManager::addNewSound(std::string _name){
	sounds.insert(std::make_pair(_name, Sound()));
}

void SoundManager::addNewSound(std::string _name, std::string _fileName){
	sounds.insert(std::make_pair(_name, Sound(_fileName)));
}

void SoundManager::addSound(std::string _name, Sound _sound){
	sounds.insert(std::make_pair(_name, _sound));
}

void SoundManager::addFile(std::string _name, std::string _fileName){
	sounds.at(_name).addFile(_fileName);
}

void SoundManager::play(std::string _name){
	sounds.at(_name).play();
}

void SoundManager::pause(std::string _name){
	sounds.at(_name).pause();
}

void SoundManager::stop(std::string _name){
	sounds.at(_name).stop();
}

void SoundManager::resume(std::string _name){
	sounds.at(_name).resume();
}