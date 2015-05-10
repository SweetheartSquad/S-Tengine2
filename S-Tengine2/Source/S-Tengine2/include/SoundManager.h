#pragma once
#include <map>

#include <Sound.h>
#include <node/NodeResource.h>

class SoundManager : public NodeResource{
public:

	double throttle;
	double lastTimeStamp;
	unsigned long int lastPlayed;

	std::map<std::string, Sound> sounds;

	explicit SoundManager(double _throttle);
	~SoundManager();

	void addNewSound(std::string _name);
	void addNewSound(std::string _name, std::string _fileName);
	void addSound(std::string _name, Sound sound);
	void addFile(std::string _name, std::string _fileName);
	void play(std::string _name, bool _loop = false);
	void pause(std::string _name);
	void stop(std::string _name);
	void resume(std::string _name);
	Sound * playRandomSound();

	void load() override;
	void unload() override;
};