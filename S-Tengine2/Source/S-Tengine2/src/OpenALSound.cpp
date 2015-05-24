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
	alSourcef(sourceId, AL_GAIN, 1.f);
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
}
void NodeOpenAL::setListenerVelocity(glm::vec3 _velocity){
	alListener3f(AL_POSITION, _velocity.x, _velocity.y, _velocity.z);
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
	looping = _loop;
	alSourcei(sourceId, AL_LOOPING, _loop);

	// Start playing source
	alSourcePlay(sourceId);
}

void OpenAL_Source::stop(){	
	// Stop playing source
	alSourceStop(sourceId);
	looping = false;
}

void OpenAL_Source::pause(){	
	// Pause source
	alSourcePause(sourceId);
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
	NodeResource(false)/*,
	curbuf(0)*/,
	active(false)
{
	/*source = new OpenAL_Source(nullptr, _positional);
	buffers[0] = new OpenAL_Buffer(_filename);
	buffers[1] = new OpenAL_Buffer(_filename);
	
	alSourceQueueBuffers(source->sourceId, 1, &buffers[0]->bufferId);
	alSourceQueueBuffers(source->sourceId, 1, &buffers[1]->bufferId);*/

	//alGenSources(1, &sourceId);
	source = new OpenAL_Source(nullptr, _positional);
	stream = alureCreateStreamFromFile(_filename, 44100/2, 2, buffers);
}


OpenAL_Stream::~OpenAL_Stream(){
	/*delete source;
	delete buffers[0];
	delete buffers[1];*/
	alureDestroyStream(stream, 2, buffers);
	source->decrementAndDelete();
}

void OpenAL_Stream::update(Step * _step){
	if(!active){
		return;
	}

	if(source->positional){
		source->setPosition(this->getWorldPos());
	}

	ALint state = AL_PLAYING;
	ALint processed = 0;
	 
	alGetSourcei(source->sourceId, AL_SOURCE_STATE, &state);
	alGetSourcei(source->sourceId, AL_BUFFERS_PROCESSED, &processed);
	if(processed > 0){
		ALuint bufs[2];
		alSourceUnqueueBuffers(source->sourceId, processed, bufs);
		processed = alureBufferDataFromStream(stream, processed, bufs);
		if(processed <= 0){
			alureRewindStream(stream);
			return; // done streaming
		}
	 
		alSourceQueueBuffers(source->sourceId, processed, bufs);
	}
	if(state != AL_PLAYING && source->looping){
		alureRewindStream(stream);
		alSourcePlay(source->sourceId);
	}

	NodeOpenAL::checkError();
}

void OpenAL_Stream::play(bool _loop){
	active = true;
	source->looping = _loop;
	//alureBufferDataFromStream(stream, 2, buffers);
	alSourceQueueBuffers(source->sourceId, 2, buffers);
	alSourcePlay(source->sourceId);
}

void OpenAL_Stream::stop(){
	active = false;
	source->stop();
	ALint processed = 0;
	alGetSourcei(source->sourceId, AL_BUFFERS_PROCESSED, &processed);
	alSourceUnqueueBuffers(source->sourceId, processed, buffers);
	alureRewindStream(stream);
}

void OpenAL_Stream::pause(){
	active = false;
	source->pause();
}