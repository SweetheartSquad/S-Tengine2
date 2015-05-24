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
	NodeResource(false)
{
	++_buffer->referenceCount;
	// generate source
	alGenSources(1, &sourceId);

	// turn off looping by default
	alSourcei(sourceId, AL_LOOPING, AL_FALSE);
	alSourcef(sourceId, AL_PITCH, 1.f);
	alSourcef(sourceId, AL_GAIN, 1.f);
	alSourcef(sourceId, AL_ROLLOFF_FACTOR, 0.05f);
	alDopplerFactor(1.f);
	alDopplerVelocity(1.f);
	alSource3f(sourceId, AL_POSITION, 0, 0, 0);
	alSource3f(sourceId, AL_VELOCITY, 0, 0, 0);

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

	
	// attach the buffer to the source
	alSourcei(sourceId, AL_BUFFER, buffer->bufferId);
	//alSourceQueueBuffers(sourceId, 1, &bufferId);
}

OpenAL_Source::~OpenAL_Source(){
	alDeleteSources(1, &sourceId);
	buffer->decrementAndDelete();
}


void NodeOpenAL::setListenerPosition(glm::vec3 _position){
	alListener3f(AL_POSITION, _position.x, _position.y, _position.z);
}
void NodeOpenAL::setListenerVelocity(glm::vec3 _velocity){
	alListener3f(AL_VELOCITY, _velocity.x, _velocity.y, _velocity.z);
}
void NodeOpenAL::setListenerOrientation(glm::vec3 _forward, glm::vec3 _up){
	float orientation[6] = {_forward.x, _forward.y, _forward.z, _up.x, _up.y, _up.z};
	alListenerfv(AL_ORIENTATION, orientation);
}

void OpenAL_Source::setPosition(glm::vec3 _pos){
	alSource3f(sourceId, AL_POSITION, _pos.x, _pos.y, _pos.z);
}
void OpenAL_Source::setDirection(glm::vec3 _forward, glm::vec3 _up){
	float orientation[6] = {_forward.x, _forward.y, _forward.z, _up.x, _up.y, _up.z};
	alSourcefv(sourceId, AL_ORIENTATION, orientation);
}

void OpenAL_Source::play(bool _loop){	
	// set the loop parameter
	alSourcei(sourceId, AL_LOOPING, _loop);

	// Start playing source
	alSourcePlay(sourceId);
}

void OpenAL_Source::stop(){	
	// Stop playing source
	alSourceStop(sourceId);
}

void OpenAL_Source::pause(){	
	// Pause source
	alSourcePause(sourceId);
}
void OpenAL_Source::resume(){	
	// Start playing source
	alSourcePlay(sourceId);
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






/*
OpenAL_Stream::OpenAL_Stream() :
	OpenAL_Sound(_filename, _positional),
	NodeResource(false)
{
}


OpenAL_Stream::~OpenAL_Stream(){
}

void OpenAL_Stream::update(Step * _step){
	OpenAL_Sound::update(_step);
}*/