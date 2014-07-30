#pragma once

#include <iostream>
#include <stdio.h>

#define READ "r"
#define WRTIE "w"
#define APPEND "a"

class FileUtils{
public:
	static char*voxReadFile(const char* filename);
};
