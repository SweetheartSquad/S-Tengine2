#pragma once

#include "StringUtils.h"
#include <CharacterUtils.h>

std::wstring StringUtils::trim(std::wstring _toTrim){
	std::wstring retString = L"";
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

std::wstring StringUtils::removeDigits(std::wstring _string){
	std::wstring retString;
	for(auto c : _string) {
		if(!CharacterUtils::isDigit(c)) {
			retString += c;
		}
	}
	return retString;
}

std::wstring StringUtils::removeSymbols(std::wstring _string, bool _keepSpaces){
	std::wstring retString;
	for(auto c : _string) {
		if(!CharacterUtils::isSymbol(c)) {
			retString += c;
		}
	}
	return retString;
}

std::wstring StringUtils::removeLetters(std::wstring _string){
	std::wstring retString;
	for(auto c : _string) {
		if(!CharacterUtils::isLetter(c)) {
			retString += c;
		}
	}
	return retString;
}

std::vector<std::wstring> StringUtils::split(std::wstring _string, char _splitOn){
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

