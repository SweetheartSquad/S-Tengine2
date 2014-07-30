#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
//#include <stdio.h>

#define READ "r"
#define WRTIE "w"
#define APPEND "a"

class FileUtils{
public:
	static std::string voxReadFile(const char* filename);
};
