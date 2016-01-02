#pragma once

#include "StringUtils.h"
#include <CharacterUtils.h>
#include <sstream>

std::wstring sweet::StringUtils::trim(std::wstring _toTrim){
	std::wstring retString = L"";
	bool started = false;
	unsigned long int sinceChar = 0;
	for(auto c : _toTrim) {
		if(!started) {
			if(c == L' ') {
				continue;
			}else {
				retString += c;
				started = true;
				continue;
			}
		}else {
			if(c == L' ') {
				sinceChar++;
				retString += c;
			}else {
				sinceChar = 0;
				retString += c;
			}
		}
	}
	for(unsigned long int i = 0; i < sinceChar; ++i) {
		retString.pop_back();
	}
	return retString;
}

std::wstring sweet::StringUtils::pad(std::wstring _toPad, wchar_t _paddingChar, unsigned long int _length, bool _padLeft){
	// if the string is already large enough, return early without modification
	if(_toPad.size() >= _length){
		return _toPad;
	}


	std::wstringstream res;
	if(!_padLeft){
		res << _toPad;
	}
	unsigned long int numPaddingChars = _length - _toPad.size();
	for(unsigned long int i = 0; i < numPaddingChars; ++i){
		res << _paddingChar;
	}
	if(_padLeft){
		res << _toPad;
	}
	return res.str();
}

std::wstring sweet::StringUtils::removeDigits(std::wstring _string){
	std::wstring retString;
	for(auto c : _string) {
		if(!CharacterUtils::isDigit(c)) {
			retString += c;
		}
	}
	return retString;
}

std::wstring sweet::StringUtils::removeSymbols(std::wstring _string, bool _keepSpaces){
	std::wstring retString;
	for(auto c : _string) {
		if(!CharacterUtils::isSymbol(c)) {
			retString += c;
		}
	}
	return retString;
}

std::wstring sweet::StringUtils::removeLetters(std::wstring _string){
	std::wstring retString;
	for(auto c : _string) {
		if(!CharacterUtils::isLetter(c)) {
			retString += c;
		}
	}
	return retString;
}

std::vector<std::wstring> sweet::StringUtils::split(std::wstring _string, wchar_t _splitOn){
	std::vector<std::wstring> retVec;
	std::wstring accumulator(L"");
	for(auto c : _string) {
		if(c == _splitOn) {
			retVec.push_back(accumulator);
			accumulator = L"";
		}else {
			accumulator += c;
		}
	}
	if(accumulator.compare(L"") != 0) {
		retVec.push_back(accumulator);
	}
	return retVec;
}







std::string sweet::StringUtils::trim(std::string _toTrim){
	std::string retString = "";
	bool started = false;
	unsigned long int sinceChar = 0;
	for(auto c : _toTrim) {
		if(!started) {
			if(c == ' ') {
				continue;
			}else {
				retString += c;
				started = true;
				continue;
			}
		}else {
			if(c == ' ') {
				sinceChar++;
				retString += c;
			}else {
				sinceChar = 0;
				retString += c;
			}
		}
	}
	for(unsigned long int i = 0; i < sinceChar; ++i) {
		retString.pop_back();
	}
	return retString;
}

std::string sweet::StringUtils::pad(std::string _toPad, char _paddingChar, unsigned long int _length, bool _padLeft){
	// if the string is already large enough, return early without modification
	if(_toPad.size() >= _length){
		return _toPad;
	}


	std::stringstream res;
	if(!_padLeft){
		res << _toPad;
	}
	unsigned long int numPaddingChars = _length - _toPad.size();
	for(unsigned long int i = 0; i < numPaddingChars; ++i){
		res << _paddingChar;
	}
	if(_padLeft){
		res << _toPad;
	}
	return res.str();
}

std::string sweet::StringUtils::removeDigits(std::string _string){
	std::string retString;
	for(auto c : _string) {
		if(!CharacterUtils::isDigit(c)) {
			retString += c;
		}
	}
	return retString;
}

std::string sweet::StringUtils::removeSymbols(std::string _string, bool _keepSpaces){
	std::string retString;
	for(auto c : _string) {
		if(!CharacterUtils::isSymbol(c)) {
			retString += c;
		}
	}
	return retString;
}

std::string sweet::StringUtils::removeLetters(std::string _string){
	std::string retString;
	for(auto c : _string) {
		if(!CharacterUtils::isLetter(c)) {
			retString += c;
		}
	}
	return retString;
}

std::vector<std::string> sweet::StringUtils::split(std::string _string, char _splitOn){
	std::vector<std::string> retVec;
	std::string accumulator("");
	for(auto c : _string) {
		if(c == _splitOn) {
			retVec.push_back(accumulator);
			accumulator = "";
		}else {
			accumulator += c;
		}
	}
	if(accumulator.compare("") != 0) {
		retVec.push_back(accumulator);
	}
	return retVec;
}

