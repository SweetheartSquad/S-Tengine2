#pragma once

#include <OpenALSound.h>
#include <Transform.h>

ALCcontext * OpenAL_Sound::context = nullptr; 
ALCdevice * OpenAL_Sound::device = nullptr;

bool OpenAL_Sound::inited = false;

void OpenAL_Sound::initOpenAL(){
	// initialize OpenAL
	if(!inited){
		/*alureInitDevice(nullptr, nullptr);*/
		
	   ALCint contextAttr[] = {ALC_FREQUENCY, 44100,0};
	   device = alcOpenDevice( NULL );
	   context = alcCreateContext( device, contextAttr );
	   alcMakeContextCurrent( context );

	   /* listener at origin, facing down -z (ears at 1.5m height) */
	   alListener3f( AL_POSITION, 0., 1.5, 0. );
	   alListener3f( AL_VELOCITY, 0., 0., 0. );
	   float orient[6] = { /*fwd:*/ 0., 0., -1., /*up:*/ 0., 1., 0. };
	   alListenerfv( AL_ORIENTATION, orient );
	   
	   inited = true;
	}
}

void OpenAL_Sound::uninitOpenAL(){
	if(inited){
		//alureShutdownDevice();
		alcDestroyContext(context);
		alcCloseDevice(device);
		inited = false;
	}
}

void OpenAL_Sound::setListenerPosition(glm::vec3 _position){
	alListener3f(AL_POSITION, _position.x, _position.y, _position.z);
}
void OpenAL_Sound::setListenerVelocity(glm::vec3 _velocity){
	alListener3f(AL_POSITION, _velocity.x, _velocity.y, _velocity.z);
}
void OpenAL_Sound::setListenerOrientation(glm::vec3 _forward, glm::vec3 _up){
	float orientation[6] = {_forward.x, _forward.y, _forward.z, _up.x, _up.y, _up.z};
	alListenerfv(AL_ORIENTATION, orientation);
}

OpenAL_Sound::OpenAL_Sound(const char * _filename, bool _loop) :
	loop(_loop)
{
	initOpenAL();
	ALenum error;

	
	// generate source
	alGenSources(1, &sourceId);
	if ((error = alGetError()) != AL_NO_ERROR)
	std::cout << "alGenSources : " << error << std::endl;
	// turn off looping
	alSourcei(sourceId, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	alSourcef(sourceId, AL_PITCH, 1.f);
	alSourcef(sourceId, AL_GAIN, 1.f);
	alSource3f(sourceId, AL_POSITION, 0, 0, 0);
	alSource3f(sourceId, AL_VELOCITY, 0, 0, 0);

	float orientation[6] = {
		/*forward vector*/
		1.f, 0.f, 0.f,
		/*up vector*/
		0.f, 1.f, 0.f
	};
	alSourcefv(sourceId, AL_DIRECTION, orientation);


	// generate a buffer
	alGenBuffers(1, &bufferId);
	if ((error = alGetError()) != AL_NO_ERROR)
	std::cout << "alGenBuffers : " << error << std::endl;

	
   /*{
      long dataSize;
	  const ALvoid* data = load(_filename, &dataSize );
      // for simplicity, assume raw file is signed-16b at 44.1kHz 
	  //alBufferData( bufferId, AL_FORMAT_MONO16, data, dataSize, 44100 );
	  //alureBufferDataFromMemory((const ALubyte *)data, dataSize, bufferId);
      free( (void*)data );
   }*/

	ALboolean alureStat = alureBufferDataFromFile(_filename, bufferId);
	if(alureStat == AL_FALSE){
		std::cout << "alureBufferDataFromFile: " << alureGetErrorString() << std::endl;
	}
   
	// attach the buffer to the source
	alSourcei(sourceId, AL_BUFFER, bufferId); 
	

	// Start playing source
	alSourcePlay(sourceId);
	if ((error = alGetError()) != AL_NO_ERROR)
	std::cout << "alSourcePlay source 0 : " << error << std::endl;
}

OpenAL_Sound::~OpenAL_Sound(){
 // alureDestroyStream(stream, 0, &bufferId);
   alDeleteSources( 1, &sourceId );
   alDeleteBuffers( 1, &bufferId );
}

void OpenAL_Sound::setPosition(glm::vec3 _pos){
	alSource3f(sourceId, AL_POSITION, _pos.x, _pos.y, _pos.z);
}
void OpenAL_Sound::setDirection(glm::vec3 _forward, glm::vec3 _up){
	float orientation[6] = {_forward.x, _forward.y, _forward.z, _up.x, _up.y, _up.z};
	alSourcefv(sourceId, AL_ORIENTATION, orientation);
}



void OpenAL_Sound::update(Step * _step){
}


OpenAL_Stream::OpenAL_Stream(const char * _filename) :
	OpenAL_Sound(_filename, true)
{
}


OpenAL_Stream::~OpenAL_Stream(){
  //alureDestroyStream(stream, 0, &bufferId);
   alDeleteSources(1, &sourceId);
   alDeleteBuffers(1, &bufferId);
}

void OpenAL_Stream::update(Step * _step){
	//this->parents.at(0)->translate(0.1, 0, 0);
	setPosition(this->getWorldPos());
	//alListener3f(AL_VELOCITY, vel[0], vel[1], vel[2]);
	//alListenerfv(AL_ORIENTATION, orientation);
}