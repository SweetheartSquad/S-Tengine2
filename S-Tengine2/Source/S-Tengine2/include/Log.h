#pragma once

#include <string>
#include <iostream>

/********************************************************
*
* A simple class for logging to the console
* TODO : Add file logging 
*
*********************************************************/
namespace Log {

	/** Whether or not warnings should be thrown */
	extern bool THROW_ON_WARN;
	
	/** Whether or not errors should be thrown */
	extern bool THROW_ON_ERROR;

	/**
	* Logs a warning message - If THROW_ON_WARN is true an exception will be thrown
	*
	* @param _message The message to display - Will be prepended with [WARN]
	*/
	void warn(std::string _message);
	
	/**
	* Logs an error message - If THROW_ON_ERROR is true an exception will be thrown
	*
	* @param _message The message to display - Will be prepended with [ERROR]
	*/
	void error(std::string _message);
	
	/**
	* Logs an ingo message
	*
	* @param _message The message to display - Will be prepended with [INFO]
	*/
	void info(std::string _message);
};