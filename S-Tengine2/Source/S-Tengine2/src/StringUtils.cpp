#pragma once

#include "StringUtils.h"
#include <CharacterUtils.h>

std::string StringUtils::trim(std::string _toTrim){
	std::string retString = "";
	bool started = false;
	int sinceChar = 0;
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

std::string StringUtils::removeDigits(std::string _string){
	std::string retString;
	for(auto c : _string) {
		if(!CharacterUtils::isDigit(c)) {
			retString += c;
		}
	}
	return retString;
}

std::string StringUtils::removeSymbols(std::string _string, bool _keepSpaces){
	std::string retString;
	for(auto c : _string) {
		if(!CharacterUtils::isSymbol(c)) {
			retString += c;
		}
	}
	return retString;
}

std::string StringUtils::removeLetters(std::string _string){
	std::string retString;
	for(auto c : _string) {
		if(!CharacterUtils::isLetter(c)) {
			retString += c;
		}
	}
	return retString;
}

std::vector<std::string> StringUtils::split(std::string _string, char _splitOn){
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