#pragma once

#include <string>
#include <vector>

namespace sweet{
class StringUtils{
public:

	static std::vector<std::wstring> split(std::wstring _string, wchar_t _splitOn);
	static std::wstring trim(std::wstring _toTrim);
	static std::wstring pad(std::wstring _toPad, wchar_t _paddingChar, unsigned long int _length, bool _padLeft);
	static std::wstring removeDigits(std::wstring _string);
	static std::wstring removeSymbols(std::wstring _string, bool _keepSpaces);
	static std::wstring removeLetters(std::wstring _string);

	static std::vector<std::string> split(std::string _string, char _splitOn);
	static std::string trim(std::string _toTrim);
	static std::string pad(std::string _toPad, char _paddingChar, unsigned long int _length, bool _padLeft);
	static std::string removeDigits(std::string _string);
	static std::string removeSymbols(std::string _string, bool _keepSpaces);
	static std::string removeLetters(std::string _string);
};
};