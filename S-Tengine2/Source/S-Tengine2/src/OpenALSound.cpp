#pragma once

#include <OpenALSound.h>
#include <Transform.h>

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

	   /* listener at origin, facing down -z (ears at 1.5m height) */
	   alListener3f( AL_POSITION, 0., 1.5, 0. );
	   alListener3f( AL_VELOCITY, 0., 0., 0. );
	   float orient[6] = { /*fwd:*/ 0., 0., -1., /*up:*/ 0., 1., 0. };
	   alListenerfv( AL_ORIENTATION, orient );
	   
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

void NodeOpenAL::checkError(){
	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR){
		std::cout << "OpenAL Error: ";
		switch (error){
		case AL_NO_ERROR:
			std::cout << "AL_NO_ERROR";
			break;
		case AL_INVALID_NAME:
			std::cout << "AL_INVALID_NAME";
			break;
		case AL_INVALID_ENUM:
			std::cout << "AL_INVALID_ENUM";
			break;
		case AL_INVALID_VALUE:
			std::cout << "AL_INVALID_VALUE";
			break;
		case AL_INVALID_OPERATION:
			std::cout << "AL_INVALID_OPERATION";
			break;
		case AL_OUT_OF_MEMORY:
			std::cout << "AL_OUT_OF_MEMORY";
			break;
		default:
			break;
		}
		std::cout << std::endl;
		assert(false);
	}
}


OpenAL_Buffer::OpenAL_Buffer(const char * _filename) :
	NodeResource(false)
{
	// generate buffer
	alGenBuffers(1, &bufferId);

	ALboolean alureStat = alureBufferDataFromFile(_filename, bufferId);
	if(alureStat == AL_FALSE){
		std::cout << "alureBufferDataFromFile() error: " << alureGetErrorString() << std::endl;
	}
}

OpenAL_Buffer::~OpenAL_Buffer(){	
	alDeleteBuffers( 1, &bufferId );
}





OpenAL_Source::OpenAL_Source(OpenAL_Buffer * _buffer, bool _positional) :
	buffer(_buffer),
	positional(_positional),
	looping(false),
	NodeResource(false)
{
	// generate source
	alGenSources(1, &sourceId);

	// turn off looping by default
	alSourcei(sourceId, AL_LOOPING, AL_FALSE);
	alSourcef(sourceId, AL_PITCH, 1.f);
	alSourcef(sourceId, AL_GAIN, 0.08f);
	alSourcef(sourceId, AL_ROLLOFF_FACTOR, 0.05f);
	alDopplerFactor(1.f);
	alDopplerVelocity(1.f);
	alSource3f(sourceId, AL_POSITION, 0, 0, 0);
	alSource3f(sourceId, AL_VELOCITY, 0, 0, 0);
    alSourcef(sourceId, AL_ROLLOFF_FACTOR, 1.0f);

	float orientation[6] = {
		/*forward vector*/
		1.f, 0.f, 0.f,
		/*up vector*/
		0.f, 1.f, 0.f
	};
	alSourcefv(sourceId, AL_DIRECTION, orientation);
	if(!positional){
		alSourcei(sourceId, AL_SOURCE_RELATIVE, AL_TRUE);
	}

	if(_buffer != nullptr){
		// attach the buffer to the source
		alSourcei(sourceId, AL_BUFFER, buffer->bufferId);
		//alSourceQueueBuffers(sourceId, 1, &bufferId);
		++_buffer->referenceCount;
	}
}

OpenAL_Source::~OpenAL_Source(){
	alDeleteSources(1, &sourceId);
	buffer->decrementAndDelete();
}


void NodeOpenAL::setListenerPosition(glm::vec3 _position){
	alListener3f(AL_POSITION, _position.x, _position.y, _position.z);
	NodeOpenAL::checkError();
}
void NodeOpenAL::setListenerVelocity(glm::vec3 _velocity){
	alListener3f(AL_VELOCITY, _velocity.x, _velocity.y, _velocity.z);
	NodeOpenAL::checkError();
}
void NodeOpenAL::setListenerOrientation(glm::vec3 _forward, glm::vec3 _up){
	float orientation[6] = {_forward.x, _forward.y, _forward.z, _up.x, _up.y, _up.z};
	alListenerfv(AL_ORIENTATION, orientation);
	NodeOpenAL::checkError();
}

void OpenAL_Source::setPosition(glm::vec3 _pos){
	alSource3f(sourceId, AL_POSITION, _pos.x, _pos.y, _pos.z);
	NodeOpenAL::checkError();
}
void OpenAL_Source::setDirection(glm::vec3 _forward, glm::vec3 _up){
	float orientation[6] = {_forward.x, _forward.y, _forward.z, _up.x, _up.y, _up.z};
	alSourcefv(sourceId, AL_ORIENTATION, orientation);
	NodeOpenAL::checkError();
}

void OpenAL_Source::play(bool _loop){	
	// set the loop parameter
	looping = _loop;
	alSourcei(sourceId, AL_LOOPING, _loop);
	NodeOpenAL::checkError();

	// Start playing source
	alSourcePlay(sourceId);
	NodeOpenAL::checkError();
}

void OpenAL_Source::stop(){	
	// Stop playing source
	alSourceStop(sourceId);
	NodeOpenAL::checkError();
	looping = false;
}

void OpenAL_Source::pause(){	
	// Pause source
	alSourcePause(sourceId);
	NodeOpenAL::checkError();
	looping = false;
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
	if(source->positional){
		//this->parents.at(0)->translate(0.1, 0, 0);
		source->setPosition(this->getWorldPos());
		//alListener3f(AL_VELOCITY, vel[0], vel[1], vel[2]);
		//alListenerfv(AL_ORIENTATION, orientation);
	}
}







OpenAL_Stream::OpenAL_Stream(const char * _filename, bool _positional) :
	NodeResource(false),
	isStreaming(false),
	state(AL_STOPPED),
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
	alGetSourcei(source->sourceId, AL_SOURCE_STATE, &state);
	NodeOpenAL::checkError();


    // Get the number of buffers that have been processed and are ready for reuse
    ALint numBufs = 0;
	alGetSourcei(source->sourceId, AL_BUFFERS_PROCESSED, &numBufs);
	NodeOpenAL::checkError();

	while(numBufs--){
		ALuint tempBuf;
		alSourceUnqueueBuffers(source->sourceId, 1, &tempBuf);
		NodeOpenAL::checkError();
		ALsizei numToQueue = alureBufferDataFromStream(stream, 1, &tempBuf);
		if(numToQueue <= 0){
			if(!alureRewindStream(stream)){
				std::cout << alureGetErrorString();
				assert(false);
			}
			if(!source->looping){
				isStreaming = false;
			}else{
				// now that the stream is rewound, try to grab a new buffer so that we can continue playing
				numToQueue = alureBufferDataFromStream(stream, 1, &tempBuf);
			}
		}
		if(isStreaming){
			alSourceQueueBuffers(source->sourceId, numToQueue, &tempBuf);
			NodeOpenAL::checkError();
		}
	}

	if(isStreaming && state != AL_PLAYING){
		alSourcePlay(source->sourceId);
	}
}

void OpenAL_Stream::play(bool _loop){
	if(state == AL_PAUSED){
		// if the stream is paused, we can simply resume playing and return early
		state = AL_PLAYING;

		// if there aren't any buffers queued, queue up some more (otherwise nothing will play)
		ALint numQueuedBuffers = 0;
		alGetSourcei(source->sourceId, AL_BUFFERS_QUEUED, &numQueuedBuffers);
		NodeOpenAL::checkError();
		if(numQueuedBuffers <= 0){
			ALsizei numBuffs = alureBufferDataFromStream(stream, NUM_BUFS, buffers);
			alSourceQueueBuffers(source->sourceId, numBuffs, buffers);
			NodeOpenAL::checkError();
		}

		alSourcePlay(source->sourceId);
		NodeOpenAL::checkError();
		isStreaming = true;
		
		source->looping = _loop; // we have to set looping to true here because the call to pause() will have set it to false
		return;
	}
	
	stop();
	source->looping = _loop; // we have to set looping to true here because stop() sets it to false
	
	ALsizei numBuffs = alureBufferDataFromStream(stream, NUM_BUFS, buffers);
	alSourceQueueBuffers(source->sourceId, numBuffs, buffers);
	NodeOpenAL::checkError();

	alSourcePlay(source->sourceId);
	NodeOpenAL::checkError();

	isStreaming = true;
}

void OpenAL_Stream::stop(){
	state = AL_STOPPED;
	isStreaming = false;

	source->stop();
	rewind();
}

void OpenAL_Stream::pause(){
	// if we aren't playing, then we can't pause
	if(state != AL_PLAYING){
		return;
	}

	state = AL_PAUSED;
	if(!isStreaming){
		// if the stream isn't playing, we can return early
		return;
	}
	isStreaming = false;

	source->pause();
}

void OpenAL_Stream::rewind(){
	// move to the beginning
	ALint numQueuedBuffers = 0;
	alGetSourcei(source->sourceId, AL_BUFFERS_QUEUED, &numQueuedBuffers);
	NodeOpenAL::checkError();
	ALuint tempBuf[NUM_BUFS];
	alSourceUnqueueBuffers(source->sourceId, numQueuedBuffers, tempBuf);
	NodeOpenAL::checkError();
	if(!alureRewindStream(stream)){
		std::cout << alureGetErrorString();
		assert(false);
	}
}