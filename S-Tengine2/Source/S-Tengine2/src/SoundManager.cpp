#pragma once

#include "SoundManager.h"
#include <NumberUtils.h>
#include <Sweet.h>

SoundManager::SoundManager(double _throttle):
	NodeResource(true),
	throttle(_throttle),
	lastTimeStamp(0.0),
	lastPlayed(0)
{
}

SoundManager::~SoundManager(){
	for(auto sound : sounds){
#pragma warning(suppress: 28182) // the sound map should never have a reference to a nullptr
		sound.second->stop();
		delete sound.second;
	}
	sounds.clear();
}

void SoundManager::addNewSound(std::string _name, std::string _fileName){
	sounds.insert(std::make_pair(_name, new OpenAL_SoundSimple(_fileName.c_str(), false, false)));
}

void SoundManager::addSound(std::string _name, OpenAL_Sound * _sound){
	sounds.insert(std::make_pair(_name, _sound));
}

void SoundManager::play(std::string _name, bool _loop){
	if(sweet::step.time - lastTimeStamp > throttle){
		sounds.at(_name)->play(_loop);
		lastTimeStamp = sweet::step.time;
	}
}

void SoundManager::pause(std::string _name){
	sounds.at(_name)->pause();
}

void SoundManager::stop(std::string _name){
	sounds.at(_name)->stop();
}

OpenAL_Sound * SoundManager::playRandomSound(){
	if(sounds.size() > 1){
		unsigned long int soundToPlay;
		do{
			soundToPlay = sweet::NumberUtils::randomInt(0, sounds.size() - 1);
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
	}else{
		return nullptr;
	}
}