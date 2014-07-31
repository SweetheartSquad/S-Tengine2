#pragma once

#include <GL\glew.h>
#include <iostream>

class GLUtils{
public:
	static void checkForError(bool printSuccess, std::string file, int line);
};