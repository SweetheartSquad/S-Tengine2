#pragma once
#include <map>

#include "Sound.h"

class SoundManager{
public:

	std::map<std::string, Sound> sounds;

	SoundManager();
	~SoundManager();

	void addNewSound(std::string _name);
	void addNewSound(std::string _name, std::string _fileName);
	void addSound(std::string _name, Sound sound);
	void addFile(std::string _name, std::string _fileName);
	void play(std::string _name);
	void pause(std::string _name);
	void stop(std::string _name);
	void resume(std::string _name);
};