#pragma once

#include <OpenALSound.h>
#include <Transform.h>
#include <Camera.h>

#include <sndfile.hh>

#include <algorithm>


ALCcontext * NodeOpenAL::context = nullptr; 
ALCdevice * NodeOpenAL::device = nullptr;

bool NodeOpenAL::inited = false;
float NodeOpenAL::listenerGain = 1.f;
glm::vec3 NodeOpenAL::listenerPosition(0);

NodeOpenAL::NodeOpenAL(){
	initOpenAL();
}

void NodeOpenAL::initOpenAL(){
	// initialize OpenAL
	if(!inited){
	   ALCint contextAttr[] = {ALC_FREQUENCY, 44100,0};
	   device = alcOpenDevice(NULL);
	   context = alcCreateContext(device, contextAttr);
	   alcMakeContextCurrent(context);

	   // default listener: at origin, facing towards the negative Z axis, with head aligned with positive Y axis
	   checkForAlError(alListener3f( AL_POSITION, 0.f, 0.f, 0.f ));
	   checkForAlError(alListener3f( AL_VELOCITY, 0.f, 0.f, 0.f ));
	   float orient[6] = { /*fwd:*/ 0.f, 0.f, -1.f, /*up:*/ 0.f, 1.f, 0.f };
	   checkForAlError(alListenerfv( AL_ORIENTATION, orient ));
	   
	   inited = true;
	}
}

void NodeOpenAL::destruct(){
	if(inited){
		alcDestroyContext(context);
		alcCloseDevice(device);
		inited = false;
	}
}

void NodeOpenAL::setListenerPosition(glm::vec3 _position, bool _autoVelocity){
	checkForAlError(alListener3f(AL_POSITION, _position.x, _position.y, _position.z));
	if(_autoVelocity){
		setListenerVelocity(_position - listenerPosition);
	}
	listenerPosition = _position;
}
void NodeOpenAL::setListenerVelocity(glm::vec3 _velocity){
	checkForAlError(alListener3f(AL_VELOCITY, _velocity.x, _velocity.y, _velocity.z));
}
void NodeOpenAL::setListenerOrientation(glm::vec3 _forward, glm::vec3 _up){
	float orientation[6] = {_forward.x, _forward.y, _forward.z, _up.x, _up.y, _up.z};
	checkForAlError(alListenerfv(AL_ORIENTATION, orientation));
}

void NodeOpenAL::setListener(Camera * _camera, bool _autoVelocity){
	setListenerPosition(_camera->getWorldPos(), _autoVelocity);
	setListenerOrientation(_camera->forwardVectorRotated, _camera->upVectorRotated);
}

void NodeOpenAL::setListenerGain(float _gain){
	listenerGain = _gain;
	checkForAlError(alListenerf(AL_GAIN, listenerGain));
}
float NodeOpenAL::getListenerGain(){
	return listenerGain;
}

OpenAL_Buffer::OpenAL_Buffer(const char * _filename, bool _autoRelease) :
	NodeResource(_autoRelease),
	bufferId(0),
	sampleRate(0),
	numSamples(0)
{
	// generate buffer
	checkForAlError(alGenBuffers(1, &bufferId));

	if(_filename != nullptr){
		// open the file
		SF_INFO fileInfo;
		SNDFILE * file = sf_open(_filename, SFM_READ, &fileInfo);

		// get the number of samples and sample rate
		numSamples = static_cast<ALsizei>(fileInfo.channels * fileInfo.frames);
		sampleRate = static_cast<ALsizei>(fileInfo.samplerate);

		// read the audio data (as a 16 bit signed integer)
		samples = (ALshort *)calloc(numSamples, sizeof(ALshort));
		// sf_read_short returns the number of samples read
		sf_count_t samplesRead = sf_read_short(file, samples, numSamples);
		// if we read less samples than requested, there must be an error
		assert(samplesRead == numSamples); 

		// close the file
		sf_close(file);

		 // set the format based on the number of channels
		switch (fileInfo.channels){
			case 1:  format = AL_FORMAT_MONO16;   break;
			case 2:  format = AL_FORMAT_STEREO16; break;
			default: format = AL_FORMAT_MONO16;
		}
		assert(format == AL_FORMAT_MONO16 || format == AL_FORMAT_STEREO16);

		// fill the buffer with the audio data
		checkForAlError(alBufferData(bufferId, format, &samples[0], numSamples * sizeof(ALushort), sampleRate));
	}
}

OpenAL_Buffer::~OpenAL_Buffer(){	
	checkForAlError(alDeleteBuffers(1, &bufferId));
	free(samples);
}





OpenAL_Source::OpenAL_Source(OpenAL_Buffer * _buffer, bool _positional, bool _autoRelease) :
	buffer(_buffer),
	positional(_positional),
	looping(false),
	state(AL_STOPPED),
	NodeResource(_autoRelease)
{
	// generate source
	checkForAlError(alGenSources(1, &sourceId));

	// turn off looping by default
	checkForAlError(alSourcei(sourceId, AL_LOOPING, AL_FALSE));
	checkForAlError(alSourcef(sourceId, AL_PITCH, 1.f));
	checkForAlError(alSourcef(sourceId, AL_GAIN, 1.f));
	checkForAlError(alDopplerFactor(1.f));
	checkForAlError(alDopplerVelocity(1.f));
	checkForAlError(alSource3f(sourceId, AL_POSITION, 0, 0, 0));
	checkForAlError(alSource3f(sourceId, AL_VELOCITY, 0, 0, 0));
    checkForAlError(alSourcef(sourceId, AL_ROLLOFF_FACTOR, 1.0f));
	checkForAlError(alSourcef(sourceId, AL_REFERENCE_DISTANCE, 1)); // 1.0f
	checkForAlError(alSourcef(sourceId, AL_MAX_DISTANCE, std::numeric_limits<float>::infinity()));  // 1000.0f

	float orientation[6] = {
		/*forward vector*/
		1.f, 0.f, 0.f,
		/*up vector*/
		0.f, 1.f, 0.f
	};
	checkForAlError(alSourcefv(sourceId, AL_DIRECTION, orientation));
	if(!positional){
		checkForAlError(alSourcei(sourceId, AL_SOURCE_RELATIVE, AL_TRUE));
	}

	if(_buffer != nullptr){
		// attach the buffer to the source
		checkForAlError(alSourcei(sourceId, AL_BUFFER, buffer->bufferId));
		++_buffer->referenceCount;
	}
}

OpenAL_Source::~OpenAL_Source(){
	checkForAlError(alDeleteSources(1, &sourceId));
	buffer->decrementAndDelete();
}

void OpenAL_Source::update(Step * _step){
	// keep the source state up-to-date
	checkForAlError(alGetSourcei(sourceId, AL_SOURCE_STATE, &state));
}

void OpenAL_Source::setPosition(glm::vec3 _pos){
	checkForAlError(alSource3f(sourceId, AL_POSITION, _pos.x, _pos.y, _pos.z));
}
void OpenAL_Source::setDirection(glm::vec3 _forward, glm::vec3 _up){
	float orientation[6] = {_forward.x, _forward.y, _forward.z, _up.x, _up.y, _up.z};
	checkForAlError(alSourcefv(sourceId, AL_ORIENTATION, orientation));
}

ALint OpenAL_Source::getSampleOffset(){
	if(state != AL_PLAYING && state != AL_PAUSED){
		// if the source isn't mid-sound, return -1 to indicate that there is no associated sample
		return -1;
	}
	ALint sampleOffset;
	checkForAlError(alGetSourcei(sourceId, AL_SAMPLE_OFFSET, &sampleOffset));
	return sampleOffset;
}

void OpenAL_Source::play(bool _loop){	
	// set the loop parameter
	looping = _loop;
	checkForAlError(alSourcei(sourceId, AL_LOOPING, _loop));

	// Start playing source
	checkForAlError(alSourcePlay(sourceId));
	state = AL_PLAYING;
}

void OpenAL_Source::stop(){	
	// Stop playing source
	checkForAlError(alSourceStop(sourceId));
	looping = false;
	state = AL_STOPPED;
}

void OpenAL_Source::pause(){	
	// Pause source
	checkForAlError(alSourcePause(sourceId));
	looping = false;
	state = AL_PAUSED;
}







float OpenAL_Sound::masterGain = 1.f;
std::map<std::string, float> OpenAL_Sound::categoricalGain = initializeCategoricalGain();
std::map<std::string, float> OpenAL_Sound::initializeCategoricalGain(){
	std::map<std::string, float> res;
	res["voice"] = res["sfx"] = res["music"] = res["other"] = 1.f;
	return res;
}

OpenAL_Sound::OpenAL_Sound(OpenAL_Source * _source, bool _autoRelease, std::string _category) :
	NodeResource(_autoRelease),
	source(_source),
	samplesPlayed(0),
	category(_category),
	gain(1.f)
{
}

OpenAL_Sound::~OpenAL_Sound(){
	source->decrementAndDelete();
}

void OpenAL_Sound::update(Step * _step){
	source->update(_step);
	if(source->positional){
		source->setPosition(this->getWorldPos());
	}
}

void OpenAL_Sound::play(bool _loop){
	setGain(gain);
	source->play(_loop);
}void OpenAL_Sound::pause(){
	source->pause();
}void OpenAL_Sound::stop(){
	source->stop();
}

void OpenAL_Sound::setPitch(float _pitch){
	checkForAlError(alSourcef(source->sourceId, AL_PITCH, _pitch));
}
void OpenAL_Sound::setGain(float _gain){
	gain = _gain;
	checkForAlError(alSourcef(source->sourceId, AL_GAIN, gain * masterGain * categoricalGain[category]));
}

void OpenAL_Sound::setPositionalAttributes(float _referenceDistance, float _rolloff, float _maxDistance){
	checkForAlError(alSourcef(source->sourceId, AL_REFERENCE_DISTANCE, _referenceDistance));
	checkForAlError(alSourcef(source->sourceId, AL_MAX_DISTANCE, _maxDistance));
	checkForAlError(alSourcef(source->sourceId, AL_ROLLOFF_FACTOR, _rolloff));
}

ALint OpenAL_Sound::getCurrentSample(){
	return source->getSampleOffset();
}

float OpenAL_Sound::getAmplitude(){
	ALint t = getCurrentSample();
	if(t < 0){
		return 0;
	}
	return (float)source->buffer->samples[t]/INT16_MAX;
}




OpenAL_SoundSimple::OpenAL_SoundSimple(const char * _filename, bool _positional, bool _autoRelease, std::string _category) :
	OpenAL_Sound(new OpenAL_Source(new OpenAL_Buffer(_filename, true), _positional, true), _autoRelease, _category),
	NodeResource(_autoRelease)
{
}

void OpenAL_SoundSimple::update(Step * _step){
	OpenAL_Sound::update(_step);
	samplesPlayed = source->getSampleOffset();
}


OpenAL_SoundStream::OpenAL_SoundStream(const char * _filename, bool _positional, bool _autoRelease, std::string _category, unsigned long int _bufferLength, unsigned long int _numBufs) :
	NodeResource(_autoRelease),
	isStreaming(false),
	OpenAL_Sound(new OpenAL_Source(nullptr, _positional, true), _autoRelease, _category),
	bufferOffset(0),
	maxBufferOffset(0),
	numBuffers(_numBufs),
	bufferLength(_bufferLength)
{
	buffers = (ALuint *)calloc(numBuffers, sizeof(ALuint));
	alGenBuffers(numBuffers, buffers);
	source->buffer = new OpenAL_Buffer(_filename, true);
	++source->buffer->referenceCount;

	maxBufferOffset = source->buffer->numSamples / bufferLength;
}


OpenAL_SoundStream::~OpenAL_SoundStream(){
	stop();
	checkForAlError(alDeleteBuffers(numBuffers, buffers));
	free(buffers);
}

void OpenAL_SoundStream::update(Step * _step){
	// keep the stream informed of the source state
	OpenAL_Sound::update(_step);

    // Get the number of buffers that have been processed and are ready for reuse
    ALint numBufs = 0;
	checkForAlError(alGetSourcei(source->sourceId, AL_BUFFERS_PROCESSED, &numBufs));
	
	while(numBufs--){
		samplesPlayed += bufferLength;
		ALuint tempBuf;
		// unqueue the processed buffer
		checkForAlError(alSourceUnqueueBuffers(source->sourceId, 1, &tempBuf));
		// attempt to fill the unqueued buffer with new data from the stream
		ALsizei numToQueue = fillBuffer(tempBuf);
		if(numToQueue <= 0){
			// if the stream didn't fill any of the buffers,
			// the stream end has been reached, so we rewind it.
			bufferOffset = 0;

			samplesPlayed = 0;
			if(!source->looping){
				isStreaming = false;
			}else{
				// now that the stream is rewound, we can try to grab
				// a new buffer so that we can loop
				numToQueue = fillBuffer(tempBuf);
			}
		}

		if(isStreaming){
			// queue the newly filled buffer on the source
			checkForAlError(alSourceQueueBuffers(source->sourceId, numToQueue, &tempBuf));
		}
	}

	// If isStreaming == true, the sound should be playing
	// but as a result of the source running out of queued
	// buffers, it will stop playing. We can double-check
	// the state and restart it if needed.
	if(isStreaming && source->state != AL_PLAYING){
		checkForAlError(alSourcePlay(source->sourceId));
	}
}

void OpenAL_SoundStream::play(bool _loop){
	// if the stream is paused, we have to behave differently
	if(source->state == AL_PAUSED){
		// if there aren't any buffers queued, queue up some more (otherwise nothing will play)
		ALint numQueuedBuffers = 0;
		checkForAlError(alGetSourcei(source->sourceId, AL_BUFFERS_QUEUED, &numQueuedBuffers));
		if(numQueuedBuffers <= 0){
			ALsizei numBuffs = fillBuffers();
			checkForAlError(alSourceQueueBuffers(source->sourceId, numBuffs, buffers));
		}
		OpenAL_Sound::play(false);
		source->looping = _loop; // we can't call play(_loop) because it sets AL_LOOPING to _loop, and you aren't supposed to do that on a streaming source

		isStreaming = true;
	}else{
		stop();
		ALsizei numBuffs = fillBuffers();
		checkForAlError(alSourceQueueBuffers(source->sourceId, numBuffs, buffers));
		OpenAL_Sound::play(false);
		source->looping = _loop; // we can't call play(_loop) because it sets AL_LOOPING to _loop, and you aren't supposed to do that on a streaming source

		isStreaming = true;
	}
}

void OpenAL_SoundStream::pause(){
	// if we aren't playing, then we can't pause
	if(source->state != AL_PLAYING){
		return;
	}
	OpenAL_Sound::pause();
	isStreaming = false;
}

void OpenAL_SoundStream::stop(){
	OpenAL_Sound::stop();
	rewind();
	samplesPlayed = 0;
	isStreaming = false;
}

void OpenAL_SoundStream::rewind(){
	// move to the beginning
	ALint numQueuedBuffers = 0;
	checkForAlError(alGetSourcei(source->sourceId, AL_BUFFERS_QUEUED, &numQueuedBuffers));
	ALuint * tempBuf = (ALuint *)calloc(numBuffers, sizeof(ALuint));
	checkForAlError(alSourceUnqueueBuffers(source->sourceId, numQueuedBuffers, tempBuf));
	bufferOffset = 0;
	free(tempBuf);
}


ALint OpenAL_SoundStream::getCurrentSample(){
	ALint t = source->getSampleOffset();
	if(t == -1 && samplesPlayed > 0){
		t = samplesPlayed;
	}else{
		t += samplesPlayed;
	}
	return std::min(t, source->buffer->numSamples-1);
}

unsigned long int OpenAL_SoundStream::fillBuffer(ALuint _bufferId){
	unsigned long int numSamplesToBuffer;
	if(bufferOffset < maxBufferOffset){
		numSamplesToBuffer = bufferLength;
	}else if(bufferOffset == maxBufferOffset){
		numSamplesToBuffer = source->buffer->numSamples - bufferOffset*bufferLength;
	}else{
		return 0;
	}
	checkForAlError(alBufferData(_bufferId, source->buffer->format, &source->buffer->samples[bufferOffset*bufferLength], numSamplesToBuffer * sizeof(ALushort), source->buffer->sampleRate));
	++bufferOffset;
	return 1;
}

unsigned long int OpenAL_SoundStream::fillBuffers(){
	ALsizei numBuffs = 0;
	while(numBuffs < 4){
		if(!fillBuffer(buffers[numBuffs++])){
			break;
		}
	}
	return numBuffs;
}




OpenAL_SoundStreamGenerative::OpenAL_SoundStreamGenerative(bool _positional, bool _autoRelease, std::string _category, unsigned long int _bufferLength, unsigned long int _numBufs) :
	OpenAL_SoundStream(nullptr, _positional, _autoRelease, _category, _bufferLength, _numBufs),
	NodeResource(_autoRelease),
	generativeFunction(nullptr)
{
	maxBufferOffset = -1;
	source->buffer->samples = (ALshort *)calloc(bufferLength, sizeof(ALshort));
	source->buffer->sampleRate = 44100;
	source->buffer->format = AL_FORMAT_MONO16;

	generativeFunction = [](unsigned long int t){
		return compressFloat(sin(t/10), 0.2f);
	};
}

ALshort OpenAL_SoundStreamGenerative::compressFloat(float _v, float _volume){
	return _v * 32767 * _volume;
}

unsigned long int OpenAL_SoundStreamGenerative::fillBuffer(ALuint _bufferId){
	for(unsigned long int i = 0; i < source->buffer->numSamples; ++i){
		unsigned long int t = i + samplesPlayed;
		source->buffer->samples[i] = generativeFunction(t);
	}

	checkForAlError(alBufferData(_bufferId, source->buffer->format, &source->buffer->samples[0], bufferLength * sizeof(ALushort), source->buffer->sampleRate));
	++bufferOffset;
	return 1;
}