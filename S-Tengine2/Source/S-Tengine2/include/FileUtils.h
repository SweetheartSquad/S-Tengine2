#pragma once

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "shell32.lib")
#include <shlwapi.h>
#include <shlobj.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class FileUtils{
public:
	// reads the contents of a file located at _filename and returns them as a string
	static std::string readFile(const std::string & _filename);

	// if the directory at _src does not exist, attempts to create it
	// returns whether or not the directory exists at the end of the operation
	// Note: this is Windows-specific
	static bool createDirectoryIfNotExists(const std::string & _src);

	// if the file at _src does not exist, attempts to create a blank file in its place
	// returns whether or not the file exists at the end of the operation
	// Note: this is a lazy, thread-unsafe check
	static bool createFileIfNotExists(const std::string & _src);
};
