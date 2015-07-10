#pragma once

#include <Log.h>
#include <Windows.h>

bool Log::THROW_ON_WARN  = false;
bool Log::THROW_ON_ERROR = false;

int Log::INFO_COLOUR_FG = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
int Log::WARN_COLOUR_FG = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
int Log::ERROR_COLOUR_FG = FOREGROUND_RED | FOREGROUND_INTENSITY;

int Log::INFO_COLOUR_BG = 0;
int Log::WARN_COLOUR_BG = 0;
int Log::ERROR_COLOUR_BG = 0;

#ifdef _DEBUG
Log::LogLevel Log::LOG_LEVEL = Log::LogLevel::kINFO;
#else
Log::LogLevel Log::LOG_LEVEL = Log::LogLevel::kNONE;
#endif

void Log::warn(std::string _message){
	if(LOG_LEVEL <= kWARN){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WARN_COLOUR_FG | WARN_COLOUR_BG);
		std::cout << "[WARN] " << _message << std::endl;
	}
	if(THROW_ON_WARN){
		throw;
	}
}

void Log::error(std::string _message){
	if(LOG_LEVEL <= kERROR){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ERROR_COLOUR_FG | ERROR_COLOUR_BG);
		std::cout << "[ERROR] " << _message << std::endl;
	}
	if(THROW_ON_ERROR){
		throw;
	}
}

void Log::info(std::string _message){
	if(LOG_LEVEL <= kINFO){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), INFO_COLOUR_FG | INFO_COLOUR_BG);
		std::cout << "[INFO] " << _message << std::endl;
	}
}