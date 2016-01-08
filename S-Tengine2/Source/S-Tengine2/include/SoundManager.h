#pragma once
#include <map>

#include <OpenALSound.h>
#include <node/NodeResource.h>

class SoundManager : public NodeResource{
public:

	double throttle;
	double lastTimeStamp;
	unsigned long int lastPlayed;

	std::map<std::string, OpenAL_Sound *> sounds;

	explicit SoundManager(double _throttle);
	~SoundManager();

	// creates a new simple sound using an audio file located at _fileName
	// sound is non-positional by default
	void addNewSound(std::string _name, std::string _fileName, std::string _category);

	// adds _sound to this sound manager
	// note that the manager deletes sounds when it is deleted
	void addSound(std::string _name, OpenAL_Sound * _sound);
	void play(std::string _name, bool _loop = false);
	void pause(std::string _name);
	void stop(std::string _name);
	OpenAL_Sound * playRandomSound();
};