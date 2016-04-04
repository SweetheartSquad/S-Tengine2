#pragma once

#include <node\NodeChild.h>
#include <node\NodeUpdatable.h>
#include <node\NodeResource.h>

#include <Log.h>

#include <AL\al.h>
#include <AL\alc.h>
#include <iostream>
#include <string>
#include <map>

class Camera;

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
		Log::error("OpenAL Error - " + std::to_string(err) + ": " + errString);\
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

	static float listenerGain;
	// internally tracks the listener position in order to automatically calculate velocity when desired
	static glm::vec3 listenerPosition;
protected:
    static ALCcontext * context;
    static ALCdevice * device;
public:
	NodeOpenAL();
	static void destruct();
	static void initOpenAL();

	// sets the global OpenAL listener position
	// if _autoVelocity is true, also uses the last known position to set the velocity
	static void setListenerPosition(glm::vec3 _position, bool _autoVelocity = false);
	// sets the global OpenAL listener velocity
	static void setListenerVelocity(glm::vec3 _velocity);
	// sets the global OpenAL listener orientation
	static void setListenerOrientation(glm::vec3 _forward, glm::vec3 _up);
	// sets the global OpenAL listener position and orientation based on _camera's worldPos and childTransform's orientation, respectively
	// if _autoVelocity is true, also uses the last known position to set the velocity
	static void setListener(Camera * _camera, bool _autoVelocity = false);
	// sets the global OpenAL listener gain
	static void setListenerGain(float _gain);
	static float getListenerGain();
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
private:
	// used to initialize the default volume categories
	static std::map<std::string, float> initializeCategoricalGain();
	// volume for the current sound effect
	float gain;
protected:
	ALint samplesPlayed;
public:
	// by default, all calls to setGain are pre-multiplied by the master volume
	static float masterGain;
	// by default, all calls to setGain are pre-multiplied by the sound category's volume
	// the categories "voice", "sfx", "music", and "other" are initialized with a volume of 1.f ("other" is for sounds which have no specified category)
	// other categories are undefined by default
	static std::map<std::string, float> categoricalGain;

	// sound category is used to determine volume
	std::string category;

	OpenAL_Sound(OpenAL_Source * _source, bool _autoRelease, std::string _category);
	virtual ~OpenAL_Sound();
	OpenAL_Source * source;
	
	virtual void update(Step * _step) override;

	// returns the offset in the raw audio data that corresponds to the currently playing audio (-1 when not playing)
	virtual ALint getCurrentSample();
	// returns the value of the sample at the current offset (in the range -1.0 to 1.0)
	float getAmplitude();
	
	// Starts the audio source if stopped, resumes if paused, restarts if playing.
	// also calls setGain with the sound's current gain
	virtual void play(bool _loop = false);
	// Pauses the audio source
	virtual void pause();
	// Stops the audio source + rewinds to the beginning
	virtual void stop();
	
	/**
	 * Source pitch.
	 * Type:    ALfloat
	 * Range:   [0.5 - 2.0]
	 * Default: 1.0
	 *
	 * A multiplier for the frequency (sample rate) of the source's buffer.
	 *
	 * Note that this alters the source, so if the source is used by multiple sound objects, they will all be modified
	 */
	void setPitch(float _pitch);
	/**
	 * Source or listener gain.
	 * Type:  ALfloat
	 * Range: [0.0 - ]
	 *
	 * A value of 1.0 means unattenuated. Each division by 2 equals an attenuation
	 * of about -6dB. Each multiplicaton by 2 equals an amplification of about
	 * +6dB.
	 *
	 * A value of 0.0 is meaningless with respect to a logarithmic scale; it is
	 * silent.
	 *
	 * Note that this alters the source, so if the source is used by multiple sound objects, they will all be modified
	 * 
	 * The provided _gain is pre-multiplied by the master volume and the categorical volume
	 */
	void setGain(float _gain);
	// returns the gain for this sound
	// if _premultiply is true, the value is pre-multiplied by the master volume and the categorical volume
	float getGain(bool _premultiply);

	// Sets the properties which determine how sound is attenuated over distance
	// _referenceDistance is the distance at which the sound is at full volume
	// _maxDistance is the distance at which the sound can't be heard anymore
	// _rolloff is a multiplier
	//
	// From online forums: attenuation = _referenceDistance / (_referenceDistance + (_rolloff * (distance - _referenceDistance)));
	void setPositionalAttributes(float _referenceDistance, float _rolloff = 1.f, float _maxDistance = std::numeric_limits<float>::infinity());
};

class OpenAL_SoundSimple : public OpenAL_Sound{
public:
	OpenAL_SoundSimple(const char * _filename, bool _positional, bool _autoRelease, std::string _category);

	virtual void update(Step * _step) override;
};


class OpenAL_SoundStream : public OpenAL_Sound{
protected:
	// number of buffers used for a single OpenAL_Stream
	unsigned long int numBuffers;
	// size of buffers used for streaming
	unsigned long int bufferLength;
public:
	ALuint * buffers;
	// the number of buffers of the stream which have already been queued
	unsigned long int bufferOffset;
	// maximum number of buffers that could possibly be queued (the number of samples / the number of samples per buffer)
	unsigned long int maxBufferOffset;

	// whether the stream should continue buffering
	bool isStreaming;

	OpenAL_SoundStream(const char * _filename, bool _positional, bool _autoRelease, std::string _category, unsigned long int _bufferLength = 4410, unsigned long int _numBufs = 4);
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
	
	OpenAL_SoundStreamGenerative(bool _positional, bool _autoRelease, std::string _category, unsigned long int _bufferLength = 4410, unsigned long int _numBufs = 4);

	// attempt to fill the _bufferId with new data from the function
	// (uses bufferOffset to determine where to start from)
	// returns the number of buffers filled
	virtual unsigned long int fillBuffer(ALuint _bufferId) override;
};