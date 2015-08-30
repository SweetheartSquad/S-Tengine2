#pragma once

#include <Log.h>
#include <Windows.h>
#include <FileUtils.h>
#include <ctime>

bool Log::THROW_ON_WARN  = false;
bool Log::THROW_ON_ERROR = false;

bool Log::FORCE_VERBOSE = false;

bool Log::WRITE_INFO_TO_FILE  = true;
bool Log::WRITE_WARN_TO_FILE  = true;
bool Log::WRITE_ERROR_TO_FILE = true;

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

void Log::warn(char * _file, int _line, std::string _message){
	if(LOG_LEVEL <= kWARN){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WARN_COLOUR_FG | WARN_COLOUR_BG);
		std::cout << "[WARN] FILE:" << _file << " LINE:" << _line << " " << _message << std::endl;
		if(WRITE_WARN_TO_FILE) {
			logToFile(getDate() + "[WARN] FILE:" + _file + " LINE:" + std::to_string(_line) + _message + "\n");
		}
	}
	if(THROW_ON_WARN){
		throw;
	}
}

void Log::warn(std::string _message){
	if(LOG_LEVEL <= kWARN){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WARN_COLOUR_FG | WARN_COLOUR_BG);
		std::cout << "[WARN] "<< _message << std::endl;
		if(WRITE_WARN_TO_FILE) {
			logToFile(getDate() + "[WARN] " + _message + "\n");
		}
	}
	if(THROW_ON_WARN){
		throw;
	}
}

void Log::error(char * _file, int _line, std::string _message){
	if(LOG_LEVEL <= kERROR){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ERROR_COLOUR_FG | ERROR_COLOUR_BG);
		std::cout << "[ERROR] FILE:" << _file << " LINE:" << _line << " "  << _message << std::endl;
		if(WRITE_ERROR_TO_FILE) {
			logToFile(getDate() + "[ERROR] FILE:" + _file + " LINE:" + std::to_string(_line) + _message + "\n");
		}
	}
	if(THROW_ON_ERROR){
		throw;
	}
}

void Log::error(std::string _message){
	if(LOG_LEVEL <= kERROR){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ERROR_COLOUR_FG | ERROR_COLOUR_BG);
		std::cout << "[ERROR]" << _message << std::endl;
		if(WRITE_ERROR_TO_FILE) {
			logToFile(getDate() + "[ERROR] " + _message + "\n");
		}
	}
	if(THROW_ON_ERROR){
		throw;
	}
}

void Log::info(char * _file, int _line, std::string _message){
	if(LOG_LEVEL <= kINFO){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), INFO_COLOUR_FG | INFO_COLOUR_BG);
		std::cout << "[INFO] FILE:" << _file << " LINE:" << _line << " "  << _message << std::endl;
		if(WRITE_INFO_TO_FILE) {
			logToFile(getDate() + "[INFO] FILE:" + _file + " LINE:" + std::to_string(_line) + _message + "\n");
		}
	}
}

void Log::info(std::string _message){
	if(LOG_LEVEL <= kINFO){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), INFO_COLOUR_FG | INFO_COLOUR_BG);
		std::cout << "[INFO] " << _message << std::endl;
		if(WRITE_INFO_TO_FILE) {
			logToFile(getDate() + "[INFO] " +  _message + "\n");
		}
	}
}


void Log::logToFile(std::string _message) {
	std::string logFileName = "data/log.txt";
	FileUtils::createFileIfNotExists(logFileName);
	std::ofstream log(logFileName, std::ios_base::app);
	log << _message;
	log.close();
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