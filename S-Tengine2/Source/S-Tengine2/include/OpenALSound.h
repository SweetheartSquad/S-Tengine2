#pragma once

#include <node\NodeChild.h>
#include <node\NodeUpdatable.h>
#include <node\NodeResource.h>
#include <AL\alure.h>
#include <iostream>
#include <string>

#ifdef _DEBUG
// OpenAL error-checking macro (enabled because _DEBUG is defined)
// If an error is found, it is printed to the console and the application will fail an assertion
#define checkForAlError(_alFunc) do{\
	(_alFunc);\
	ALenum err = alGetError();\
	if(err != AL_NO_ERROR){\
		std::string errString;\
		switch(err){\
			case AL_NO_ERROR:			errString = "AL_NO_ERROR"; break;\
			case AL_INVALID_NAME:		errString = "AL_INVALID_NAME"; break;\
			case AL_INVALID_ENUM:		errString = "AL_INVALID_ENUM"; break;\
			case AL_INVALID_VALUE:		errString = "AL_INVALID_VALUE"; break;\
			case AL_INVALID_OPERATION:	errString = "AL_INVALID_OPERATION"; break;\
			case AL_OUT_OF_MEMORY:		errString = "AL_OUT_OF_MEMORY"; break;\
			default: break;\
		}\
		std::cout << "OpenAL Error - " << err << ": " << errString << std::endl;\
		assert(false);\
	}\
}while(false)
#else
// OpenAL error-checking macro (disabled because _DEBUG is not defined)
#define checkForAlError(_alFunc) (_alFunc)
#endif

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
};

class OpenAL_Buffer : public virtual NodeOpenAL, public virtual NodeResource{
public:
	ALuint bufferId;
	ALsizei numSamples, sampleRate;
	std::vector<ALshort> samples;
	OpenAL_Buffer(const char * _filename);
	~OpenAL_Buffer();
};

class OpenAL_Source : public virtual NodeOpenAL, public virtual NodeResource, public virtual NodeUpdatable{
public:
	ALuint sourceId;
	OpenAL_Buffer * buffer;
	bool positional;
	bool looping;
	// the current state of the source
	ALint state;
	OpenAL_Source(OpenAL_Buffer * _buffer, bool _positional);
	~OpenAL_Source();
	virtual void update(Step * _step) override;
	
	// sets the audio source's position
	void setPosition(glm::vec3 _pos);
	// sets the audio source's direction
	void setDirection(glm::vec3 _forward, glm::vec3 _up);

	// returns the current position in the sound data (-1 when not playing/paused)
	ALint getSampleOffset();

	// start playing from the audio source
	void play(bool _loop = false);
	void pause();
	void stop();
};

class OpenAL_Sound abstract : public virtual NodeOpenAL, public virtual NodeResource, public virtual NodeUpdatable, public virtual NodeChild{
protected:
	ALint samplesPlayed;
public:
	OpenAL_Sound(OpenAL_Source * _source);
	~OpenAL_Sound();
	OpenAL_Source * source;
	
	virtual void update(Step * _step) override;

	// returns the offset in the raw audio data that corresponds to the currently playing audio (-1 when not playing)
	virtual ALint getCurrentSample();
	// returns the value of the sample at the current offset (in the range -1.0 to 1.0)
	float getAmplitude();
	
	// Starts the audio source if stopped, resumes if paused, restarts if playing.
	virtual void play(bool _loop = false);
	// Pauses the audio source
	virtual void pause();
	// Stops the audio source + rewinds to the beginning
	virtual void stop();
};

class OpenAL_SoundSimple : public OpenAL_Sound{
public:
	OpenAL_SoundSimple(const char * _filename, bool _positional);

	virtual void update(Step * _step) override;
};


// number of buffers used for a single OpenAL_Stream
#define NUM_BUFS 4
// size of buffers used for streaming
#define BUFFER_LEN 44100/10
class OpenAL_SoundStream : public virtual OpenAL_Sound{
public:
	alureStream * stream;
	ALuint buffers[NUM_BUFS];

	// whether the stream should continue buffering
	bool isStreaming;

	OpenAL_SoundStream(const char * _filename, bool _positional);
	~OpenAL_SoundStream();

	virtual void update(Step * _step) override;
	
	// returns the offset in the raw audio data that corresponds to the currently playing audio (-1 when not playing)
	virtual ALint getCurrentSample() override;
	
	// Starts the audio source if stopped, resumes if paused, restarts if playing.
	virtual void play(bool _loop = false) override;
	// Pauses the audio source
	virtual void pause() override;
	// Stops the audio source + rewinds to the beginning
	virtual void stop() override;
	// Unqueues any queued buffers and rewinds the stream
	void rewind();
};