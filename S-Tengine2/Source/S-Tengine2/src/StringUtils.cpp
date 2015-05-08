#pragma once

#include "StringUtils.h"

std::string StringUtils::trim(std::string _toTrim){
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

std::string StringUtils::removeDigits(std::string _string){
	std::string retString;
	for(auto c : _string) {
		if(c < 48 || c > 57) {
			retString += c;
		}
	}
	return retString;
}

std::string StringUtils::removeSymbols(std::string _string, bool _keepSpaces){
	std::string retString;
	for(auto c : _string) {
		if((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || c == 32) {
			retString += c;
		}
	}
	return retString;
}

std::string StringUtils::removeLetters(std::string _string){
	std::string retString;
	for(auto c : _string) {
		if(c < 65 || c > 90) {
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