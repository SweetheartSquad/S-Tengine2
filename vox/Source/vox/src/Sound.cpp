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

void Sound::play(bool _loop){
	if(_loop){
		libZPlay::TStreamTime start;
		libZPlay::TStreamTime end;
		libZPlay::TStreamInfo info;
		player->GetStreamInfo(&info);
		start.ms = 0;
		end.ms = info.Length.ms-25;

		player->Stop();
		player->PlayLoop(libZPlay::TTimeFormat::tfMillisecond, &start, libZPlay::TTimeFormat::tfMillisecond, &end, -1, 0);
	}else{
		player->Stop();
		player->Play();
	}
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