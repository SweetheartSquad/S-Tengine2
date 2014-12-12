#pragma once

#include <string>

class ConsoleEntry{
public:
	std::string message;
	enum Type{
		kLOG,
		kWARNING,
		kERROR
	} type;

	ConsoleEntry(std::string _message, Type _type);
};