#pragma once

#include <node\NodeChild.h>
#include <node\NodeUpdatable.h>
#include <node\NodeResource.h>
#include <AL\alure.h>
#include <iostream>

//#define NUM_BUFS 32

class NodeOpenAL abstract : public Node{
private:
	static bool inited;

	static void initOpenAL();
	static void uninitOpenAL();

protected:
    static ALCcontext * context;
    static ALCdevice * device;
public:
	NodeOpenAL();

	// sets the global OpenAL listener position
	static void setListenerPosition(glm::vec3 _position);
	// sets the global OpenAL listener position
	static void setListenerVelocity(glm::vec3 _velocity);
	// sets the global OpenAL listener orientation
	static void setListenerOrientation(glm::vec3 _forward, glm::vec3 _up);
	// if an OpenAL error is found, prints it to the console
	static void checkError();
};

class OpenAL_Buffer : public virtual NodeOpenAL, public virtual NodeResource{
public:
	ALuint bufferId;
	OpenAL_Buffer(const char * _filename);
	~OpenAL_Buffer();
};

class OpenAL_Source : public virtual NodeOpenAL, public virtual NodeResource{
public:
	ALuint sourceId;
	OpenAL_Buffer * buffer;
	bool positional;
	bool looping;
	OpenAL_Source(OpenAL_Buffer * _buffer, bool _positional);
	~OpenAL_Source();
	
	void setPosition(glm::vec3 _pos);
	void setDirection(glm::vec3 _forward, glm::vec3 _up);

	void play(bool _loop = false);
	void pause();
	void stop();
};

class OpenAL_Sound : public virtual NodeUpdatable, public virtual NodeResource, public virtual NodeChild{
public:
	OpenAL_Source * source;
	
	OpenAL_Sound(OpenAL_Source * _source);
	OpenAL_Sound(const char * _filename, bool _positional);
	~OpenAL_Sound();
	
	virtual void update(Step * _step) override;
};


// number of buffers used for a single OpenAL_Stream
#define NUM_BUFS 4
class OpenAL_Stream : public virtual NodeUpdatable, public virtual NodeResource, public virtual NodeChild{
public:
	alureStream * stream;
	OpenAL_Source * source;
	ALuint buffers[NUM_BUFS];

	OpenAL_Stream(const char * _filename, bool _positional);
	~OpenAL_Stream();

	void update(Step * _step) override;
	
	// Starts the audio stream if stopped, resumes if paused, restarts if playing.
	void play(bool _loop = false);
	// Pauses the audio stream
	void pause();
	// Stops the audio stream + rewinds to the beginning
	void stop();

	void rewind();

	bool isStreaming;
	ALenum state;
};