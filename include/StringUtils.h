#pragma once

#include <string>
#include <vector>

class StringUtils{
public:

	static std::vector<std::string> split(std::string _string, char _splitOn);
	static std::string trim(std::string _toTrim);
	static std::string removeDigits(std::string _string);
	static std::string removeSymbols(std::string _string, bool _keepSpaces);
	static std::string removeLetters(std::string _string);
};