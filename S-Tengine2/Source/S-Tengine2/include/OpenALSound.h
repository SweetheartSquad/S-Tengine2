#pragma once

#include <node\NodeChild.h>
#include <node\NodeUpdatable.h>
#include <node\NodeResource.h>
#include <AL\alure.h>
#include <iostream>

//#define NUM_BUFS 32

class OpenAL_Sound : public virtual NodeUpdatable, public virtual NodeResource, public virtual NodeChild{
private:
	static bool inited;

	static void initOpenAL();
	static void uninitOpenAL();
public:
    static ALCcontext * context;
    static ALCdevice * device;
	
	// sets the global OpenAL listener position
	static void setListenerPosition(glm::vec3 _position);
	// sets the global OpenAL listener position
	static void setListenerVelocity(glm::vec3 _velocity);
	// sets the global OpenAL listener orientation
	static void setListenerOrientation(glm::vec3 _forward, glm::vec3 _up);

	ALuint sourceId;
	ALuint bufferId;
	alureStream * stream;

	OpenAL_Sound(const char * _filename);
	~OpenAL_Sound();
	
	virtual void update(Step * _step) override;
	virtual void load() override;
	virtual void unload() override;
	
	void setPosition(glm::vec3 _pos);
	void setDirection(glm::vec3 _forward, glm::vec3 _up);

	
	void play(bool _loop = false);
	void pause();
	void stop();
	void resume();
};
class OpenAL_Stream : public OpenAL_Sound{
public:
	OpenAL_Stream(const char * _filename);
	~OpenAL_Stream();
	void update(Step * _step) override;
};