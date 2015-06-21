#pragma once

#include <Log.h>

bool Log::THROW_ON_WARN  = false;
bool Log::THROW_ON_ERROR = false;

void Log::warn(std::string _message){
	std::cout << "[WARN] " << _message << std::endl;
	if(THROW_ON_WARN){
		throw;
	}
}

void Log::error(std::string _message){
	std::cout << "[ERROR] " << _message << std::endl;
	if(THROW_ON_ERROR){
		throw;
	}
}

void Log::info(std::string _message){
	std::cout << "[INFO] " << _message << std::endl;
}