#pragma once

#include <string>
#include <iostream>

/********************************************************
*
* A simple class for logging to the console
* TODO : Add file logging 
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
	static void warn(std::string _message);
	
	/**
	* Logs an error message - If THROW_ON_ERROR is true an exception will be thrown
	*
	* @param _message The message to display - Will be prepended with [ERROR]
	*/
	static void error(std::string _message);
	
	/**
	* Logs an ingo message
	*
	* @param _message The message to display - Will be prepended with [INFO]
	*/
	static void info(std::string _message);

private:
	static void logToFile(std::string _message);
	static std::string getDate();
};