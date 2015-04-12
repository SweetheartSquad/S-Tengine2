#pragma once

#include "Sound.h"

#include <iostream>
#include <libzplay.h>

unsigned long int Sound::masterVolume = 100;

Sound::Sound() :
	player(libZPlay::CreateZPlay())
{
}

Sound::Sound(std::string _file) :
	player(libZPlay::CreateZPlay())
{
	addFile(_file);
}

Sound::~Sound(){
}

void Sound::play(){
	player->Play();
	player->SetPlayerVolume(masterVolume, masterVolume);
}

void Sound::pause(){
	player->Pause();
}

void Sound::stop(){
	player->Stop();
}

void Sound::resume(){
	player->Resume();
}

void Sound::addFile(std::string _file){
	int result = player->OpenFile(_file.c_str(), libZPlay::sfAutodetect);
    if(result == 0)
    {
		std::cout << player->GetError();
        player->Release();
    }
}