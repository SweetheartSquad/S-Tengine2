#pragma once

#include <string>
#include <iostream>

// Verbose logging(File and Line included)
#define ST_LOG_ERROR_V(message) Log::error(__FILE__, __LINE__, message);
#define ST_LOG_WARN_V(message)  Log::warn(__FILE__, __LINE__, message);
#define ST_LOG_INFO_V(message)  Log::info(__FILE__, __LINE__, message);

// Just the message
#define ST_LOG_ERROR(message) Log::error(message);
#define ST_LOG_WARN(message)  Log::warn(message);
#define ST_LOG_INFO(message)  Log::info(message);

/********************************************************
*
* A simple class for logging to the console
*
*********************************************************/
class Log {
public:

	/** Whether or not warnings should be thrown */
	static bool THROW_ON_WARN;
	
	/** Whether or not errors should be thrown */
	static bool THROW_ON_ERROR;

	/** Wherer or not messages should be logged to a file*/
	static bool WRITE_TO_FILE;
	
	static int INFO_COLOUR_FG;
	static int WARN_COLOUR_FG;
	static int ERROR_COLOUR_FG;
	static int INFO_COLOUR_BG;
	static int WARN_COLOUR_BG;
	static int ERROR_COLOUR_BG;
	
	static enum LogLevel{
		kINFO,
		kWARN,
		kERROR,
		kNONE
	} LOG_LEVEL;

	/**
	* Logs a warning message - If THROW_ON_WARN is true an exception will be thrown
	*
	* @param _message The message to display - Will be prepended with [WARN]
	*/
	static void warn(char * _file, int _line, std::string _message);
	static void warn(std::string _message);
	
	/**
	* Logs an error message - If THROW_ON_ERROR is true an exception will be thrown
	*
	* @param _message The message to display - Will be prepended with [ERROR]
	*/
	static void error(char * _file, int _line, std::string _message);
	static void error(std::string _message);
	
	/**
	* Logs an ingo message
	*
	* @param _message The message to display - Will be prepended with [INFO]
	*/
	static void info(char * _file, int _line, std::string _message);
	static void info(std::string _message);

private:
	static void logToFile(std::string _message);
	static std::string getDate();
};