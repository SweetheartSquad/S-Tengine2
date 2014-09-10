#pragma once

#include <GL\glew.h>
#include <iostream>

class GLUtils{
public:
	static void checkForError(bool _printSuccess, std::string _file, int _line);
};