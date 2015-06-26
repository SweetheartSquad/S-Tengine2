#pragma once

#include <node\NodeChild.h>
#include <node\NodeUpdatable.h>
#include <node\NodeResource.h>
#include <AL\al.h>
#include <AL\alc.h>
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
protected:
    static ALCcontext * context;
    static ALCdevice * device;
public:
	NodeOpenAL();
	static void destruct();
	static void initOpenAL();

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
    ALenum format;
	ALshort * samples;
	OpenAL_Buffer(const char * _filename, bool _autoRelease);
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
	OpenAL_Source(OpenAL_Buffer * _buffer, bool _positional, bool _autoRelease);
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
	OpenAL_Sound(OpenAL_Source * _source, bool _autoRelease);
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
	OpenAL_SoundSimple(const char * _filename, bool _positional, bool _autoRelease);

	virtual void update(Step * _step) override;
};


class OpenAL_SoundStream : public OpenAL_Sound{
protected:
	// number of buffers used for a single OpenAL_Stream
	const static unsigned long int NUM_BUFS = 4;
	// size of buffers used for streaming
	const static unsigned long int BUFFER_LEN = 44100/10;
public:
	ALuint buffers[NUM_BUFS];
	// the number of buffers of the stream which have already been queued
	unsigned long int bufferOffset;
	// maximum number of buffers that could possibly be queued (the number of samples / the number of samples per buffer)
	unsigned long int maxBufferOffset;

	// whether the stream should continue buffering
	bool isStreaming;

	OpenAL_SoundStream(const char * _filename, bool _positional, bool _autoRelease);
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

	// attempt to fill the _bufferId with new data from the source
	// (uses bufferOffset to determine where to start from)
	// returns the number of buffers filled
	virtual unsigned long int fillBuffer(ALuint _bufferId);
	// fills buffers with as much data from the source as possible, limited by maxBufferOffset
	// (uses bufferOffset to determine where to start from)
	// returns the number of buffers filled
	unsigned long int fillBuffers();
};

#include <functional>
class OpenAL_SoundStreamGenerative : public OpenAL_SoundStream{
public:
	// converts a float in the range -1 to 1 into an ALShort in the range -32767 to 32767 (the min and max amplitude)
	static ALshort compressFloat(float _v, float _volume = 0.8f);

	std::function<ALshort(unsigned long int _time)> generativeFunction;
	
	OpenAL_SoundStreamGenerative(bool _positional, bool _autoRelease);

	// attempt to fill the _bufferId with new data from the function
	// (uses bufferOffset to determine where to start from)
	// returns the number of buffers filled
	virtual unsigned long int fillBuffer(ALuint _bufferId) override;
};