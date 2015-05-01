#pragma once

#include "SoundManager.h"
#include <NumberUtils.h>
#include <Vox.h>

SoundManager::SoundManager(double _throttle):
	NodeResource(true),
	throttle(_throttle),
	lastTimeStamp(0.0),
	lastPlayed(0)
{
}

SoundManager::~SoundManager(){
	for(auto sound : sounds){
		sound.second.stop();
	}
	sounds.clear();
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

void SoundManager::play(std::string _name, bool _loop){
	if(vox::step.time - lastTimeStamp > throttle){
		sounds.at(_name).play(_loop);
		lastTimeStamp = vox::step.time;
	}
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

void SoundManager::load(){
}

void SoundManager::unload(){
}

Sound & SoundManager::playRandomSound(){
	if(sounds.size() > 1){
		unsigned long int soundToPlay;
		do{
			soundToPlay = vox::NumberUtils::randomInt(0, sounds.size() - 1);
		}while(soundToPlay == lastPlayed);
		lastPlayed = soundToPlay;
		auto it = sounds.begin();
		std::advance(it, soundToPlay);
		auto s = it->first;
		play(s);
		return it->second;
	}else if(sounds.size() == 1){
		play(sounds.begin()->first);
		return sounds.begin()->second;
	}
}