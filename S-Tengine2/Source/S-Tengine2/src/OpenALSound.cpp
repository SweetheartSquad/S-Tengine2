#pragma once

#include <OpenALSound.h>
#include <Transform.h>

#include <sndfile.hh>

ALCcontext * NodeOpenAL::context = nullptr; 
ALCdevice * NodeOpenAL::device = nullptr;

bool NodeOpenAL::inited = false;

NodeOpenAL::NodeOpenAL(){
	initOpenAL();
}

void NodeOpenAL::initOpenAL(){
	// initialize OpenAL
	if(!inited){
		/*alureInitDevice(nullptr, nullptr);*/
		
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

void NodeOpenAL::uninitOpenAL(){
	if(inited){
		//alureShutdownDevice();
		alcDestroyContext(context);
		alcCloseDevice(device);
		inited = false;
	}
}

OpenAL_Buffer::OpenAL_Buffer(const char * _filename) :
	NodeResource(false),
	bufferId(0),
	sampleRate(0),
	numSamples(0)
{
	// open the file
    SF_INFO fileInfo;
	SNDFILE * file = sf_open(_filename, SFM_READ, &fileInfo);

	/*checkForAlError(alBufferData(bufferId,
		file.channels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
		buffer, BUFFER_LEN * sizeof(short), file.samplerate()));*/

	// get the number of samples and sample rate
	numSamples = static_cast<ALsizei>(fileInfo.channels * fileInfo.frames);
    sampleRate = static_cast<ALsizei>(fileInfo.samplerate);

	// read the audio data (as a 16 bit signed integer)
    samples.resize(numSamples);
	// sf_read_short returns the number of samples read
    // if we read zero samples, there must be an error
	assert(!sf_read_short(file, &samples[0], numSamples) < numSamples); 

	// close the file
	sf_close(file);

	 // set the format based on the number of channels
    ALenum format;
    switch (fileInfo.channels){
        case 1:  format = AL_FORMAT_MONO16;   break;
        case 2:  format = AL_FORMAT_STEREO16; break;
        default: format = 0;
    }
	assert(format == AL_FORMAT_MONO16 || format == AL_FORMAT_STEREO16);

	
	// generate buffer
	checkForAlError(alGenBuffers(1, &bufferId));

	// fill the buffer with the audio data
   checkForAlError(alBufferData(bufferId, format, &samples[0], numSamples * sizeof(ALushort), sampleRate));
}

OpenAL_Buffer::~OpenAL_Buffer(){	
	checkForAlError(alDeleteBuffers(1, &bufferId));
}





OpenAL_Source::OpenAL_Source(OpenAL_Buffer * _buffer, bool _positional) :
	buffer(_buffer),
	positional(_positional),
	looping(false),
	state(AL_STOPPED),
	NodeResource(false)
{
	// generate source
	checkForAlError(alGenSources(1, &sourceId));

	// turn off looping by default
	checkForAlError(alSourcei(sourceId, AL_LOOPING, AL_FALSE));
	checkForAlError(alSourcef(sourceId, AL_PITCH, 1.f));
	checkForAlError(alSourcef(sourceId, AL_GAIN, 0.5f));
	checkForAlError(alSourcef(sourceId, AL_ROLLOFF_FACTOR, 0.05f));
	checkForAlError(alDopplerFactor(1.f));
	checkForAlError(alDopplerVelocity(1.f));
	checkForAlError(alSource3f(sourceId, AL_POSITION, 0, 0, 0));
	checkForAlError(alSource3f(sourceId, AL_VELOCITY, 0, 0, 0));
    checkForAlError(alSourcef(sourceId, AL_ROLLOFF_FACTOR, 1.0f));

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
		//alSourceQueueBuffers(sourceId, 1, &bufferId);
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

void NodeOpenAL::setListenerPosition(glm::vec3 _position){
	checkForAlError(alListener3f(AL_POSITION, _position.x, _position.y, _position.z));
}
void NodeOpenAL::setListenerVelocity(glm::vec3 _velocity){
	checkForAlError(alListener3f(AL_VELOCITY, _velocity.x, _velocity.y, _velocity.z));
}
void NodeOpenAL::setListenerOrientation(glm::vec3 _forward, glm::vec3 _up){
	float orientation[6] = {_forward.x, _forward.y, _forward.z, _up.x, _up.y, _up.z};
	checkForAlError(alListenerfv(AL_ORIENTATION, orientation));
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








OpenAL_Sound::OpenAL_Sound(OpenAL_Source * _source) :
	NodeResource(false),
	source(_source)
{
	
}
OpenAL_Sound::OpenAL_Sound(const char * _filename, bool _positional) :
	NodeResource(false),
	source(new OpenAL_Source(new OpenAL_Buffer(_filename), _positional))
{
	
}

OpenAL_Sound::~OpenAL_Sound(){
	source->decrementAndDelete();
}


void OpenAL_Sound::update(Step * _step){
	source->update(_step);
	if(source->positional){
		//this->parents.at(0)->translate(0.1, 0, 0);
		source->setPosition(this->getWorldPos());
		//alListener3f(AL_VELOCITY, vel[0], vel[1], vel[2]);
		//alListenerfv(AL_ORIENTATION, orientation);
	}
}

float OpenAL_Sound::getAmplitude(){
	ALint t = source->getSampleOffset();
	if(t == -1){
		// if the source isn't sampling anything, the amplitude has to be zero
		return 0;
	}
	return (float)source->buffer->samples.at(t)/INT16_MAX;
}

OpenAL_Stream::OpenAL_Stream(const char * _filename, bool _positional) :
	NodeResource(false),
	isStreaming(false),
	source(new OpenAL_Source(nullptr, _positional)),
	stream(nullptr)
{
	stream = alureCreateStreamFromFile(_filename, 44100/10, NUM_BUFS, buffers);
}


OpenAL_Stream::~OpenAL_Stream(){
	alureDestroyStream(stream, NUM_BUFS, buffers);
	source->decrementAndDelete();
}

void OpenAL_Stream::update(Step * _step){
	// keep the stream informed of the source state
	source->update(_step);


    // Get the number of buffers that have been processed and are ready for reuse
    ALint numBufs = 0;
	checkForAlError(alGetSourcei(source->sourceId, AL_BUFFERS_PROCESSED, &numBufs));
	
	while(numBufs--){
		ALuint tempBuf;
		// unqueue the processed buffer
		checkForAlError(alSourceUnqueueBuffers(source->sourceId, 1, &tempBuf));
		// attempt to fill the unqueued buffer with new data from the stream
		ALsizei numToQueue = alureBufferDataFromStream(stream, 1, &tempBuf);
		if(numToQueue <= 0){
			// if the stream didn't fill any of the buffers,
			// the stream end has been reached, so we rewind it.
			if(!alureRewindStream(stream)){
				std::cout << alureGetErrorString();
				assert(false);
			}
			if(!source->looping){
				isStreaming = false;
			}else{
				// now that the stream is rewound, we can try to grab
				// a new buffer so that we can loop
				numToQueue = alureBufferDataFromStream(stream, 1, &tempBuf);
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

void OpenAL_Stream::play(bool _loop){
	if(source->state == AL_PAUSED){
		// if the stream is paused, we can simply resume playing and return early
		source->state = AL_PLAYING;

		// if there aren't any buffers queued, queue up some more (otherwise nothing will play)
		ALint numQueuedBuffers = 0;
		checkForAlError(alGetSourcei(source->sourceId, AL_BUFFERS_QUEUED, &numQueuedBuffers));
		if(numQueuedBuffers <= 0){
			ALsizei numBuffs = alureBufferDataFromStream(stream, NUM_BUFS, buffers);
			checkForAlError(alSourceQueueBuffers(source->sourceId, numBuffs, buffers));
		}

		checkForAlError(alSourcePlay(source->sourceId));
		isStreaming = true;
		
		source->looping = _loop; // we have to set looping to true here because the call to pause() will have set it to false
		return;
	}
	
	stop();
	ALsizei numBuffs = alureBufferDataFromStream(stream, NUM_BUFS, buffers);
	checkForAlError(alSourceQueueBuffers(source->sourceId, numBuffs, buffers));
	source->play(false);
	source->looping = _loop; // we can't call play(_loop) because it sets AL_LOOPING to _loop, and you aren't supposed to do that on a streaming source

	isStreaming = true;
}

void OpenAL_Stream::stop(){
	isStreaming = false;

	source->stop();
	rewind();
}

void OpenAL_Stream::pause(){
	// if we aren't playing, then we can't pause
	if(source->state != AL_PLAYING){
		return;
	}

	isStreaming = false;

	source->pause();
}

void OpenAL_Stream::rewind(){
	// move to the beginning
	ALint numQueuedBuffers = 0;
	checkForAlError(alGetSourcei(source->sourceId, AL_BUFFERS_QUEUED, &numQueuedBuffers));
	ALuint tempBuf[NUM_BUFS];
	checkForAlError(alSourceUnqueueBuffers(source->sourceId, numQueuedBuffers, tempBuf));
	if(!alureRewindStream(stream)){
		std::cout << alureGetErrorString();
		assert(false);
	}
}