#pragma once
#include <string>

namespace libZPlay{
	class ZPlay;
}

class Sound{
public:
	// value is 0 - 100
	static unsigned long int masterVolume;

	libZPlay::ZPlay * player;

	Sound();
	explicit Sound(std::string _file);
	~Sound();

	void addFile(std::string _file);
	void play();
	void pause();
	void stop();
	void resume();
};