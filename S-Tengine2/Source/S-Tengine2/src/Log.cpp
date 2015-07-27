#pragma once

#include <Log.h>
#include <Windows.h>
#include <FileUtils.h>
#include <ctime>

bool Log::THROW_ON_WARN  = false;
bool Log::THROW_ON_ERROR = false;

bool Log::WRITE_TO_FILE  = true;

int Log::INFO_COLOUR_FG  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
int Log::WARN_COLOUR_FG  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
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
		if(WRITE_TO_FILE) {
			logToFile(getDate() + "[WARN] " + _message + "\n");
		}
	}
	if(THROW_ON_WARN){
		throw;
	}
}

void Log::error(std::string _message){
	if(LOG_LEVEL <= kERROR){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ERROR_COLOUR_FG | ERROR_COLOUR_BG);
		std::cout << "[ERROR] " << _message << std::endl;
		if(WRITE_TO_FILE) {
			logToFile(getDate() + "[ERROR] " + _message + "\n");
		}
	}
	if(THROW_ON_ERROR){
		throw;
	}
}

void Log::info(std::string _message){
	if(LOG_LEVEL <= kINFO){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), INFO_COLOUR_FG | INFO_COLOUR_BG);
		std::cout << "[INFO] " << _message << std::endl;
		if(WRITE_TO_FILE) {
			logToFile(getDate() + "[INFO] " +  _message + "\n");
		}
	}
}

void Log::logToFile(std::string _message) {
	FileUtils::createFileIfNotExists("LOG.txt");
	std::ofstream log;
	log.open("LOG.txt", std::ios_base::app);
	log << _message; 
}

std::string Log::getDate() {
	time_t t = time(0);
	struct tm now;
	localtime_s(&now, &t);
	std::stringstream out;
	out << "["
		<< (now.tm_year + 1900)
		<< '-'
		<< (now.tm_mon + 1)
		<< '-'
		<< now.tm_mday
		<< '_'
		<< now.tm_hour
		<< ":"
		<< now.tm_min
		<< ":"
		<< now.tm_sec
		<< "] ";
	return out.str();
}