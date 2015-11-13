#pragma once

#include <string>
#include <vector>

class StringUtils{
public:

	static std::vector<std::wstring> split(std::wstring _string, char _splitOn);
	static std::wstring trim(std::wstring _toTrim);
	static std::wstring removeDigits(std::wstring _string);
	static std::wstring removeSymbols(std::wstring _string, bool _keepSpaces);
	static std::wstring removeLetters(std::wstring _string);
};