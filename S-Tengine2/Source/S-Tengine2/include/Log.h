#pragma once

#include <string>
#include <iostream>

namespace Log {

	extern bool THROW_ON_WARN;
	extern bool THROW_ON_ERROR;

	void warn(std::string _message);
	void error(std::string _message);
	void info(std::string _message);
};