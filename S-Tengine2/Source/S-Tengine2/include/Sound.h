#pragma once
#include <string>

#include <node\Node.h>

namespace libZPlay{
	class ZPlay;
}

class Sound : public Node{
public:
	// value is 0 - 100
	static unsigned long int masterVolume;

	libZPlay::ZPlay * player;

	Sound();
	explicit Sound(std::string _file);
	~Sound();

	void addFile(std::string _file);
	void play(bool _loop = false);
	void pause();
	void stop();
	void resume();
};